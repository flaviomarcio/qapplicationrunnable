#include "./qapr_scheduler.h"
#include "../application/qapr_macro.h"
#include "../application/qapr_application.h"
#include "../../../qstm/src/qstm_util_variant.h"
#include <QTimer>
#include <QCryptographicHash>
#include <QVariant>
#include <QMutex>
#include <QMetaProperty>

namespace QApr{

class SchedulerPvt:public QObject{
public:
    Scheduler *parent = nullptr;
    QStm::SettingBase settings;
    QStm::Envs env;
    explicit SchedulerPvt(Scheduler *parent):QObject{parent}, parent{parent}, settings{this},env{this}
    {

    }

    void readSettings(const QMetaMethod &method)
    {
        Q_DECLARE_VU;
        auto &manager=QApr::Application::i().manager();
        auto vSettings=manager.settingBody(method.name());

        this->settings
            .clear()
            .fromHash(vSettings);

        this->env
            .customEnvs(vSettings);
    }
};


Scheduler::Scheduler(QObject *parent):QObject{parent}, PrivateQApr::SchedulerAnnotation{this}, p{new SchedulerPvt{this}}
{
}

const QStm::SettingBase &Scheduler::settings()const
{
    return p->settings;
}

bool Scheduler::canEnabled(const SchedulerScopeGroup *scope, QMetaMethod &method)
{
    Q_UNUSED(scope)

    p->readSettings(method);

    const auto &annotations = this->annotation(method);
    auto scEnabled=annotations.find(this->scEnabled()).value();
    scEnabled=p->env.parser(scEnabled);

    if(!scEnabled.toBool())
        return {};
    return true;
}

bool Scheduler::canInvoke(const SchedulerScopeGroup *scope, QMetaMethod &method)
{
    Q_UNUSED(scope)
    Q_UNUSED(method)
    return true;
}

Scheduler::InvokeReturn Scheduler::invoke(const SchedulerScopeGroup *scope, QMetaMethod &method)
{
    if(!this->canEnabled(scope, method))
        return DISABLED;

    if(!this->canInvoke(scope, method))
        return SKIPPED;

    aInfo()<<QStringLiteral("scope:%1, group: %2, method: %3 : start").arg(scope->scope(), scope->group(), method.name());

    QString message;
    bool __return=false;
    if(!this->invokeBefore(scope, method))
        message="invokeBefore fail";
    else if(!method.invoke(this, Qt::DirectConnection)){
        message="invoke fail";
        this->invokeFail(scope, method);
    }
    else if(!this->invokeAfter(scope, method)){
        message="invokeAfter fail";
    }
    else{
        message="invoke successful";
        __return=true;
    }
    aInfo()<<QStringLiteral("scope:%1, group: %2, method: %3 : %4").arg(scope->scope(), scope->group(), method.name(), message);
    return __return
               ?SUCCESSFUL
               :FAIL;
}

}
