#include "./qapr_scheduler_agent.h"
#include "../application/qapr_macro.h"
#include "../application/qapr_application.h"
#include "../../../qstm/src/qstm_util_variant.h"
#include "./qapr_scheduler_task.h"
#include <QMultiHash>
#include <QMutex>
#include <QTimer>

namespace QApr {

static const auto __scheduler="scheduler";
//static const auto __scope="scope";
//static const auto __enabled="enabled";

Q_GLOBAL_STATIC(SchedulerAgent, staticAgent);

class SchedulerAgentPvt: public QObject{
public:
    SchedulerAgent *agent=nullptr;
    QHash<QUuid, QDateTime> tasksInterval;
    QHash<QUuid, SchedulerTask*> tasks;

    QStm::SettingBase settingScheduler;

    explicit SchedulerAgentPvt(SchedulerAgent *parent) : QObject{parent}, agent{parent}, settingScheduler{parent}
    {
        Q_DECLARE_VU;
        auto &manager=QApr::Application::i().manager();
        auto vSetting=manager.settingBody(__scheduler);
        settingScheduler.fromHash(vSetting);
    }

public slots:

    void start()
    {
        const auto &scopeList=settingScheduler.scope();

        //create
        for(auto &scope:SchedulerScopeGroup::scopes()){
            if(!scope->isScope(scopeList))
                continue;

            auto task=this->tasks.value(scope->uuid());

            if(task && task->isRunning())
                continue;

            if(task){
                task->quit();
                if(task->wait(1000))
                    delete task;
                else
                    task->deleteLater();
                task=nullptr;
            }

            task=SchedulerTask::builder(scope).build();
            this->tasks.insert(scope->uuid(), task);
        }

        {//start
            QHashIterator <QUuid, SchedulerTask*> i(this->tasks);
            while(i.hasNext()){
                i.next();
                if(!i.value()->isRunning())
                    i.value()->start();
            }
        }
    }

    void free()
    {
        //tasks
        auto lst=tasks.values();
        tasks.clear();
        for(auto &task: lst){
            task->quit();
            if(task->wait(1000))
                delete task;
            else
                task->deleteLater();
        }
    }

};

SchedulerAgent::SchedulerAgent(QObject *parent):QThread{nullptr}, PrivateQApr::SchedulerAnnotation{this},p{new SchedulerAgentPvt{this}}
{
    Q_UNUSED(parent)
    this->moveToThread(this);
}

SchedulerAgent &SchedulerAgent::i()
{
    return *staticAgent;
}

void SchedulerAgent::run()
{
    if(!p->settingScheduler.enabled())
        return;
    aInfo()<<QStringLiteral("%1 started").arg(this->metaObject()->className());
    QTimer::singleShot(100,this,[this](){
        p->start();
    });
    this->exec();
    p->free();
    aInfo()<<QStringLiteral("%1 finished").arg(this->metaObject()->className());
 }

bool SchedulerAgent::start()
{
#ifdef QAPR_LOG_VERBOSE
    aWarning()<<QStringLiteral("started");
#endif
    QThread::start();
    while(this->eventDispatcher()==nullptr)
        QThread::msleep(1);
    return true;
}

bool SchedulerAgent::stop()
{
    this->quit();
    return true;
}

bool SchedulerAgent::reg(const QMetaObject &metaObject)
{
    SchedulerScopeGroup::reg(metaObject);
    return true;
}

}
