#include "./qapr_circuit_breaker.h"
#include "../../../../qstm/src/qstm_setting_base.h"
#include "../../../../qstm/src/qstm_util_variant.h"
#include "./qapr_macro.h"
#include "./qapr_consts.h"
#if Q_APR_LOG
#include "./qapr_macro.h"
#endif
#include <QCoreApplication>
#include <QTimer>

namespace QApr {

class CircuitBreakerPvt:public QObject
{
public:
    QStm::SettingBase settingService;
    QVariantHash setting;
    QTimer*timerBreaker=nullptr;

    QDateTime activityLimit;
    qlonglong memoryLimitKB=0;

    explicit CircuitBreakerPvt(const QVariantHash &vSetting, QObject *parent):QObject{parent}, setting{vSetting}
    {
    }

    virtual ~CircuitBreakerPvt()
    {
        this->stop();
    }

    bool start()
    {
        settingService.fromHash(this->setting);
        if(!settingService.enabled())
            return this->stop();

        if(settingService.activityLimit()<=0)
            return this->stop();

        if(this->timerBreaker==nullptr){
            this->timerBreaker=new QTimer(this);
            QObject::connect(this->timerBreaker, &QTimer::timeout,this, &CircuitBreakerPvt::onCheck);
            this->timerBreaker->setInterval(1000);
            this->timerBreaker->start();
        }
        return (this->timerBreaker!=nullptr) && (this->timerBreaker->isActive());
    }

    bool stop()
    {
        if(this->timerBreaker!=nullptr){
            QObject::disconnect(this->timerBreaker, &QTimer::timeout, this, &CircuitBreakerPvt::onCheck);
            timerBreaker->stop();
            delete timerBreaker;
            this->timerBreaker=nullptr;
        }
        return (this->timerBreaker==nullptr);
    }

private:
    void onCheck()
    {
        const auto _activityLimit=settingService.activityLimit();
        const auto _memoryLimit=settingService.memoryLimit();
        const auto _now = QDateTime::currentDateTime();

        if(!this->activityLimit.isValid() || this->activityLimit.isNull())
            this->activityLimit=_activityLimit<=0?QDateTime():_now.addMSecs(_activityLimit);

        if(this->memoryLimitKB<=0)
            this->memoryLimitKB=_memoryLimit/1024;//to KB

        if(activityLimit.isValid() && (_now>this->activityLimit)){
            aWarning()<<QStringLiteral("break application by timeout");
            this->onBreak();
        }

        //    const auto memoryUsage=Application::memoryUsage();
        //    const auto &memoryLimitKB=this->memoryLimitKB;

        //    if(memoryLimitKB>0 && (memoryUsage > memoryLimitKB)){
        //        aWarning()<<QStringLiteral("break application by memory limit(limitMem: %1 KB, curMen==%2 KB)").arg(memoryUsage, memoryLimitKB);
        //        this->onBreak();
        //    }
    }

    void onBreak()
    {
        QObject::disconnect(this->timerBreaker, &QTimer::timeout,this, &CircuitBreakerPvt::onCheck);
        this->timerBreaker->stop();
        aWarning()<<QStringLiteral("circuit will be stopped");
        qApp->quit();
    }
};

CircuitBreaker::CircuitBreaker(const QVariantHash &vSetting, QObject *parent):QObject{parent}, p{new CircuitBreakerPvt{vSetting, this}}
{
}

bool CircuitBreaker::start()
{
    return p->start();
}

bool CircuitBreaker::stop()
{
    return p->stop();
}

void CircuitBreaker::print()const
{
    Q_DECLARE_VU;
    aInfo()<<QStringLiteral(".");
    QStm::SettingBase settingService;
    settingService.fromHash(p->setting);
    settingService.print();
}

bool CircuitBreaker::isRunning()
{
    return p->timerBreaker!=nullptr;
}

}

