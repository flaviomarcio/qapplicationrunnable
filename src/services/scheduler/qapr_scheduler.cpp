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

static const auto __scheduler="scheduler";

class SchedulerPvt:public QObject{
public:
    Scheduler *parent = nullptr;
    QStm::SettingBase settings;
    explicit SchedulerPvt(Scheduler *parent):QObject{parent}, parent{parent}, settings{parent}
    {

    }

    QVariantHash readSettings(const QMetaMethod &method)
    {
        Q_DECLARE_VU;
        auto &manager=QApr::Application::i().manager();
        auto vSettingsScheduler=manager.settingBody(__scheduler);
        auto vSettingsMethod=manager.settingBody(method.name().toLower());
        auto vSettings=vu.vMerge(vSettingsScheduler, vSettingsMethod).toHash();
        return vSettings;
    }
};


Scheduler::Scheduler(QObject *parent):QObject{parent}, PrivateQApr::SchedulerAnnotation{this}, p{new SchedulerPvt{this}}
{
}

const QStm::SettingBase &Scheduler::settings()const
{
    return p->settings;
}

bool Scheduler::invoke(const SchedulerScopeGroup *scope, QMetaMethod &method)
{
    aInfo()<<QStringLiteral("scope:%1, group: %2, method: %3 : start").arg(scope->scope(), scope->group(), method.name());
    p->settings.fromHash(p->readSettings(method));
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
    return __return;
}

}
