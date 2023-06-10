#include "./qapr_scheduler_agent.h"
#include "../application/qapr_macro.h"
#include "../application/qapr_application.h"
#include "../../../qstm/src/qstm_util_variant.h"
#include "./qapr_scheduler.h"
#include "./qapr_scheduler_task.h"
#include <QMultiHash>
#include <QMutex>
#include <QTimer>

namespace QApr {


static const auto __services="services";
static const auto __default="default";

Q_GLOBAL_STATIC(SchedulerAgent, staticAgent);

class SchedulerAgentPvt: public QObject{
public:
    SchedulerAgent *agent=nullptr;
    QHash<QByteArray, SchedulerScopeGroup*> scope;
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
        this->free();

//        QHashIterator <int, const QMetaObject*> i(this->scope);
//        while(i.hasNext()){
//            i.next();
//            const auto metaObject=i.value();
//            auto method=metaObject->method(i.key());
//            scheduleCreate(method, *metaObject);
//        }

//        {//start
//            QHashIterator <QUuid, SchedulerTask*> i(this->tasks);
//            while(i.hasNext()){
//                i.next();
//                if(!i.value()->isRunning())
//                    i.value()->start();
//            }
//        }
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
        {//scope
            auto lst=scope.values();
            scope.clear();
            qDeleteAll(lst);
        }
    }

    bool scheduleCreate(const QMetaMethod &method, const QMetaObject &metaObject)
    {
        QScopedPointer<QObject> sObj(metaObject.newInstance(Q_ARG(QObject*, this )));
        if(!sObj.data())
            return false;
        auto scheduler=dynamic_cast<Scheduler*>(sObj.data());
        if(!scheduler)
            return false;

        auto vSettings=QApr::Application::i().manager().settingBody(__services);
        {
            Q_DECLARE_VU;
            auto vSettingsDefault=vSettings.value(__default).toHash();
            //auto vSettingsAgent=vSettings.value(__agent).toHash();
            auto vSettingsService=vSettings.value(method.name()).toHash();
            vSettings=vu.vMerge(vSettingsDefault, vSettingsService).toHash();
        }

        const auto &annotations = scheduler->annotation(method);

        if(!annotations.contains(agent->scSchedule()))
            return false;

        auto scTaskEnabled = annotations.find(agent->scTaskEnabled()).value();
        auto scExecTimeLimit = annotations.find(agent->scExecTimeLimit()).value();
        auto scExecTimeInitial = annotations.find(agent->scExecTimeInitial()).value();
        auto scExecTime = annotations.find(agent->scExecTime()).value();

        QStm::Envs envs;
        envs
            .clearUnfoundEnvs(true)
            .customEnvs(vSettings);

        scTaskEnabled = envs.parser(scTaskEnabled);
        scExecTimeLimit = envs.parser(scExecTimeLimit);
        scExecTimeInitial = envs.parser(scExecTimeInitial);
        scExecTime = envs.parser(scExecTime);

        if (scTaskEnabled.isValid() && !scTaskEnabled.toBool())
            return false;

        static const auto t10m="10m";
        static const auto t1m="1m";
        static const auto t100ms="100ms";

        scExecTime=(scExecTime.isValid())?scExecTime:t1m;
        scExecTimeInitial=(scExecTimeInitial.isValid())?scExecTimeInitial:t100ms;
        scExecTimeLimit=(scExecTimeLimit.isValid())?scExecTimeLimit:t10m;

        auto task=SchedulerTask::builder(this)
                        .name(method.name())
                        .taskMetaMethod(method)
                        .taskMetaObject(metaObject)
                        .build();

        if(!this->tasks.contains(task->uuid())){

        }

        task->settings().clear();
        task->settings().setActivityInterval(scExecTime);
        task->settings().setActivityIntervalInitial(scExecTimeInitial);
        task->settings().setActivityLimit(scExecTimeLimit);
        task->settings().setEnabled(scTaskEnabled.toBool());

        this->tasks.insert(task->uuid(), task);
        return true;
    }


    bool serviceRegister(const QMetaObject *metaObject)
    {
        if (!metaObject->inherits(&Scheduler::staticMetaObject))
            return false;

        QScopedPointer<QObject> sObj(metaObject->newInstance(Q_ARG(QObject*, this )));
        if(!sObj.data())
            return false;
        auto scheduler=dynamic_cast<Scheduler*>(sObj.data());
        if(!scheduler)
            return false;

        auto vSettings=QApr::Application::i().manager().settingBody(__services);
        QStm::Envs envs;
        envs
            .clearUnfoundEnvs(true)
            .customEnvs(vSettings);

        QByteArray scopeClass;
        QAnnotation::Annotation scExecScopeClass;
        {//class annotation
            const auto &annotations = scheduler->annotation();
            const auto &scExecScope = annotations.find(agent->scExecScope());
            scopeClass=envs.parser(scExecScope.value()).toByteArray();
            auto ann=scExecScope.toHash();
            ann.insert(QByteArrayLiteral("v"), scopeClass);
            scExecScopeClass=QAnnotation::Annotation(ann);
            scopeClass=scopeClass.isEmpty()
                             ?scopeClass
                             :scopeClass+"_";
        }

        for (int index = 0; index < metaObject->methodCount(); ++index) {
            auto method=metaObject->method(index);

            if(method.methodType()!=QMetaMethod::Method)
                continue;

            const auto &annotations = scheduler->annotation(method);

            if(!annotations.contains(agent->scSchedule()))
                continue;

            if(!scExecScopeClass.isEmpty()){
                const auto &scExecScopeMethod = annotations.find(agent->scExecScope());
                if(!scExecScopeMethod.contains(scExecScopeClass))
                    continue;

            }

            const auto &scExecGroup = annotations.find(agent->scExecGroup());
            auto scopeKey=scopeClass+envs.parser(scExecGroup.value()).toByteArray();
            auto &scope=this->scope[scopeKey];
            if(scope==nullptr)
                scope=new SchedulerScopeGroup(scopeKey, metaObject, this);
            scope->methods().append(index);
        }
        return !scope.isEmpty();
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
    aDebug()<<QStringLiteral("Scheduler running");
    QTimer::singleShot(100,this,[this](){
        p->start();
    });
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

bool SchedulerAgent::serviceRegister(const QMetaObject &metaObject)
{
    return p->serviceRegister(&metaObject);
}

}
