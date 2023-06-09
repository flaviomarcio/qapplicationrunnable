#include "./qapr_application.h"
#include "./qapr_consts.h"
#include "./qapr_startup.h"
#if Q_APR_LOG
#include "./qapr_macro.h"
#endif
#include "../../../qstm/src/qstm_util_variant.h"
#include <QJsonDocument>
#include <QJsonArray>
#include <QCoreApplication>

namespace QApr {

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
    auto &i=*staticInstance;
    const auto &settingFile=i.resourceSettings();

    Q_DECLARE_VU;

    {//settings
        QVariantHash vSettings;
        auto vList=QVariant(settingFile.setting()).toList();
        for(auto &v:vList)
            v=vu.toVariantObject(v);

        for(auto &v:vList)
            vSettings=vu.vMerge(vSettings, v).toHash();

        {//QRpc::SettingManager
            auto &manager=i.manager();
            if(!manager.load(vSettings)){
                aWarning()<<QObject::tr("Connection manager: no loaded for %1").arg(settingFile.setting().join(','));
                return;
            }
        }

        {//application settings
            i.settings().setValues(vSettings);
            if(!i.connectionManager().load(vSettings)){
                aWarning()<<QObject::tr("Connection manager is not loaded for %1").arg(settingFile.setting().join(','));
                return;
            }
        }
    }

    {//QStm::Envs
        QVariantHash vEnvs;
        auto vList=QVariant(settingFile.envs()).toList();
        for(auto &v:vList)
            v=vu.toVariantObject(v);

        for(auto &v:vList)
            vEnvs=vu.vMerge(vEnvs, v).toHash();

        {//QStm::Envs
            auto &envs=i.envs();
            if(!vEnvs.isEmpty()){
                envs
                    .systemEnvs(vEnvs)
                    .customEnvs(vEnvs);
            }
        }
    }

    if(!settingFile.setting().isEmpty())
        aWarning() << QObject::tr("loaded settings: %1").arg(settingFile.setting().join(','));

    if(!settingFile.envs().isEmpty())
        aWarning() << QObject::tr("loaded envs: %1").arg(settingFile.envs().join(','));
}

static void startUp(Application &i)
{
    Q_UNUSED(i)
    startSettings();
    startCircuitBreaker();
}

static void init()
{
    startUp(*staticInstance);
}

Q_APR_STARTUP_FUNCTION(init)

class ApplicationPvt:public QObject
{
public:
    Application *application=nullptr;
    SettingFile settingFile;
    QVariantHash arguments;
    QRpc::SettingManager manager;
    QOrm::ConnectionManager connectionManager;
    Settings settings;
    QApr::CircuitBreaker circuitBreaker;
    QStm::Envs envs;

    explicit ApplicationPvt(Application*parent=nullptr)
        :
        QObject{parent}
    {
        if(applicationSettingDir->isEmpty())
            *applicationSettingDir=QStringLiteral("%1/%2").arg(settings_HOME_DIR, qApp->applicationName().toLower());
        this->application=parent;
    }

    ~ApplicationPvt()
    {
        circuitBreaker.stop();
    }

    const SettingFile &resourceSettings()
    {
        if(!settingFile.setting().isEmpty())
            return this->settingFile;

        auto findFile=[](const QString &ext){
#ifdef QAPR_APP_TESTS
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
            static const auto __listFileNames=QStringList{{__application, qApp->applicationName().toLower(), __qapr__development_file}};
#else
            static const auto __listFileNames=QStringList{{qApp->applicationName().toLower(), __application}};
#endif

            static const auto __root_path=":";
            static const auto __qapr_path=":/qapr";
            static const auto __listPaths=QStringList{{/*qApp->applicationDirPath(), */__root_path, __qapr_path}};
            static const auto __fileFormatSetting=QStringLiteral("%1/%2-settings.%3");

            QStringList __files;

            for(auto &path:__listPaths){
                for(auto &fileName:__listFileNames){
                    auto fileCheck=__fileFormatSetting.arg(path,fileName,ext);
                    if(QFile::exists(fileCheck))
                        __files.append(fileCheck);
                }
            }
            return __files;
        };

        auto fileSetting=findFile("json");
        auto fileEnv=findFile("env");

        if(fileSetting.isEmpty()){
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

//    static void resourceExtract()
//    {
//        QDir dirHome(*applicationSettingDir);
//        if(!dirHome.exists())
//            dirHome.mkpath(*applicationSettingDir);

//        if(!dirHome.exists())
//            return;

//        auto findFile=[](QStringList &outPut, const QString &ext){
//            auto filter=QStringList{QString("*.%1").arg(ext)};
//            auto sufix=QString{"-settings.%1"}.arg(ext);
//            auto __listPath=QStringList{":",":/qapr"};
//            for(auto &path:__listPath){
//                QDir dir(path);
//                dir.setNameFilters(filter);
//                for(auto &info:dir.entryInfoList()){
//                    QFile fileSrc(info.filePath());

//                    if (!fileSrc.fileName().endsWith(sufix))
//                        continue;

//                    if(fileSrc.exists())
//                        outPut.append(fileSrc.fileName());
//                }
//            }
//            return !outPut.isEmpty();
//        };

//        QStringList fileList;

//        if(!findFile(fileList, "json"))
//            return;

//        findFile(fileList, "env");

//        for(auto &filePath: fileList){
//            auto fileName=filePath.split("/").last();
//            QFile fileSrc(filePath);
//            QFile fileDst(QStringLiteral("%1/%2").arg(*applicationSettingDir, fileName));

//            if(fileDst.exists())
//                continue;

//            if(!fileSrc.open(fileSrc.ReadOnly)){
//#if Q_RPC_LOG
//                aWarning()<<QStringLiteral("No open file:")<<fileSrc.fileName()<<QStringLiteral(", error: ")<<fileSrc.errorString();
//#endif
//                continue;
//            }
//            if(!fileDst.open(fileDst.Truncate | fileDst.Unbuffered | fileDst.WriteOnly)){
//#if Q_RPC_LOG
//                aWarning()<<QStringLiteral("No open file:")<<fileDst.fileName()<<QStringLiteral(", error: ")<<fileDst.errorString();
//#endif
//                fileSrc.close();
//                continue;
//            }

//            fileDst.write(fileSrc.readAll());
//            fileDst.flush();
//            fileDst.close();
//            fileSrc.close();
//        }
//    }

    QVariantHash &getArguments()
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

    static Application &i()
    {
        return *staticInstance;
    }

    void circuitBreakerStart()
    {
        if(!circuitBreakerSettings->enabled())
            return;
        this->circuitBreaker.setSettings(circuitBreakerSettings->toHash());
        if(this->circuitBreaker.start())
            this->circuitBreaker.print();
    }
};

Application::Application(QObject *parent) : QObject{parent}
{
    this->p=new ApplicationPvt{this};
}

const SettingFile &Application::resourceSettings()
{
    return p->resourceSettings();
}

QRpc::SettingManager &Application::manager()
{
    return p->manager;
}

QOrm::ConnectionManager &Application::connectionManager()
{
    return p->connectionManager;
}

QOrm::ConnectionPool &Application::pool()
{
    return p->connectionManager.pool();
}

QStm::Envs &Application::envs()const
{
    return p->envs;
}

int Application::exec(QCoreApplication &a)
{
    p->circuitBreakerStart();
    return a.exec();
}

Application &Application::i()
{
    return ApplicationPvt::i();
}

const QUuid &Application::instanceUuid()
{
    static const auto appInstanceUuid=QUuid::createUuidV5(QUuid::createUuid(),QUuid::createUuid().toString());
    return appInstanceUuid;
}

QVariantHash &Application::arguments()const
{
    return p->getArguments();
}

Application &Application::printArguments()
{
#if Q_APR_LOG
    QHashIterator<QString, QVariant> i(p->getArguments());
    while (i.hasNext()) {
        i.next();
        aInfo()<<QStringLiteral("%1 : %2").arg(i.key(), i.value().toString());
    }
#endif
    return *this;
}

CircuitBreaker &Application::circuitBreaker()
{
    return p->circuitBreaker;
}

Settings &Application::settings()
{
    return p->settings;
}

}
