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

static void initApp(Application&i)
{
#ifdef QT_DEBUG
    i.resourceExtract();
#endif
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

const QUuid &Application::instanceUuid()
{
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

}
