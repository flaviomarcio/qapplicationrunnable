#include "./qapr_application.h"
#include "./private/p_qapr_application.h"
#include "./qapr_consts.h"
#if Q_APR_LOG
#include "./qapr_macro.h"
#endif
#include <QJsonDocument>
#include <QJsonArray>

namespace QApr {

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

Application &Application::resourceExtract()
{
    p->resourceExtract();
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
