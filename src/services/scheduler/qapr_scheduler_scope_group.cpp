#include "qapr_scheduler_scope_group.h"
#include "qapr_scheduler.h"
#include "../application/qapr_macro.h"
#include <QCryptographicHash>
#include <QMetaMethod>
#include <QMetaEnum>

namespace QApr {

static const auto __attrEvents="events";
static const auto __attrMethod="method";
static const auto __attrStack="stack";
static const auto __default="default";
static const auto __methodName="methodName";
static const auto __stackId="stackId";
static const auto __stacks="stacks";

typedef QHash<QUuid, SchedulerScopeGroup*> ScopeCache;
typedef QVector<SchedulerScopeGroup*> ScopeList;

Q_GLOBAL_STATIC(ScopeCache, scopeCache);
Q_GLOBAL_STATIC(ScopeList, scopeList);

class SchedulerScopeGroupPvt: public QObject
{
public:
    SchedulerScopeGroup *parent=nullptr;
    const QUuid uuid;
    const QString scope, group;
    bool synchronize=false;
    const QMetaObject metaObject;
    QHash<QString, int> methods;
    QVariantHash stackStats;

    explicit SchedulerScopeGroupPvt(SchedulerScopeGroup *parent)
        :QObject{parent}, parent{parent}, metaObject{QObject::staticMetaObject}
    {
        QObject::connect(parent, &SchedulerScopeGroup::invokeState, this, &SchedulerScopeGroupPvt::invokeState);
    }

    explicit SchedulerScopeGroupPvt(SchedulerScopeGroup *parent, const QUuid &uuid, const QString &scope, const QString &group, bool synchronize, const QMetaObject &metaObject)
        :QObject{parent}, parent{parent}, uuid{uuid}, scope{scope.trimmed().toLower()}, group{group.trimmed().toLower()}, synchronize{synchronize}, metaObject{metaObject}
    {
        QObject::connect(parent, &SchedulerScopeGroup::invokeState, this, &SchedulerScopeGroupPvt::invokeState);
    }

    ~SchedulerScopeGroupPvt()
    {
        QObject::disconnect(parent, &SchedulerScopeGroup::invokeState, this, &SchedulerScopeGroupPvt::invokeState);
    }

    static QUuid toMd5Uuid(const QByteArray &md5)
    {
        QByteArray suuid;
        auto hashMD5=QString(md5).replace(QStringLiteral("-"),"").replace(QStringLiteral("{"),"").replace(QStringLiteral("}"),"");
        if(hashMD5.length()==32){
            int i=0;
            for(auto &c:hashMD5){
                ++i;
                suuid.append(c.toLatin1());
                if(i==8 || i==12 || i==16 || i==20)
                    suuid.append(QByteArrayLiteral("-"));
            }
            static const auto __format=QStringLiteral("{%1}");
            auto uuid=QUuid::fromString(__format.arg(suuid));
            return uuid;
        }
        return {};
    }

    static QUuid scopeUuid(const QString &scope, const QString &group)
    {
        const auto &sScope=scope.isEmpty()
                                 ?QByteArray{}
                                 :scope.trimmed().toLower();
        const auto &sGroup=group.isEmpty()
                                 ?QByteArray{}
                                 :group.trimmed().toLower();

        QStringList listEnv;
        if(!sScope.isEmpty() && !sGroup.isEmpty())
            listEnv={sScope, sGroup};
        else if(!sScope.isEmpty())
            listEnv={sScope};
        else if(!sGroup.isEmpty())
            listEnv={sGroup};
        else
            return QUuid::createUuid();
        return toMd5Uuid(QCryptographicHash::hash(listEnv.join('.').toUtf8(), QCryptographicHash::Md5).toHex());
    }

private slots:

    void regState(const QString &stateName, const QString &sAttrName, const QString &eventName, const QUuid &stackId, const QDateTime &dt, const QByteArray &methodName)
    {
/*
    Attr(QString attrName)
        -events(__attrEvents)
            +{event}(QString eventName)
                -staks(__stackId)
                    +stack(QUuid stackId)
                        -stackId    QUuid
                        -methodName QString
                        -started    QDateTime, generate by(const QString &stateName)
                        -fail       QDateTime, generate by(const QString &stateName)
                        -finished   QDateTime, generate by(const QString &stateName)

*/
        auto attr=this->stackStats.value(sAttrName).toHash();
        {
            auto attrEvents=attr.value(__attrEvents).toHash();
            {
                auto vEventsHash=attrEvents.value(eventName).toHash();
                {
                    auto vStacks=vEventsHash.value(__stacks).toHash();
                    {
                        auto vStackId=vStacks.value(stackId.toString()).toHash();
                        if(!vStackId.contains(__stackId))
                            vStackId.insert(__stackId, stackId);
                        if(!vStackId.contains(__methodName))
                            vStackId.insert(__methodName, methodName);
                        vStackId.insert(stateName, dt);

                        vStacks.insert(stackId.toString(), vStackId);
                    }
                    vEventsHash.insert(__stacks, vStacks);
                }
                attrEvents.insert(eventName, vEventsHash);
            }
            attr.insert(__attrEvents, attrEvents);
        }
        attr.insert(sAttrName, attr);
    }
private slots:
    void invokeState(QUuid stackId, QDateTime dt, QByteArray methodName, int state)
    {
        QMetaEnum stateEnum=QMetaEnum::fromType<SchedulerScopeGroup::InvokeState>();
        auto stateName=QByteArray(stateEnum.valueToKey(state)).toLower();
        regState(stateName, __attrStack, stackId.toString(), stackId, dt, methodName);
        regState(stateName, __attrMethod, methodName, stackId, dt, methodName);
    }

};

SchedulerScopeGroup::SchedulerScopeGroup(QObject *parent)
    : QObject{parent}, p{new SchedulerScopeGroupPvt{this}}
{
}

SchedulerScopeGroup::SchedulerScopeGroup(const QUuid &uuid, const QString &scope, const QString &group, bool synchronize, const QMetaObject &metaObject, QObject *parent)
    : QObject{parent}, p{new SchedulerScopeGroupPvt{this, uuid, scope, group, synchronize, metaObject}}
{
}

void SchedulerScopeGroup::reg(const QMetaObject &metaObject)
{
    if (!metaObject.inherits(&Scheduler::staticMetaObject))
        return;

    QScopedPointer<QObject> sObj(metaObject.newInstance(Q_ARG(QObject *, nullptr)));
    if(!sObj.data())
        return;

    auto scheduler=dynamic_cast<Scheduler*>(sObj.data());
    if(!scheduler)
        return;

    bool scSynchronize=false;
    {
        const auto &annotations = scheduler->annotation();
        scSynchronize=annotations.find(scheduler->scSynchronize()).toValueBool(false);
    }

    QVector<QMetaMethod> methodList;

    for (int index = 0; index < metaObject.methodCount(); ++index) {
        auto method=metaObject.method(index);

        if(method.methodType()!=QMetaMethod::Method)
            continue;

        const auto &annotations = scheduler->annotation(method);

        if(!annotations.contains(scheduler->scSchedule()))
            continue;

        methodList.append(method);
    }

    for( auto &method: methodList){
        const auto &annotations = scheduler->annotation(method);

        auto scExecScope = annotations.find(scheduler->scExecScope()).toValueStringList(__default);
        auto scExecGroup = annotations.find(scheduler->scExecGroup()).toValueStringList(__default);
        auto scExecOrder = annotations.find(scheduler->scExecOrder()).toValueInt(-1);

        static const auto __formatOrder=QStringLiteral("%1-%2-%3");
        static const auto __a="a-";
        static const auto __b="b-";
        for(auto &scopeName:scExecScope){
            scopeName=scopeName.toLower();
            for(auto &groupName:scExecGroup){
                groupName=groupName.toLower();

                auto scopeUuid=SchedulerScopeGroupPvt::scopeUuid(scopeName, groupName);
                auto scope=scopeCache->value(scopeUuid);
                if(scope==nullptr){
                    scope=new SchedulerScopeGroup(scopeUuid, scopeName, groupName, scSynchronize, metaObject);
                    scopeCache->insert(scope->uuid(), scope);
                    scopeList->append(scope);
                }

                QString sortTag=(scExecOrder>=0)
                                      ?__a+QString::number(scExecOrder).rightJustified(11,'0')
                                      :__b+method.name().toLower();

                auto sortName=__formatOrder.arg( scopeName, groupName, sortTag).toLower();
                if(!scope->p->methods.contains(sortName))
                    scope->p->methods.insert(sortName, method.methodIndex());
            }
        }
    }
}

const QVector<SchedulerScopeGroup*> &SchedulerScopeGroup::scopes()
{
    return *scopeList;
}

const QUuid &SchedulerScopeGroup::uuid() const
{
    return p->uuid;
}

const QString &SchedulerScopeGroup::scope() const
{
    return p->scope;
}

const QString &SchedulerScopeGroup::group() const
{
    return p->group;
}

bool SchedulerScopeGroup::synchronize()
{
    return p->synchronize;
}

bool SchedulerScopeGroup::isScope(const QStringList &scope) const
{
    if(scope.isEmpty() && this->scope().isEmpty())
        return true;

    if(scope.contains(this->scope(), Qt::CaseInsensitive))
        return true;

    return {};
}

void SchedulerScopeGroup::invoke()
{
    auto methodKeys=p->methods.keys();
    methodKeys.sort(Qt::CaseInsensitive);
    QVector<QMetaMethod> methodList;
    for(auto &key:methodKeys){
        auto method=p->metaObject.method(p->methods.value(key));
        if(!method.isValid())
            continue;

        methodList.append(method);
    }

    QUuid stackId=QUuid::createUuid();

    for(auto method: methodList){

        QScopedPointer<QObject> sObj(p->metaObject.newInstance(Q_ARG(QObject *, nullptr)));

        if(sObj.data()==nullptr){
            aWarning()<<tr("%1, Invalid Scheduler object: [%2], scope: [%3], group: [%4").arg(p->metaObject.className(), this->scope(), this->group());
            return;
        }
        auto scheduler=dynamic_cast<Scheduler*>(sObj.data());

        if(scheduler==nullptr){
            aWarning()<<tr("%1, Invalid Scheduler object: [%2], scope: [%3], group: [%4]").arg(p->metaObject.className(), this->scope(), this->group());
            return;
        }

        emit invokeState(stackId, QDateTime::currentDateTime(), method.name(), STARTED);
        switch (scheduler->invoke(this, method)) {
        case Scheduler::SUCCESSFUL:
            emit invokeState(stackId, QDateTime::currentDateTime(), method.name(), SUCCESSFUL);
            break;
        case Scheduler::FAIL:
            emit invokeState(stackId, QDateTime::currentDateTime(), method.name(), FAIL);
            break;
        case Scheduler::SKIPPED:
            emit invokeState(stackId, QDateTime::currentDateTime(), method.name(), SKIPPED);
            break;
        case Scheduler::NOTHING:
            emit invokeState(stackId, QDateTime::currentDateTime(), method.name(), NOTHING);
            break;
        default:
            emit invokeState(stackId, QDateTime::currentDateTime(), method.name(), NOTHING);
            break;
        }


    }
}

} // namespace QApr
