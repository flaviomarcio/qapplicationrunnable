#include "./qapr_scheduler_task.h"
#include "../application/qapr_macro.h"
#include "../../qstm/src/qstm_util_variant.h"
#include <QTimer>

namespace QApr {

class SchedulerTaskPvt: public QObject{
public:

    SchedulerTask *parent=nullptr;
    QTimer *timer=nullptr;
    QRpc::ServiceSetting settings;
    QVariant name;
    QHash<QByteArray, const QMetaObject *> taskMetaObject;
    QMetaMethod taskMetaMethod;
    QVariantHash connection;
    QVariantHash stats;
    QDateTime lastExec;
    const SchedulerScopeGroup *scope=nullptr;
    explicit SchedulerTaskPvt(SchedulerTask *parent, const SchedulerScopeGroup *scope) : QObject{parent}, scope{scope}
    {
        this->parent=parent;
    }

    void freeTimer()
    {
        if(this->timer==nullptr)
            return;
        QObject::disconnect(timer, &QTimer::timeout, this, &SchedulerTaskPvt::taskRun);
        this->timer->stop();
        this->timer->deleteLater();
        this->timer=nullptr;
    }


    QTimer *newTimer()
    {
        freeTimer();
        auto timer=new QTimer();
        timer->setInterval(this->settings.activityIntervalInitial());
        QObject::connect(timer, &QTimer::timeout, this, &SchedulerTaskPvt::taskRun);
        return timer;
    }

public slots:

    void taskRun()
    {
        aInfo()<<QStringLiteral("Scheduler[%1]: started").arg(this->parent->name());

        this->timer->stop();
        this->timer->setInterval(this->settings.activityInterval());

        if(this->scope==nullptr){
            aWarning()<<QStringLiteral("Scheduler[%1]: invalid scope").arg(this->parent->name());
        }
        else{
            this->lastExec=QDateTime::currentDateTime();
            this->scope->invoke(this);
        }

        emit this->parent->taskUpdate(this->parent);
        if(this->timer->interval()>0)
            this->timer->start();

        auto totalTime=QDateTime::currentDateTime().toMSecsSinceEpoch()-this->lastExec.toMSecsSinceEpoch();
        aInfo()<<QStringLiteral("Scheduler[%1]: finished, total-time[%2 ms]").arg(this->parent->name(), QString::number(totalTime));
    }

};

SchedulerTask::SchedulerTask(QObject *parent):QThread{nullptr}
{
    Q_UNUSED(parent)
    this->p = new SchedulerTaskPvt{this, nullptr};
    this->moveToThread(this);
}

SchedulerTask::SchedulerTask(const SchedulerScopeGroup *scope):QThread{nullptr}
{
    this->p = new SchedulerTaskPvt{this, scope};
    this->moveToThread(this);
}

void SchedulerTask::run()
{
    emit this->taskStarted(this);
#ifdef QAPR_LOG_VERBOSE
    aWarning()<<QStringLiteral("started");
#endif
    p->timer=p->newTimer();
    if(!p->timer || (p->timer->interval()<=0)){
        aDebug()<<QStringLiteral("Scheduler[%1]: interval is 0").arg(this->name());
        aDebug()<<QStringLiteral("Scheduler[%1]: stoped").arg(this->name());
    }
    else{
        p->timer->start();
        aDebug()<<QStringLiteral("Scheduler: running");
        this->exec();
    }
    p->freeTimer();
    emit this->taskFinished(this);
}

QRpc::ServiceSetting &SchedulerTask::settings() const
{
    return p->settings;
}

QUuid SchedulerTask::uuid() const
{
    Q_DECLARE_VU;
    const auto keys=p->taskMetaObject.keys();
    QByteArray bytes;
    for(auto&v:keys)
        bytes.append(v+" ");
    bytes.append('.');
    bytes.append(this->name());
    return vu.toUuid(bytes);
}

QByteArray SchedulerTask::name() const
{
    auto name=p->name.toByteArray().trimmed();
    if(name.isEmpty())
        return this->taskMetaMethod().name();
    return name;
}

SchedulerTask &SchedulerTask::name(const QVariant &newName)
{
    if (p->name == newName)
        return *this;
    p->name = newName;
    return *this;
}

SchedulerTask &SchedulerTask::resetName()
{
    return name({});
}

QVariantHash SchedulerTask::connection() const
{
    return p->connection;
}

void SchedulerTask::connection(const QVariantHash &newConnection)
{
    if (p->connection == newConnection)
        return;
    p->connection = newConnection;
}

void SchedulerTask::resetConnection()
{
    return connection({});
}

SchedulerTask &SchedulerTask::taskMetaObject(const QMetaObject &newTaskMetaObject)
{
    p->taskMetaObject.insert(newTaskMetaObject.className(), &newTaskMetaObject);
    return *this;
}

SchedulerTask &SchedulerTask::resetTaskMetaObject()
{
    return taskMetaObject({});
}

QMetaMethod SchedulerTask::taskMetaMethod() const
{
    return p->taskMetaMethod;
}

SchedulerTask &SchedulerTask::taskMetaMethod(const QMetaMethod &newTaskMetaMethod)
{
    if (p->taskMetaMethod == newTaskMetaMethod)
        return *this;
    p->taskMetaMethod = newTaskMetaMethod;
    return *this;
}

SchedulerTask &SchedulerTask::resetTaskMetaMethod()
{
    return taskMetaMethod({});
}

}
