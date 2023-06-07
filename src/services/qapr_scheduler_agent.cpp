#include "./qapr_scheduler_agent.h"
#include "../application/qapr_macro.h"
#include "./qapr_scheduler.h"
#include "./qapr_scheduler_task.h"
#include <QMultiHash>
#include <QMutex>
#include <QTimer>

namespace QApr {

Q_GLOBAL_STATIC(SchedulerAgent, staticAgent);

Q_GLOBAL_STATIC_WITH_ARGS(QList<QByteArray>, methodBlackList,(QAPR_METHOD_BACK_LIST));

static void init()
{
    auto metaObject=QStm::Object::staticMetaObject;
    for (int index = 0; index < metaObject.superClass()->methodCount(); ++index){
        auto method=metaObject.method(index);
        methodBlackList->append(method.name());
    }
}

Q_COREAPP_STARTUP_FUNCTION(init)

class SchedulerAgentPvt: public QObject{
public:
    SchedulerAgent *agent=nullptr;
    QHash<QByteArray, const QMetaObject*> services;
    QHash<QByteArray, Schedule*> schedulers;
    QHash<QByteArray, QDateTime> tasksInterval;
    QHash<QUuid, SchedulerTask*> tasks;

    explicit SchedulerAgentPvt(SchedulerAgent *parent) : QObject{parent}, agent{parent}
    {
        this->agent=parent;
    }

public slots:

    void start()
    {
        auto lst=tasks.values();
        for(auto&task: lst)
            task->start();
    }

    void free()
    {
        auto lst=tasks.values();
        tasks.clear();
        for(auto&task: lst){
            task->quit();
            if(task->wait(1000))
                delete task;
            else
                task->deleteLater();
        }
    }

    bool serviceRegister(const QMetaObject &metaObject)
    {
        if (!metaObject.inherits(&Scheduler::staticMetaObject))
            return false;

        QScopedPointer<QObject> sObj(metaObject.newInstance(Q_ARG(QObject*, this )));
        
        auto scheduler=dynamic_cast<Scheduler*>(sObj.data());

        if(scheduler==nullptr)
            return false;

        const auto &annotations = scheduler->annotation();

        if(!annotations.contains(agent->scObject()))
            return false;

        for (int index = 0; index < metaObject.methodCount(); ++index) {
            auto method=metaObject.method(index);

            if(method.methodType()!=QMetaMethod::Method || method.returnType()!=QMetaType::Void)
                continue;

            auto methodName=method.name().toLower();

            if(methodBlackList->contains(methodName))
                continue;

            const auto &annotations = scheduler->annotation(method);

            if(annotations.isEmpty())
                continue;

            auto scEnabled = annotations.find(agent->scEnabled()).value();
            if (scEnabled.isValid() && !scEnabled.toBool())
                continue;


            scEnabled=true;
            auto scIntervalLimit = annotations.find(agent->scIntervalLimit()).value();
            auto scIntervalInitial = annotations.find(agent->scIntervalInitial()).value();
            auto scInterval = annotations.find(agent->scInterval()).value();

            static const auto t10m="10m";
            static const auto t1m="1m";
            static const auto t100ms="100";

            scInterval=(scInterval.toInt()>0)?scInterval:t1m;
            scIntervalInitial=(scIntervalInitial.toInt()>0)?scIntervalInitial:t100ms;
            scIntervalLimit=(scIntervalLimit.toInt()>0)?scIntervalLimit:t10m;

            auto task=SchedulerTask::builder(this)
                            .name(method.name())
                            .taskMetaMethod(method)
                            .taskMetaObject(metaObject)
                            .build();

            if(task->uuid().isNull()){
                delete task;
                continue;
            }

            {//free old object
                auto oldTask=this->tasks.value(task->uuid());
                if(oldTask){
                    oldTask->quit();
                    oldTask->wait(1000);
                    oldTask->deleteLater();
                }
            }

            task->settings().clear();
            task->settings().setActivityInterval(scInterval);
            task->settings().setActivityIntervalInitial(scIntervalInitial);
            task->settings().setActivityLimit(scIntervalLimit);
            task->settings().setEnabled(scEnabled.toBool());

            this->tasks.insert(task->uuid(), task);
        }
        return !services.isEmpty();
    }
private slots:

};

SchedulerAgent::SchedulerAgent(QObject *parent):QThread{nullptr}, PrivateQApr::SchedulerAnnotation{this}
{
    Q_UNUSED(parent)
    this->p = new SchedulerAgentPvt{this};
    this->moveToThread(this);
}

SchedulerAgent &SchedulerAgent::instance()
{
    return *staticAgent;
}

void SchedulerAgent::run()
{
    aDebug()<<QStringLiteral("Scheduler running");
    p->start();
    this->exec();
    p->free();
    aDebug()<<QStringLiteral("Scheduler finished");
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

bool SchedulerAgent::serviceRegister(const QMetaObject&metaObject)
{
    return p->serviceRegister(metaObject);
}

}
