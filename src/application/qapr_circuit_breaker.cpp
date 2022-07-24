#include "./qapr_circuit_breaker.h"
#include "./private/p_qapr_circuit_breaker.h"
#include "./qapr_consts.h"
#if Q_APR_LOG
#include "./qapr_macro.h"
#endif
#include <QCoreApplication>
#include <QtReforce/QStm>

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
    return *this;
}

CircuitBreaker &CircuitBreaker::setSettings(const QVariantHash &v)
{
    p->setting=v;
    return *this;
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
    settingService=p->setting;
    settingService.print();
}

bool CircuitBreaker::isRunning()
{
    return p->timerBreaker!=nullptr;
}

}

