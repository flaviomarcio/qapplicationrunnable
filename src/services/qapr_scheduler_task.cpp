#include "./qapr_scheduler_task.h"
#include "../application/qapr_macro.h"
#include "../../qrpc/src/qrpc_service_setting.h"
#include <QTimer>

namespace QApr {

class SchedulerTaskPvt: public QObject{
public:

    SchedulerTask *parent=nullptr;
    QTimer *timer=nullptr;
    QRpc::ServiceSetting settings;
    QVariantHash stats;
    QDateTime lastExec;
    const SchedulerScopeGroup *scope=nullptr;
    explicit SchedulerTaskPvt(SchedulerTask *parent, const SchedulerScopeGroup *scope)
        :
        QObject{parent},
        parent{parent},
        scope{scope}
    {

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
        aInfo()<<QStringLiteral("Scheduler[%1]: started").arg(this->scope->scopeName());

        this->timer->stop();
        this->timer->setInterval(this->settings.activityInterval());

        if(this->scope==nullptr){
            aWarning()<<QStringLiteral("Scheduler[%1]: invalid scope").arg(this->scope->scopeName());
        }
        else{
            this->lastExec=QDateTime::currentDateTime();
            this->scope->invoke(this);
        }

        emit this->parent->taskUpdate(this->parent);
        if(this->timer->interval()>0)
            this->timer->start();

        auto totalTime=QDateTime::currentDateTime().toMSecsSinceEpoch()-this->lastExec.toMSecsSinceEpoch();
        aInfo()<<QStringLiteral("Scheduler[%1]: finished, total-time[%2 ms]").arg(this->scope->scopeName(), QString::number(totalTime));
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
    this->setObjectName(QString("SchTsk_%1").arg(p->scope->scopeName()));
    emit this->taskStarted(this);
#ifdef QAPR_LOG_VERBOSE
    aWarning()<<QStringLiteral("started");
#endif
    p->timer=p->newTimer();
    if(!p->timer || (p->timer->interval()<=0)){
        aDebug()<<QStringLiteral("Scheduler[%1]: interval is 0").arg(p->scope->scopeName());
        aDebug()<<QStringLiteral("Scheduler[%1]: stoped").arg(p->scope->scopeName());
    }
    else{
        p->timer->start();
        aDebug()<<QStringLiteral("Scheduler[%1]: running").arg(p->scope->scopeName());
        this->exec();
    }
    p->freeTimer();
    emit this->taskFinished(this);
}

QUuid &SchedulerTask::uuid() const
{
    return p->scope->uuid();
}

}
