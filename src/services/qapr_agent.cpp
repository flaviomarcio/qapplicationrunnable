#include "./qapr_agent.h"
#include "../application/qapr_application.h"
#include "./qapr_agent_base.h"
#include <QMultiHash>
#include <QMutex>
#include <QTimer>

namespace QApr {

Q_GLOBAL_STATIC(Agent, staticAgent);

class AgentPvt: public QObject{
public:
    QString topicSetting;
    QMutex mutexAgent;
    Agent*agent=nullptr;
    QOrm::ConnectionNotify connectionNotify;
    QHash<QByteArray, const QMetaObject*> services;
    QHash<QByteArray, AgentBase*> tasks;
    QHash<QByteArray, QDateTime> tasksInterval;
    QTimer*timer=nullptr;
    explicit AgentPvt(Agent *parent) : QObject{parent}, agent(parent)
    {

    }

    virtual ~AgentPvt()
    {
        this->freeTimer();
    }

    void freeTimer()
    {
        if(this->timer==nullptr)
            return;
        QObject::disconnect(timer, &QTimer::timeout, this, &AgentPvt::taskCheck);
        this->timer->stop();
        this->timer->deleteLater();
        this->timer=nullptr;
    }

    QTimer*newTimer()
    {
        freeTimer();
        auto timer=new QTimer();
        timer->setInterval(1000);
        QObject::connect(timer, &QTimer::timeout, this, &AgentPvt::taskCheck);
        //timer->moveToThread(this->agent);
        return timer;
    }

    AgentBase*taskInstance(const QByteArray &service)
    {
        return this->tasks.value(service);
    }

    const QVariantHash taskStats(const QByteArray &service)
    {
        QMutexLocker<QMutex> locker(&mutexAgent);
        auto task=this->taskInstance(service);
        return (task==nullptr)?QVariantHash():task->stats();
    }
public slots:
    void taskRun(const QByteArray &service)
    {
        auto task = this->tasks.value(service);
        if(task==nullptr){
            auto metaObject=this->services.value(service);

            auto object=metaObject->newInstance(Q_ARG(QObject*, nullptr ));
            if(object==nullptr)
                return;

            task=dynamic_cast<AgentBase*>(object);
            if(task==nullptr){
                delete object;
                return;
            }

            this->tasks.insert(service, task);
            task->setAgentName(service);
            task->start();
        }

        if(task==nullptr)
            return;

        auto &settings=task->agentSetting();
        if(settings.enabled()){
            task->start();
            task->runCheck();
        }
    }

    void taskCheck()
    {
        QMutexLocker<QMutex> locker(&mutexAgent);
        QHashIterator<QByteArray, const QMetaObject*> i(this->services);
        while (i.hasNext()) {
            i.next();
            auto &service=i.key();
            this->taskRun(service);
        }
    }

    bool serviceRegister(const QMetaObject &metaObject, const QByteArray &methodNames)
    {
        static auto chars=QStringList{QStringLiteral(";"),QStringLiteral("|"),QStringLiteral(","),QStringLiteral("  ")};
        QString service=methodNames;
        for(auto &c:chars){
            while(service.contains(c))
                service=service.replace(c,QStringLiteral(" "));
        }
        auto listMethod=methodNames.split(' ');
        for(auto &service:listMethod){
            services.insert(service.trimmed(), &metaObject);
        }
        return !services.isEmpty();
    }


private slots:
    void db_notification(const QString &channel, const QVariant &payload)
    {
        if(channel.toLower()==this->topicSetting){
            this->agent->notifySettingsChanged(payload);
        }
    }
    void taskFinished(const QVariant &v)
    {
        auto service = v.toByteArray();
        auto task = this->tasks.value(service);
        if(task==nullptr)
            return;

        emit task->taskResume();
    }
};

Agent::Agent(QObject *parent):QThread{nullptr}
{
    Q_UNUSED(parent)
    this->p = new AgentPvt{this};
    this->moveToThread(this);
}

const SettingFile &Agent::resourceSettings()
{
    return QApr::Application::i().resourceSettings();
}

Agent &Agent::instance()
{
    return *staticAgent;
}

void Agent::run()
{
    p->topicSetting=QStringLiteral("agent:%1").arg(QT_STRINGIFY2(topicSetting)+QStringLiteral(":")+QString::fromUtf8(this->metaObject()->className()));
#ifdef QAPR_LOG_VERBOSE
    aWarning()<<QStringLiteral("started");
#endif
    p->timer=p->newTimer();
    p->timer->start();
    oDebug()<<QStringLiteral("Agent: running");
    this->exec();
    p->freeTimer();
}

bool Agent::start()
{
#ifdef QAPR_LOG_VERBOSE
    aWarning()<<QStringLiteral("started");
#endif
    auto objectName=this->objectName().trimmed();
    if(objectName.isEmpty()){
        objectName=this->metaObject()->className();
        while(objectName.contains(QStringLiteral(":")))
            objectName=objectName.replace(QStringLiteral(":"),QStringLiteral("_"));
        while(objectName.contains(QStringLiteral("__")))
            objectName=objectName.replace(QStringLiteral("__"),QStringLiteral("_"));
    }
    this->setObjectName(objectName.left(60));
    QThread::start();
    while(this->eventDispatcher()==nullptr)
        QThread::msleep(1);
    return true;
}

bool Agent::stop()
{
    this->quit();
    return true;
}

void Agent::serviceStart(const QByteArray &service)
{
    p->taskRun(service);
}

QVariantHash Agent::serviceStats(const QByteArray &service)
{
    return p->taskStats(service);
}

bool Agent::serviceRegister(const QMetaObject&metaObject, const QByteArray &service)
{
    return p->serviceRegister(metaObject, service);
}

bool Agent::notifySettingsChanged(const QVariant &payload)
{
    return p->connectionNotify.notify_send(p->topicSetting, payload);
}



}
