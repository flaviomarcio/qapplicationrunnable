#include "./qapr_agent_base.h"
#include "../application/qapr_application.h"
#include "../application/qapr_consts.h"
#if Q_APR_LOG
#include "../application/qapr_macro.h"
#include <QSqlError>
#endif
#include <QTimer>
#include <QCryptographicHash>
#include <QVariant>
#include <QMutex>
#include <QMetaProperty>

namespace QApr{

class AgentBasePvt:public QObject{
public:
    QMutex serviceStartLock;
    QDateTime runner_date;
    QVariantHash stats;
    AgentBase*parent = nullptr;
    explicit AgentBasePvt(AgentBase*parent=nullptr):QObject{parent}
    {
        this->parent=parent;
    }

public slots:
    void onServiceRun()
    {
#if Q_APR_LOG_VERBOSE
        aWarning()<<this->parent->agentName()<<tr(": started");
#endif
        QMutexLocker<QMutex> locker(&serviceStartLock);
        auto &pp=*this->parent;
#if Q_APR_LOG_SUPER_VERBOSE
        sInfo()<<"run "<<pp.agentName();
#endif
        this->runner_date=QDateTime::currentDateTime();
        auto &cnMng=Application::i().connectionManager();
        auto manager=QOrm::ConnectionManager(cnMng, this);
        auto &pool=manager.pool();
        const auto &agentSetting=pp.agentSetting();
        Q_UNUSED(agentSetting)
        QSqlDatabase db;
#if Q_APR_LOG_VERBOSE
        sInfo()<<this->agentName()<<", db connecting";
#endif
        if(!pool.get(db)){
#if Q_APR_LOG
            sInfo()<<QStringLiteral("%1, no connection db: %2").arg(pp.agentName(), pool.lastError().text());
#endif
            return;
        }

        if(pp.setConnection(db)){
#if Q_APR_LOG_VERBOSE
            sInfo()<<this->agentName()<<", "<<agentSetting.name();
#endif
            this->service_start();
            pool.finish(db);
            return;
        }
#if Q_APR_LOG_VERBOSE
        aWarning()<<this->parent->agentName()<<tr(": finished");
#endif
    }

    void service_start()
    {
#if Q_APR_LOG_VERBOSE
        aDebugMethodStart();
#endif
        auto &pp=*this->parent;
        auto agentName=pp.agentName();
        auto metaObject=pp.metaObject();
        auto methodName=agentName;
        auto message=tr("Method not found");
        for(int methodIndex = 0; methodIndex < metaObject->methodCount(); ++methodIndex) {
            auto method = metaObject->method(methodIndex);
            if(method.parameterCount()>0)
                continue;

            if(method.name().toLower() != methodName)
                continue;

            if(!pp.canMethodExecute(method))
                continue;
#if Q_APR_LOG_VERBOSE
            aWarning()<<QStringLiteral("invoke method(%1)").arg(QString(method.name()));
#endif
            if(!method.invoke(&pp, Qt::DirectConnection)){
                message=tr("Method not called");
#if Q_APR_LOG
                aWarning()<<QStringLiteral("invoke method(%1): error==%2").arg(method.name(), message);
#endif
                continue;
            }
#if Q_APR_LOG_VERBOSE
            oDebug()<<"invoke method:"<<method.name();
#endif
            message.clear();
            break;
        }
#if Q_APR_LOG_VERBOSE
        aDebugMethodFinish();
#endif
        this->runner_date=this->makeNewDateRun();
    }

    QDateTime makeNewDateRun()
    {
        auto &agentSetting=this->parent->agentSetting();
        auto interval = agentSetting.activityInterval();
        auto next=QDateTime::currentDateTime();
        next=next.addMSecs(interval);

        return next;
    }

    bool runCheck()
    {
        auto &p=*this;
        auto service=this->parent->agentName();
        auto &agentSetting=this->parent->agentSetting();
        if(!agentSetting.enabled()){
#if Q_APR_LOG_VERBOSE
            sInfo()<<service<<QStringLiteral(" disabled");
#endif
            return false;
        }

        auto _run=[this, &p, &service](){
            Q_UNUSED(service)
            this->runner_date=this->makeNewDateRun();
            if(!p.serviceStartLock.tryLock(10)){
#if Q_APR_LOG_VERBOSE
                sInfo()<<service<<QStringLiteral(" skipped");
#endif
                return false;
            }

            emit this->parent->serviceStart();
            p.serviceStartLock.unlock();
            return true;
        };

        auto now=QDateTime::currentDateTime();
        if(p.runner_date.isNull())
            return _run();

        if(now>p.runner_date)
            return _run();

        return false;
    }
};


AgentBase::AgentBase(QObject *parent):QThread{nullptr}
{
    Q_UNUSED(parent)
    this->p = new AgentBasePvt{this};
    this->moveToThread(this);
    QObject::connect(this, &AgentBase::serviceStart, p, &AgentBasePvt::onServiceRun);
}


QRpc::ServiceSetting &AgentBase::agentSetting()
{
    auto &manager=Application::i().manager();
    auto agentName=this->agentName();
    auto &setting=manager.setting(agentName);
    if(!setting.enabled()){
        static QRpc::ServiceSetting __default;
        return __default;
    }

    return setting;
}

void AgentBase::run()
{
#if Q_APR_LOG_VERBOSE
    sInfo()<<tr("started");
#endif
    this->exec();
}

bool AgentBase::canMethodExecute(const QMetaMethod &method)
{
    Q_UNUSED(method)
    return true;
}

bool AgentBase::runCheck()
{
    return p->runCheck();
}

void AgentBase::start()
{
    if(!this->agentSetting().enabled())
        return;

    if(this->isRunning())
        return;

    QThread::start();
    while(this->eventDispatcher()==nullptr)
        QThread::msleep(1);
}

QByteArray AgentBase::agentName() const
{
    auto v=this->property(QByteArrayLiteral("agentName")).toByteArray();
    return v;
}

void AgentBase::setAgentName(const QVariant &v)
{
    this->setProperty(QByteArrayLiteral("agentName"),v);
    auto c_name=QString(this->metaObject()->className());
    auto a_name=QString(this->agentName());
    auto objectName=QStringLiteral("%1::%2").arg(c_name, a_name);
    objectName=objectName.replace(QStringLiteral(":"),QStringLiteral("_")).replace(QStringLiteral("__"),QStringLiteral("_"));
    this->setObjectName(objectName);
}

QVariantHash &AgentBase::stats()
{
    return p->stats;
}

const QVariant AgentBase::resourceSettings()
{
    return QApr::Application::i().resourceSettings();
}



}
