#include "qapr_scheduler_scope_group.h"
#include "qapr_scheduler.h"
#include "../application/qapr_macro.h"
#include <QObject>

namespace QApr {

class SchedulerScopeGroupPvt: public QObject
{
public:
    SchedulerScopeGroup *parent=nullptr;
    QByteArray scopeName;
    const QMetaObject *metaObject=nullptr;
    QVector<int> methods;
    explicit SchedulerScopeGroupPvt(const QByteArray &scopeName, const QMetaObject *metaObject, SchedulerScopeGroup *parent)
        :QObject{parent}, scopeName{scopeName}, metaObject{metaObject}
    {
        this->parent=parent;
    }
};

SchedulerScopeGroup::SchedulerScopeGroup(QObject *parent)
    : QObject{parent}, p{new SchedulerScopeGroupPvt{{}, nullptr, this}}
{
}

SchedulerScopeGroup::SchedulerScopeGroup(const QByteArray &scopeName, const QMetaObject *metaObject, QObject *parent)
    : QObject{parent}, p{new SchedulerScopeGroupPvt{scopeName, metaObject, this}}
{
    ;
}

QByteArray &SchedulerScopeGroup::scopeName() const
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
        auto taskObject=dynamic_cast<Scheduler*>(sObj.data());

        if(taskObject==nullptr){
            aWarning()<<tr("%1, Invalid Scheduler object: [%1], method: [%2]").arg(taskMetaObject->className(), taskMetaMethod.name());
            return;
        }

        if(!taskObject->beforeExec(this, taskMetaMethod))
            continue;

        if(!taskMetaMethod.invoke(taskObject, Qt::DirectConnection))
            aWarning()<<QStringLiteral("invoke method(%1): error==%2").arg(taskMetaMethod.name());

        taskObject->afterExec(this, taskMetaMethod);
    }

}

} // namespace QApr
