#include "./qapr_application.h"
#include "./private/p_qapr_application.h"
#include <QProcess>
#include <QJsonDocument>
#include <QJsonArray>

namespace QApr {

struct PSInfo{
public:
    QVariant USER,PID,CPU_percent, MEM_percent, VSZ, RSS , TTY , STAT, START;
};

Q_GLOBAL_STATIC(Application, staticInstance);
Q_GLOBAL_STATIC(QMutex, ____mutex);

struct ConstsApplicationBase{
    QRpc::ServiceSetting circuit_breaker;
    void init()
    {
        auto &manager=staticInstance->manager();
        circuit_breaker=manager.setting(qsl("circuit-breaker"));
        if(!circuit_breaker.isValid())
            circuit_breaker=manager.setting(qsl("circuit_breaker"));
    }
};

Q_GLOBAL_STATIC(ConstsApplicationBase, constsApplicationBase)

static void initApp(Application&i)
{
#ifdef QT_DEBUG
    i.resourceExtract();
#endif
    auto settingFile=i.settings_SERVER();
    auto &manager=i.manager();
    manager.load(settingFile);
    constsApplicationBase->init();
    auto &cnn=i.connectionManager();
    if(!cnn.isLoaded())
        sWarning()<<qtr("Connection manager is not loaded");
}

static bool initCheck=false;
static void init()
{
    if(initCheck)//em caso de chamada direta do instance ele vai controlar o acesso
        return;

    QMutexLOCKER locker(____mutex);//em caso de chamada direta do instance ele vai controlar o acesso

    if(initCheck)//em caso de chamada direta do instance ele vai controlar o acesso
        return;

    initApp(*staticInstance);
    initCheck=true;
}

Q_COREAPP_STARTUP_FUNCTION(init)

Application::Application(QObject *parent) : QObject{parent}
{
    this->p=new ApplicationPvt{this};
}

Application::~Application()
{
    dPvt();
    delete&p;
}

QVariant Application::settings_SERVER()const
{
    dPvt();
    auto vList=p.settings_SERVER();
    return vList;
}

QRpc::ServiceManager &Application::manager()
{
    dPvt();
    return p.manager;
}

int Application::exec(QCoreApplication&a)
{
    dPvt();
    p.circuitBreaker.setSettings(constsApplicationBase->circuit_breaker.toHash());
    if(p.circuitBreaker.start())
        p.circuitBreaker.print();
    return a.exec();
}

Application &Application::instance()
{
    if(!initCheck)//em caso de chamada direta do instance ele vai controlar o acesso
        init();
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
    dPvt();
    return p.arguments();
}

Application&Application::printArguments()
{
#if QAPR_LOG
    dPvt();
    QHashIterator<QString, QVariant> i(p.__arguments);
    while (i.hasNext()) {
        i.next();
        sInfo()<<qsl("%1 : %2").arg(i.key(), i.value().toString());
    }
#endif
    return*this;
}

Application &Application::resourceExtract()
{
    dPvt();
    p.resourceExtract();
    return*this;
}

QApr::CircuitBreaker &Application::circuitBreaker()
{
    dPvt();
    return p.circuitBreaker;
}

}
