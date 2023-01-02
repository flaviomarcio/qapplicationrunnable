#include "./qapr_mainservice.h"
#include <QProcess>
#include <QDir>

#ifdef QAPR_TEST
#include <gmock/gmock.h>
#include <QGuiApplication>
#else
#include "./qapr_application.h"
#include "../services/qapr_server.h"
#include "../services/qapr_agent.h"
#include "../services/qapr_notify.h"
#endif


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
#ifdef QAPR_TEST
    testing::InitGoogleTest(&argc, argv);
    testing::InitGoogleMock(&argc, argv);
    return RUN_ALL_TESTS();
#else
    bool __return=false;

    auto &appInstance=Application::i();

    const auto &arguments = appInstance.arguments();

    if(arguments.contains(QStringLiteral("ws"))){
        auto &service = Server::instance();
        service.start();
        __return = service.isRunning() || __return;
    }

    if(arguments.contains(QStringLiteral("agent"))){
        auto &service = Agent::instance();
        service.start();
        __return = service.isRunning() || __return;
    }

    if(arguments.contains(QStringLiteral("notify"))){
        auto &service = Notify::instance();
        service.start();
        __return = service.isRunning() || __return;
    }

    if(__return)
        return appInstance.exec(a);

    return QProcess::NormalExit;

#endif

}

}
