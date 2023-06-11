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
static const auto __scheduler="scheduler";
static const auto __default="default";
static const auto __scope="scope";

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

    static QByteArray scopeName(const QVariant&scope, const QVariant&group)
    {

        auto sScope=scope.toByteArray().trimmed().toLower();
        auto sGroup=group.toByteArray().trimmed().toLower();

        if(!sScope.isEmpty() && !sGroup.isEmpty()){
            static const auto __format=QStringLiteral("%1_%2");
            return __format.arg(sScope,sGroup).toUtf8();
        }

        if(!sScope.isEmpty())
            return sScope;

        return sGroup;
    }

    void start()
    {
        this->free();

        QStm::Envs envs;
        QByteArray scopeName;
        {
            Q_DECLARE_VU;
            auto &manager=QApr::Application::i().manager();
            auto vSettingsDefault=manager.settingBody(__default);
            auto vSettingsScheduler=manager.settingBody(__scheduler);
            auto vSettings=vu.vMerge(vSettingsDefault, vSettingsScheduler).toHash();
            scopeName=vSettings.value(__scope).toByteArray().trimmed().toLower();
        }

        QHashIterator <QByteArray, SchedulerScopeGroup*> i(this->scope);
        while(i.hasNext()){
            i.next();
            if(i.value()->scopeName()!=scopeName)
                continue;
            scheduleCreate(i.value());
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
        {//scope
            auto lst=scope.values();
            scope.clear();
            qDeleteAll(lst);
        }
    }

    bool scheduleCreate(SchedulerScopeGroup *scope)
    {
        auto task=SchedulerTask::builder(scope).build();
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

        for (int index = 0; index < metaObject->methodCount(); ++index) {
            auto method=metaObject->method(index);

            if(method.methodType()!=QMetaMethod::Method)
                continue;

            const auto &annotations = scheduler->annotation(method);

            if(!annotations.contains(agent->scSchedule()))
                continue;

            const auto &scExecScope = annotations.find(agent->scExecScope());
            const auto &scExecGroup = annotations.find(agent->scExecGroup());
            const auto scopeKey=scopeName(scExecScope.value(), scExecGroup.value());
            auto &scope=this->scope[scopeKey];
            if(scope==nullptr)
                scope=new SchedulerScopeGroup(scExecScope.toValueByteArray(), scExecGroup.toValueByteArray(), metaObject, this);

            scope->settings(
                [](const QMetaMethod &method){

                    auto &manager=QApr::Application::i().manager();
                    Q_DECLARE_VU;
                    auto vSettingsDefault=manager.settingBody(__default);
                    auto vSettingsScheduler=manager.settingBody(__scheduler);
                    auto vSettingsMethod=manager.settingBody(method.name());
                    auto vSettings=vSettingsDefault;
                    vSettings=vu.vMerge(vSettings, vSettingsScheduler).toHash();
                    vSettings=vu.vMerge(vSettings, vSettingsMethod).toHash();
                    return vSettings;
                }
                );
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
