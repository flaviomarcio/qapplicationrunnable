#include "./private/p_qapr_circuit_breaker.h"

namespace QApr {

CircuitBreaker::CircuitBreaker(QObject *parent):QObject{parent}
{
    this->p=new CircuitBreakerPvt{this};
}

QVariantHash CircuitBreaker::settings() const
{
    return p->setting;
}

CircuitBreaker &CircuitBreaker::settings(const QVariantHash &v)
{
    p->setting=v;
    return*this;
}

CircuitBreaker &CircuitBreaker::setSettings(const QVariantHash &v)
{
    p->setting=v;
    return*this;
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
    sInfo()<<qsl(".");
    QStm::SettingBase settingService;
    settingService=p->setting;
    settingService.print();
}

bool CircuitBreaker::isRunning()
{
    return p->timerBreaker!=nullptr;
}

}

