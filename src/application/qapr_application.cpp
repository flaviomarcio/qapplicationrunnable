#include "./qapr_application.h"
#include "./qapr_startup.h"
#include "./private/p_qapr_application.h"
#include <QProcess>
#include <QJsonDocument>
#include <QJsonArray>

namespace QApr {


Q_GLOBAL_STATIC(Application, staticInstance);
Q_GLOBAL_STATIC(QRpc::ServiceSetting, circuitBreakerSettings)

static void startCircuitBreaker()
{
    auto &manager=staticInstance->manager();
    *circuitBreakerSettings=manager.setting(qsl("circuit-breaker"));
}

static void startSettings()
{
    auto&i=*staticInstance;
    auto settingFiles=i.resourceSettings().toStringList();
    auto &manager=i.manager();
    for(auto&settingFile: settingFiles){
        if(manager.load(settingFile)){
            i.settings().setValues(settingFile);
            break;
        }
    }
    auto &cnn=i.connectionManager();
    if(!cnn.isLoaded())
        sWarning()<<qtr("Connection manager is not loaded for %1").arg(settingFiles.join(','));

}

static void startUp(Application &i)
{   
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

Application::Application(QObject *parent) : QObject{parent}
{
    this->p=new ApplicationPvt{this};
}

QVariant Application::resourceSettings()const
{
    return p->resourceSettings();
}

QRpc::ServiceManager &Application::manager()
{
    return p->manager;
}

int Application::exec(QCoreApplication &a)
{
    p->circuitBreaker.setSettings(circuitBreakerSettings->toHash());
    if(p->circuitBreaker.start())
        p->circuitBreaker.print();
    return a.exec();
}

Application &Application::instance()
{
    return*staticInstance;
}

Application &Application::i()
{
    return Application::instance();
}

qlonglong Application::memoryUsage()
{
    QProcess process;
    process.start(qsl("cat"), qvsl{qsl("/proc/%1/status").arg(qApp->applicationPid())});

    if(!process.waitForStarted(1000)){
        return 0;
    }

    if(!process.waitForFinished(1000)){
        process.terminate();
        return 0;
    }

    QString bytes=process.readAllStandardOutput().toLower();
    bytes=bytes.replace(qsl("\t"), qsl_null);
    while(bytes.contains(qsl("  ")))
        bytes=bytes.replace(qsl("  "), qsl_space);

    auto vList=bytes.split(qsl("\n"));
    for(auto &s:vList){
        if(!s.contains(qsl_fy(vmpeak)))
            continue;

        auto vmRSS=s.split(qsl(":")).last().trimmed().replace(qsl("kb"),qsl_null).trimmed();
        return vmRSS.toLongLong();
    }
    return 0;
}

const QUuid &Application::instanceUuid()
{
    static const auto appInstanceUuid=QUuid::createUuidV5(QUuid::createUuid(),QUuid::createUuid().toString());
    return appInstanceUuid;
}

QVariantHash &Application::arguments()const
{
    return p->arguments();
}

Application&Application::printArguments()
{
#if QAPR_LOG
    QHashIterator<QString, QVariant> i(p->_arguments);
    while (i.hasNext()) {
        i.next();
        sInfo()<<qsl("%1 : %2").arg(i.key(), i.value().toString());
    }
#endif
    return*this;
}

Application &Application::resourceExtract()
{
    p->resourceExtract();
    return*this;
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
