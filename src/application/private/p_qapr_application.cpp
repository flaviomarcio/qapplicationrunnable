#include "./p_qapr_application.h"
#include "../qapr_startup.h"
#include "../qapr_consts.h"
#include "../qapr_macro.h"
//#include "../../qorm/src/qorm_macro.h"

namespace QApr{

Q_GLOBAL_STATIC(QString, applicationSettingDir)
Q_GLOBAL_STATIC(Application, staticInstance);
Q_GLOBAL_STATIC(QRpc::ServiceSetting, circuitBreakerSettings);

static const auto __circuit_breaker="circuit-breaker";

static void startCircuitBreaker()
{
    auto &manager=staticInstance->manager();
    *circuitBreakerSettings=manager.setting(__circuit_breaker);
}

static void startSettings()
{
    auto&i=*staticInstance;
    const auto &settingFile=i.resourceSettings();
    auto &manager=i.manager();
    auto &envs=i.envs();

    QFile file(settingFile.envs());
    if(file.exists())
        envs
                .systemEnvs(file)
                .customEnvs(file);

    if(!manager.load(settingFile.setting())){
        aWarning()<<QObject::tr("Connection manager: no loaded for %1").arg(settingFile.setting());
        return;
    }

    i.settings().setValues(settingFile.setting());
    if(!i.connectionManager().load(settingFile.setting())){
        aWarning()<<QObject::tr("Connection manager is not loaded for %1").arg(settingFile.setting());
        return;
    }

    aWarning() << QObject::tr("loaded settings json: %1").arg(settingFile.setting());
    aWarning() << QObject::tr("loaded settings env: %1").arg(settingFile.envs());
}

static void startUp(Application &i)
{
    Q_UNUSED(i)
//#ifdef QT_DEBUG
//    i.resourceExtract();
//#endif
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

    auto findFile=[](const QString &ext){
#ifdef Q_APR_TEST
    #define __qapr__development_file "application-test.json"
#else
    #ifdef QT_DEBUG
        #define __qapr__development_file "application-development"
    #else
        #undef __qapr__development_file
    #endif
#endif
        static const auto __application="application";
#ifdef __qapr__development_file
        static const auto __listFileNames=QStringList{{qApp->applicationName().toLower(), __application, __qapr__development_file}};
#else
        static const auto __listFileNames=QStringList{{qApp->applicationName().toLower(), __application}};
#endif

        static const auto __root_path=":";
        static const auto __qapr_path=":/qapr";
        static const auto __listPaths=QStringList{{/*qApp->applicationDirPath(), */__root_path, __qapr_path}};
        static const auto __fileFormatSetting=QStringLiteral("%1/%2-settings.%3");

        for(auto&path:__listPaths){
            for(auto&fileName:__listFileNames){
                auto fileCheck=__fileFormatSetting.arg(path,fileName,ext);
                QFile file(fileCheck);
                if(file.exists())
                    return fileCheck;
            }
        }
        return QString{};
    };

    auto fileSetting=findFile("json");
    auto fileEnv=findFile("env");

    if(!QFile::exists(fileSetting)){
        aWarning()<<"Application settings not found";
        this->settingFile.clear();
    }
    else{
        this->settingFile
                .setting(fileSetting)
                .envs(fileEnv);
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

    auto findFile=[](QStringList &outPut, const QString &ext){
        auto filter=QStringList{QString("*.%1").arg(ext)};
        auto sufix=QString{"-settings.%1"}.arg(ext);
        auto __listPath=QStringList{":",":/qapr"};
        for(auto&path:__listPath){
            QDir dir(path);
            dir.setNameFilters(filter);
            for(auto &info:dir.entryInfoList()){
                QFile fileSrc(info.filePath());

                if (!fileSrc.fileName().endsWith(sufix))
                    continue;

                if(fileSrc.exists())
                    outPut.append(fileSrc.fileName());
            }
        }
        return !outPut.isEmpty();
    };

    QStringList fileList;

    if(!findFile(fileList, "json"))
        return;

    findFile(fileList, "env");

    for(auto&filePath: fileList){
        auto fileName=filePath.split("/").last();
        QFile fileSrc(filePath);
        QFile fileDst(QStringLiteral("%1/%2").arg(*applicationSettingDir,fileName));
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
