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
    const QMetaObject metaObject;
    QHash<QString, int> methods;
    QVariantHash stackStats;

    explicit SchedulerScopeGroupPvt(SchedulerScopeGroup *parent)
        :QObject{parent}, parent{parent}, metaObject{QObject::staticMetaObject}
    {
        QObject::connect(parent, &SchedulerScopeGroup::invokeState, this, &SchedulerScopeGroupPvt::invokeState);
    }
    explicit SchedulerScopeGroupPvt(SchedulerScopeGroup *parent, const QUuid &uuid, const QString &scope, const QString &group, const QMetaObject &metaObject)
        :QObject{parent}, parent{parent}, uuid{uuid}, scope{scope.trimmed().toLower()}, group{group.trimmed().toLower()}, metaObject{metaObject}
    {
        QObject::connect(parent, &SchedulerScopeGroup::invokeState, this, &SchedulerScopeGroupPvt::invokeState);
    }

    ~SchedulerScopeGroupPvt()
    {
        QObject::disconnect(parent, &SchedulerScopeGroup::invokeState, this, &SchedulerScopeGroupPvt::invokeState);
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
            listEnv<<sScope<<sGroup;
        else if(!sScope.isEmpty())
            listEnv={sScope};
        else if(!sGroup.isEmpty())
            listEnv={sGroup};
        else
            return QUuid::createUuid();

        QString bytes=listEnv.join('.');
        static const auto __format=QStringLiteral("{%1}");
        QString suuid=__format.arg(QCryptographicHash::hash(bytes.toUtf8(), QCryptographicHash::Md5).toHex());
        return QUuid(suuid);
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

SchedulerScopeGroup::SchedulerScopeGroup(const QUuid &uuid, const QString &scope, const QString &group, const QMetaObject &metaObject, QObject *parent)
    : QObject{parent}, p{new SchedulerScopeGroupPvt{this, uuid, scope, group, metaObject}}
{
}

void SchedulerScopeGroup::reg(const QMetaObject &metaObject)
{
    if (!metaObject.inherits(&Scheduler::staticMetaObject))
        return;

    QScopedPointer<QObject> sObj(metaObject.newInstance(Q_ARG(QObject*, nullptr)));
    if(!sObj.data())
        return;

    auto scheduler=dynamic_cast<Scheduler*>(sObj.data());
    if(!scheduler)
        return;

    QVector<QMetaMethod> methodList;
    for (int index = 0; index < metaObject.methodCount(); ++index) {
        auto method=metaObject.method(index);

        if(method.methodType()!=QMetaMethod::Method)
            continue;

        methodList.append(method);
    }

    for( auto &method: methodList){
        const auto &annotations = scheduler->annotation(method);

        if(!annotations.contains(scheduler->scSchedule()))
            continue;

        auto scExecScope = annotations.find(scheduler->scExecScope()).toValueStringList();
        auto scExecGroup = annotations.find(scheduler->scExecGroup()).toValueStringList();
        auto scExecOrder = annotations.find(scheduler->scExecOrder()).toValueInt();

        static const auto __formatOrder=QStringLiteral("%1-%2-%3");

        for(auto &scopeName:scExecScope){
            scopeName=scopeName.trimmed().isEmpty()?__default:scopeName.trimmed().toLower();
            for(auto &groupName:scExecGroup){
                groupName=groupName.trimmed().isEmpty()?__default:groupName.trimmed().toLower();

                auto scopeUuid=SchedulerScopeGroupPvt::scopeUuid(scopeName, groupName);
                auto scope=scopeCache->value(scopeUuid);
                if(scope==nullptr){
                    scope=new SchedulerScopeGroup(scopeUuid, scopeName, groupName,  metaObject);
                    scopeCache->insert(scope->uuid(), scope);
                }

                auto sortName=__formatOrder.arg( scopeName, groupName, QString::number(scExecOrder).rightJustified(11,'0') ).toLower();
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

        QScopedPointer<QObject> sObj(p->metaObject.newInstance(Q_ARG(QObject*, nullptr)));

        if(sObj.data()==nullptr){
            aWarning()<<tr("%1, Invalid Scheduler object: [%1], scope: [%2], group: [%2]").arg(p->metaObject.className(), this->scope(), this->group());
            return;
        }
        auto scheduler=dynamic_cast<Scheduler*>(sObj.data());

        if(scheduler==nullptr){
            aWarning()<<tr("%1, Invalid Scheduler object: [%1], scope: [%2], group: [%2]").arg(p->metaObject.className(), this->scope(), this->group());
            return;
        }

        emit invokeState(stackId, QDateTime::currentDateTime(), method.name(), Stated);
        if(scheduler->invoke(this, method))
            emit invokeState(stackId, QDateTime::currentDateTime(), method.name(), Fail);
        else
            emit invokeState(stackId, QDateTime::currentDateTime(), method.name(), SuccessFul);

    }
}

} // namespace QApr
