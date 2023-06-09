#include "./qapr_mainservice.h"
#include <QProcess>
#include <QDir>

#ifdef QT_GUI_LIB
#include <QGuiApplication>
#else
#include <QCoreApplication>
#endif
#include "./qapr_application.h"
#include "../services/qapr_server.h"
#include "../services/qapr_scheduler_agent.h"

namespace QApr {

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

    if(arguments.contains(QStringLiteral("api")) || arguments.contains(QStringLiteral("ws"))){
        auto &service = Server::i();
        service.start();
        __return = service.isRunning() || __return;
    }

    if(arguments.contains(QStringLiteral("job")) || arguments.contains(QStringLiteral("agent"))){
        auto &service = SchedulerAgent::i();
        service.start();
        __return = service.isRunning() || __return;
    }

    if(__return)
        return appInstance.exec(a);

    return QProcess::NormalExit;
}

}
