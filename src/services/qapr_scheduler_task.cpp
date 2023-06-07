#include "./qapr_scheduler_task.h"
#include "./qapr_scheduler.h"
#include "../application/qapr_macro.h"
#include "../application/qapr_application.h"
#include "../../qstm/src/qstm_util_variant.h"
#include <QTimer>
#include <QSqlError>


namespace QApr {

class SchedulerTaskPvt: public QObject{
public:

    SchedulerTask *parent=nullptr;
    QTimer *timer=nullptr;
    QRpc::ServiceSetting settings;
    QVariant name;
    QMetaObject taskMetaObject;
    QMetaMethod taskMetaMethod;
    QVariantHash connection;
    QVariantHash stats;

    explicit SchedulerTaskPvt(SchedulerTask *parent) : QObject{parent}
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
        QScopedPointer<QObject> sObj(this->taskMetaObject.newInstance(Q_ARG(QObject*, this )));

        if(sObj.data()==nullptr){
            aWarning()<<tr("%1, Invalid Scheduler metaObject: [%1], method: [%2]").arg(this->taskMetaObject.className(), this->taskMetaMethod.name());
            return;
        }
        
        auto taskObject=dynamic_cast<Scheduler*>(sObj.data());

        if(taskObject==nullptr){
            aWarning()<<tr("%1, Invalid Scheduler object: [%1], method: [%2]").arg(this->taskMetaObject.className(), this->taskMetaMethod.name());
            return;
        }

        this->timer->stop();
        this->timer->setInterval(this->settings.activityInterval());

        QOrm::ConnectionPool pool(Application::i().connectionManager().pool());
        if(!settings.connection().isEmpty())
            pool.from(settings.connection());

        QSqlDatabase db;

        if(!pool.get(db))
            aWarning()<<tr("%1, no connection db: %2").arg(this->parent->name(), pool.lastError().text());


        taskObject->setConnection(db);

        if(!taskMetaMethod.invoke(taskObject, Qt::DirectConnection))
            aWarning()<<QStringLiteral("invoke method(%1): error==%2").arg(taskMetaMethod.name());

        pool.finish();

        emit this->parent->taskUpdate(this->parent);
        if(this->timer->interval()>0)
            this->timer->start();
    }

};

SchedulerTask::SchedulerTask(QObject *parent):QThread{nullptr}
{
    Q_UNUSED(parent)
    this->p = new SchedulerTaskPvt{this};
    this->moveToThread(this);
}

void SchedulerTask::run()
{
    emit this->taskStarted(this);
#ifdef QAPR_LOG_VERBOSE
    aWarning()<<QStringLiteral("started");
#endif
    if(!p->timer || (p->timer->interval()<=0)){
        aDebug()<<QStringLiteral("Scheduler[%1]: interval is 0").arg(this->name());
        aDebug()<<QStringLiteral("Scheduler[%1]: stoped").arg(this->name());
    }
    else{
        p->timer=p->newTimer();
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
    static const auto __format=QString("%1.%2");
    Q_DECLARE_VU;
    auto bytes=__format.arg(this->taskMetaObject().className(),this->name());
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

QMetaObject &SchedulerTask::taskMetaObject() const
{
    return p->taskMetaObject;
}

SchedulerTask &SchedulerTask::taskMetaObject(const QMetaObject &newTaskMetaObject)
{
    p->taskMetaObject = newTaskMetaObject;
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
