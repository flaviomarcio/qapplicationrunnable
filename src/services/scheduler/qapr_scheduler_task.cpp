#include "./qapr_scheduler_task.h"
#include "../application/qapr_macro.h"
#include "../../qstm/src/qstm_setting_base.h"
#include <QTimer>

namespace QApr {

class SchedulerTaskPvt: public QObject{
public:

    SchedulerTask *parent=nullptr;
    QTimer *timer=nullptr;
    QStm::SettingBase settings;
    QVariantHash stats;
    QDateTime lastExec;
    SchedulerScopeGroup *scope=nullptr;
    explicit SchedulerTaskPvt(SchedulerTask *parent, SchedulerScopeGroup *scope)
        :
        QObject{parent},
        parent{parent},
        scope{scope}
    {

    }

    void timerStop()
    {
        if(this->timer==nullptr)
            return;
        QObject::disconnect(timer, &QTimer::timeout, this, &SchedulerTaskPvt::taskRun);
        this->timer->stop();
        this->timer->deleteLater();
        this->timer=nullptr;
    }


    void timerStart()
    {
        timerStop();
        if(timer==nullptr)
            timer=new QTimer();
        auto interval=this->settings.activityInterval();
        if(interval>0){
            timer->setInterval(interval);
            QObject::connect(timer, &QTimer::timeout, this, &SchedulerTaskPvt::taskRun);
        }
    }

public slots:

    void taskRun()
    {
        this->timerStop();
        aInfo()<<QStringLiteral("Scheduler[%1]: started").arg(scope->scope());

        if(scope==nullptr){
            aWarning()<<QStringLiteral("Scheduler[%1]: invalid scope").arg(this->scope->scope());
        }
        else{
            this->lastExec=QDateTime::currentDateTime();
            scope->invoke();
        }
        auto totalTime=QDateTime::currentDateTime().toMSecsSinceEpoch()-this->lastExec.toMSecsSinceEpoch();
        this->timerStart();
        aInfo()<<QStringLiteral("Scheduler[%1]: finished, total-time[%2 ms]").arg(this->scope->scope(), QString::number(totalTime));
    }

};

SchedulerTask::SchedulerTask(QObject *parent):QThread{nullptr}
{
    Q_UNUSED(parent)
    this->p = new SchedulerTaskPvt{this, nullptr};
    this->moveToThread(this);
}

SchedulerTask::SchedulerTask(SchedulerScopeGroup *scope):QThread{nullptr}
{
    this->p = new SchedulerTaskPvt{this, scope};
    this->moveToThread(this);
}

void SchedulerTask::run()
{
    this->setObjectName(QString("SchTsk_%1").arg(p->scope->scope()));
#ifdef QAPR_LOG_VERBOSE
    aWarning()<<QStringLiteral("started");
#endif
    auto interval=p->settings.activityInterval();
    if(interval<=0){
        aDebug()<<QStringLiteral("Scope:[%1-%2]: interval: %3, state: stoped").arg(p->scope->scope(), p->scope->group()).arg(interval);
    }
    else{
        auto intervalInitial=p->settings.activityIntervalInitial();
        intervalInitial=(intervalInitial>0)?intervalInitial:100;
        QTimer::singleShot(intervalInitial, this, [this](){
            p->taskRun();
        });
        aDebug()<<QStringLiteral("Scope:[%1,%2]: initialStart: %3, state: running ").arg(p->scope->scope(), p->scope->group()).arg(intervalInitial);
        this->exec();
    }
    p->timerStop();
}

const QUuid &SchedulerTask::uuid() const
{
    return p->scope->uuid();
}

}
