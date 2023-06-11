#include "qapr_scheduler_scope_group.h"
#include "qapr_scheduler.h"
#include "../application/qapr_macro.h"
#include "../../../qstm/src/qstm_envs.h"
#include "../../../qnotation/src/qnotation_notation.h"
#include <QCryptographicHash>
#include <QVariant>

namespace QApr {

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

class SchedulerScopeGroupPvt: public QObject
{
public:
    SchedulerScopeGroup *parent=nullptr;
    QUuid uuid;
    QByteArray scopeName, groupName;
    const QMetaObject *metaObject=nullptr;
    QVector<int> methods;
    SchedulerScopeSettingMethod settings=nullptr;
    explicit SchedulerScopeGroupPvt(const QByteArray &scopeName, const QByteArray &groupName, const QMetaObject *metaObject, SchedulerScopeGroup *parent)
        :QObject{parent}, scopeName{scopeName.toLower().trimmed()}, groupName{groupName.toLower().trimmed()}, metaObject{metaObject}
    {
        this->parent=parent;
        {
            static const auto __format=QStringLiteral("{%1}");
            QByteArray bytes=this->metaObject->className();
            this->uuid=QUuid::fromString(__format.arg(QCryptographicHash::hash(bytes, QCryptographicHash::Md5)));
        }
    }
};

SchedulerScopeGroup::SchedulerScopeGroup(QObject *parent)
    : QObject{parent}, p{new SchedulerScopeGroupPvt{{}, {}, nullptr, this}}
{
}

SchedulerScopeGroup::SchedulerScopeGroup(const QByteArray &scopeName, const QByteArray &groupName, const QMetaObject *metaObject, QObject *parent)
    : QObject{parent}, p{new SchedulerScopeGroupPvt{scopeName, groupName, metaObject, this}}
{
}

QUuid &SchedulerScopeGroup::uuid() const
{
    return p->uuid;
}

QByteArray &SchedulerScopeGroup::scopeName() const
{
    return p->scopeName;
}

QByteArray &SchedulerScopeGroup::groupName() const
{
    return p->scopeName;
}

const QMetaObject *SchedulerScopeGroup::scopeMetaObject() const
{
    return p->metaObject;
}

QVector<int> &SchedulerScopeGroup::methods() const
{
    return p->methods;
}

SchedulerScopeSettingMethod SchedulerScopeGroup::settings()
{
    return p->settings;
}

SchedulerScopeGroup &SchedulerScopeGroup::settings(const SchedulerScopeSettingMethod &settings)
{
    p->settings=settings;
    return *this;
}

void SchedulerScopeGroup::invoke(QObject *parent)const
{
    auto &taskMetaObject=p->metaObject;

    for(auto index: p->methods){
        auto taskMetaMethod=taskMetaObject->method(index);
        if(!taskMetaMethod.isValid())
            continue;

        QScopedPointer<QObject> sObj(taskMetaObject->newInstance(Q_ARG(QObject*, parent)));

        if(sObj.data()==nullptr){
            aWarning()<<tr("%1, Invalid Scheduler metaObject: [%1], method: [%2]").arg(taskMetaObject->className(), taskMetaMethod.name());
            return;
        }
        Q_UNUSED(parent)
        auto scheduler=dynamic_cast<Scheduler*>(sObj.data());

        if(scheduler==nullptr){
            aWarning()<<tr("%1, Invalid Scheduler object: [%1], method: [%2]").arg(taskMetaObject->className(), taskMetaMethod.name());
            return;
        }

        if(!scheduler->beforeExec(this, taskMetaMethod))
            continue;

        const auto &annotations = scheduler->annotation(taskMetaMethod);

        QStm::Envs envs;
        if(p->settings){
            auto vSetting=p->settings(taskMetaMethod);
            scheduler->settings()=vSetting;
            envs.customEnvs(vSetting);
        }

        auto scopeMethod = annotations.find(Scheduler::scExecScope()).toValueByteArray().toLower();
        auto groupMethod = annotations.find(Scheduler::scExecGroup()).toValueByteArray().toLower();

        if((this->scopeName()!=scopeMethod) || (this->groupName()!=groupMethod))
            continue;

        if(!taskMetaMethod.invoke(scheduler, Qt::DirectConnection))
            aWarning()<<QStringLiteral("invoke method(%1): error==%2").arg(taskMetaMethod.name());

        scheduler->afterExec(this, taskMetaMethod);
    }

}

} // namespace QApr
