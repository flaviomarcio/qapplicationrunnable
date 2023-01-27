#include "./p_qapr_application.h"
#include "../qapr_startup.h"
#include "../qapr_consts.h"
#include "../qapr_macro.h"
//#include "../../qorm/src/qorm_macro.h"

namespace QApr{

Q_GLOBAL_STATIC(QString, applicationSettingDir)
Q_GLOBAL_STATIC(Application, staticInstance);
Q_GLOBAL_STATIC(QRpc::ServiceSetting, circuitBreakerSettings);

static const auto __env_file="ENV_FILE";
static const auto __static_env_file=":/env_file.env";

static void startCircuitBreaker()
{
    auto &manager=staticInstance->manager();
    *circuitBreakerSettings=manager.setting(QStringLiteral("circuit-breaker"));
}

static void startSettings()
{
    auto&i=*staticInstance;
    const auto &settingFile=i.resourceSettings();
    auto &manager=i.manager();
    auto &envs=i.envs();

#ifdef QT_DEBUG
    QString envFile=__static_env_file;
#else
    Q_UNUSED(__static_env_file)
    QString envFile;
#endif
    if(!envs.contains(__env_file)){
        auto file=envs.value(__env_file).toString();
        if(QFile::exists(file))
            envFile=file;
    }
    QFile file(envFile);
    if(file.exists())
        envs.customEnvs(file);

    if(file.exists()){
        aWarning()<<QObject::tr("loaded ${ENV_FILE}: [%1]").arg(file.fileName());
        envs.customEnvs(file);
    }


    if(!manager.load(settingFile.setting())){
        aWarning()<<QObject::tr("Connection manager: no loaded for %1").arg(settingFile.setting());
        return;
    }

    i.settings().setValues(settingFile.setting());
    if(!i.connectionManager().load(settingFile.setting())){
        aWarning()<<QObject::tr("Connection manager is not loaded for %1").arg(settingFile.setting());
        return;
    }

    aWarning() << QObject::tr("loaded settings: %1").arg(settingFile.setting());
}

static void startUp(Application &i)
{
    Q_UNUSED(i)
#ifdef QT_DEBUG
    i.resourceExtract();
#endif
    startSettings();
    startCircuitBreaker();
}

static void init()
{
    startUp(*staticInstance);
}

Q_APR_STARTUP_FUNCTION(init)

ApplicationPvt::ApplicationPvt(Application *parent)
    :
      QObject{parent}
{
    if(applicationSettingDir->isEmpty())
        *applicationSettingDir=QStringLiteral("%1/%2").arg(settings_HOME_DIR, qApp->applicationName().toLower());
    this->application=parent;
}

ApplicationPvt::~ApplicationPvt()
{
    circuitBreaker.stop();
}

const SettingFile &ApplicationPvt::resourceSettings()
{
    if(QFile::exists(settingFile.setting().trimmed()))
        return this->settingFile;

    static const auto __development_file=
    #ifdef Q_APR_TEST
        "settings.debug.json";
    #else
        #ifdef QT_DEBUG
            "settings.debug.json";
        #else
            "settings.release.json";
        #endif
    #endif

    this->settingFile.clear();
    {
        static const auto __application="application";
        static const auto __qt_reforce_file_dir=settings_HOME_DIR+"/"+qAppName().toLower();
        static const auto __listPaths=QStringList{{qApp->applicationDirPath(), __qt_reforce_file_dir}};
        static const auto __listFileNames=QStringList{{qApp->applicationName().toLower(), __application, __development_file}};
        static const auto __fileFormatSetting=QStringLiteral("%1/%2.settings.json");
        static const auto __fileFormatEnv=QStringLiteral("%1/%2.settings.env");
        auto checkFile=[this](const QString &path)
        {
            for(auto&fileName:__listFileNames){
                auto fileSetting=__fileFormatSetting.arg(path,fileName);
                auto fileEnvs=__fileFormatEnv.arg(path,fileName);
                if(QFile::exists(fileSetting)){
                    this->settingFile
                            .setting(fileSetting)
                            .envs(fileEnvs);
                    return true;
                }
            }
            return false;
        };

        for(auto&path:__listPaths){
            if(checkFile(path))
                break;
        }
    }

    if(this->settingFile.setting().isEmpty()){
        aWarning()<<"Application settings not found:";
        this->settingFile.clear();
    }

    return this->settingFile;

}

void ApplicationPvt::resourceExtract()
{
    QDir dirHome(*applicationSettingDir);
    if(!dirHome.exists())
        dirHome.mkpath(*applicationSettingDir);

    if(!dirHome.exists())
        return;

    QDir dir(QStringLiteral(":"));
    dir.setNameFilters(QStringList{QStringLiteral("*.json")});
    for(auto &info:dir.entryInfoList()){
        QFile fileSrc(info.filePath());
        QFile fileDst(QStringLiteral("%1/%2").arg(*applicationSettingDir,info.fileName()));
        if(fileDst.exists())
            continue;

        if(!fileSrc.open(fileSrc.ReadOnly)){
#if Q_RPC_LOG
            aWarning()<<QStringLiteral("No open file:")<<fileSrc.fileName()<<QStringLiteral(", error: ")<<fileSrc.errorString();
#endif
            continue;
        }
        if(!fileDst.open(fileDst.Truncate | fileDst.Unbuffered | fileDst.WriteOnly)){
#if Q_RPC_LOG
            aWarning()<<QStringLiteral("No open file:")<<fileDst.fileName()<<QStringLiteral(", error: ")<<fileDst.errorString();
#endif
            fileSrc.close();
            continue;
        }

        fileDst.write(fileSrc.readAll());
        fileDst.flush();
        fileDst.close();
        fileSrc.close();
    }
}

QVariantHash &ApplicationPvt::getArguments()
{
    if(!arguments.isEmpty())
        return arguments;

    for(auto &v:qApp->arguments()){
        auto l=v.split(QStringLiteral("="));
        if(l.isEmpty())
            continue;

        if(l.size()==1){
            auto key=l.first();
            auto value=l.last();
            arguments.insert(key, value);
            continue;
        }

        auto key=l.first().toLower();
        auto value=l.last();
        arguments.insert(key, value);
    }

    QHashIterator<QString, QVariant> i(manager.arguments());
    while (i.hasNext()) {
        i.next();
        arguments.insert(i.key(), i.value());
    }
    return arguments;
}

Application &ApplicationPvt::i()
{
    return *staticInstance;
}

void ApplicationPvt::circuitBreakerStart()
{
    if(circuitBreakerSettings->enabled()){
        this->circuitBreaker.setSettings(circuitBreakerSettings->toHash());
        if(this->circuitBreaker.start())
            this->circuitBreaker.print();
    }
}

}
