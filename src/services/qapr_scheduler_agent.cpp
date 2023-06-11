#include "./qapr_scheduler_agent.h"
#include "../application/qapr_macro.h"
#include "../application/qapr_application.h"
#include "../../../qstm/src/qstm_util_variant.h"
#include "./qapr_scheduler_task.h"
#include <QMultiHash>
#include <QMutex>
#include <QTimer>

namespace QApr {


static const auto __services="services";
static const auto __scheduler="scheduler";
static const auto __default="default";
static const auto __scope="scope";

Q_GLOBAL_STATIC(SchedulerAgent, staticAgent);

class SchedulerAgentPvt: public QObject{
public:
    SchedulerAgent *agent=nullptr;
    QHash<QUuid, QDateTime> tasksInterval;
    QHash<QUuid, SchedulerTask*> tasks;

    explicit SchedulerAgentPvt(SchedulerAgent *parent) : QObject{parent}, agent{parent}
    {
        this->agent=parent;
    }

public slots:

    void start()
    {
        this->free();

        QStm::Envs envs;
        QStringList scopeName;
        {
            Q_DECLARE_VU;
            auto &manager=QApr::Application::i().manager();
            auto vSettingsDefault=manager.settingBody(__default);
            auto vSettingsScheduler=manager.settingBody(__scheduler);
            auto vSettings=vu.vMerge(vSettingsDefault, vSettingsScheduler).toHash();
            scopeName=vSettings.value(__scope).toStringList();
        }

        for(auto&scope:SchedulerScopeGroup::scopes()){
            auto task=SchedulerTask::builder(scope).build();
            this->tasks.insert(task->uuid(), task);
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
        {//tasks
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
    }

};

SchedulerAgent::SchedulerAgent(QObject *parent):QThread{nullptr}, PrivateQApr::SchedulerAnnotation{this}
{
    Q_UNUSED(parent)
    this->p = new SchedulerAgentPvt{this};
    this->moveToThread(this);
}

SchedulerAgent &SchedulerAgent::i()
{
    return *staticAgent;
}

void SchedulerAgent::run()
{
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
