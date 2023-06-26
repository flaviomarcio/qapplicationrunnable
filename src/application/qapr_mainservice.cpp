#include "./qapr_mainservice.h"
#include <QProcess>

#ifdef QT_GUI_LIB
#include <QGuiApplication>
#else
#include <QCoreApplication>
#endif
#include "./qapr_macro.h"
#include "./qapr_application.h"
#include "../services/servers/qapr_server.h"
#include "../services/scheduler/qapr_scheduler_agent.h"

namespace QApr {

static const auto __comma=",";
static const auto __api="api";
static const auto __ws="ws";
static const auto __scheduler="scheduler";
static const auto __sch="sch";
static const auto __job="job";

Q_GLOBAL_STATIC_WITH_ARGS(QStringList, __valid_args,({__api,__ws,__scheduler,__sch, __job}))

MainService::MainService(QObject *parent):QObject{parent}
{
    QLocale::setDefault(QLocale(QLocale::Portuguese, QLocale::Brazil));
}

int MainService::exec(Q_APR_APP_CLASS &a)
{
    return exec(a, 0, nullptr);
}

int MainService::exec(Q_APR_APP_CLASS &a, int argc, char* argv[])
{
    Q_UNUSED(a)
    Q_UNUSED(argc)
    Q_UNUSED(argv)

    bool __return=false;

    auto &appInstance=Application::i();

    const auto &arguments = appInstance.arguments();

    if(arguments.isEmpty()){
        aWarning()<<"No application arguments";
        return QProcess::NormalExit;
    }

    if(arguments.contains(__api) || arguments.contains(__ws)){
        auto &service = Server::i();
        service.start();
        __return = service.isRunning() || __return;
    }

    if(arguments.contains(__scheduler) || arguments.contains(__sch)){
        auto &service = SchedulerAgent::i();
        service.start();
        __return = service.isRunning() || __return;
    }

    if(arguments.contains(__job)){
        __return = true;
    }

    if(__return)
        return appInstance.exec(a);

    auto args=arguments.keys();
    aWarning()<<QStringLiteral("No valid arguments:[%1], set valid arguments[%2]").arg(args.join(__comma), __valid_args->join(__comma));
    return QProcess::NormalExit;
}

}
