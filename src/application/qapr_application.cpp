#include "./qapr_application.h"
#include "./qapr_consts.h"
#include "./qapr_startup.h"
#if Q_APR_LOG
#include "./qapr_macro.h"
#endif
#include "../../../qstm/src/qstm_util_variant.h"
#include "../../../qrpc/src/qrpc_request.h"
#include <QJsonDocument>
#include <QJsonArray>
#include <QCoreApplication>

namespace QApr {

QMutex *makeLockedMutex()
{
    auto mutex=new QMutex();
    mutex->lock();
    return mutex;
}

static QMutex *lockedMutex=makeLockedMutex();

Q_GLOBAL_STATIC(Application, staticInstance);
Q_GLOBAL_STATIC(QVariantHash, circuitBreakerSettings);

static const auto __circuit_breaker="circuit-breaker";
static const auto __environments="environments";
static const auto __connection="connection";
static const auto __enabled="enabled";

static void startCircuitBreaker()
{
    auto &manager=staticInstance->manager();
    *circuitBreakerSettings=manager.settingBody(__circuit_breaker);
}

static QVariant envsRemote()
{
    auto &manager=staticInstance->manager();
    auto settings=manager.setting(__environments);
    if(!settings || !settings->enabled())
        return {};

    if(!qAprApp.settings().name().isEmpty())
        qApp->setApplicationName(qAprApp.settings().name());

    auto settingName=settings->name().toLower().trimmed();
    if(settingName.isEmpty())
        return {};

    QRpc::Request rq;
    auto&response=rq
                         .settings(settings)
                         .GET()
                         .call("/settings/setting",QVariantHash{{"name", settingName}});
    if(!response){
        rq.print();
        qFatal("envs not loaded");
    }

    aWarning() << QObject::tr("loaded remove settings: %1/api/settings/setting?name=%2").arg(rq.url(),settingName);

    return response.bodyVariant();

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
            auto vConnection=vSettings.value(__connection).toHash();
            if(!i.connectionManager().load(vConnection)){
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
                    .reset()
                    .systemEnvs(vEnvs)
                    .customEnvs(vEnvs);
            }

            envs
                .customEnvs(envsRemote());
        }
    }

    if(!settingFile.setting().isEmpty())
        aWarning() << QObject::tr("loaded settings: %1").arg(settingFile.setting().join(','));

    if(!settingFile.envs().isEmpty())
        aWarning() << QObject::tr("loaded envs: %1").arg(settingFile.envs().join(','));

    lockedMutex->unlock();
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
    Application *parent=nullptr;
    QVariantHash arguments;
    QStm::SettingFile settingFile;
    Settings settings;
    QStm::SettingManager manager;
    QOrm::ConnectionManager connectionManager;
    QApr::CircuitBreaker circuitBreaker;
    QStm::Envs envs;

    explicit ApplicationPvt(Application *parent=nullptr)
        :
        QObject{parent},
        parent{parent},
        settingFile{parent},
        settings{parent},
        manager{parent},
        connectionManager{parent},
        circuitBreaker{*circuitBreakerSettings, parent}
    {

    }

    ~ApplicationPvt()
    {
        circuitBreaker.stop();
    }

    const QStm::SettingFile &resourceSettings()
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
        fileEnv.append(settings.envFile());

        if(fileSetting.isEmpty()){
            aWarning()<<"Application settings not found";
            this->settingFile.clear();
        }
        else{
            this->settingFile
                .setting(fileSetting)
                .envsAdd(fileEnv);
        }

        return this->settingFile;
    }

    QVariantHash &getArguments()
    {
        if(!arguments.isEmpty())
            return arguments;

        auto args=qApp->arguments();
        if(args.size()>1){
            args.remove(0);//file path
            for(auto &v:args){
                auto l=v.split(QStringLiteral("="));
                if(l.isEmpty())
                    continue;

                if(l.size()==1){
                    auto key=l.first();
                    auto value=l.last();
                    arguments.insert(key, value);
                    continue;
                }
                else if(l.count()==2){
                    auto key=l.first().toLower();
                    auto value=l.last();
                    arguments.insert(key, value);
                }
                else{
                    auto key=l.takeFirst().toLower();
                    auto value=l.join('=');
                    arguments.insert(key, value);
                }
            }
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
        if(!circuitBreakerSettings->value(__enabled).toBool())
            return;
        if(this->circuitBreaker.start())
            this->circuitBreaker.print();
    }
};

Application::Application(QObject *parent) : QObject{parent}, p{new ApplicationPvt{this}}
{

}

bool Application::lockedWait()
{
    QMutexLocker locker(lockedMutex);
    return true;
}

const QStm::SettingFile &Application::resourceSettings()
{
    return p->resourceSettings();
}

QStm::SettingManager &Application::manager()
{
    return p->manager;
}

QOrm::ConnectionManager &Application::connectionManager()
{
    return p->connectionManager;
}

//QOrm::ConnectionPool &Application::pool()
//{
//    return p->connectionManager.pool();
//}

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
