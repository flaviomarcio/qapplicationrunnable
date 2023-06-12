#pragma once

#include <QObject>
#include "./qapr_global.h"

namespace QApr {
class CircuitBreakerPvt;
//!
//! \brief The CircuitBreaker class
//!
class Q_APR_EXPORT CircuitBreaker : public QObject
{
    Q_OBJECT
public:
    //!
    //! \brief CircuitBreaker
    //! \param parent
    //!
    explicit CircuitBreaker(const QVariantHash &vSetting, QObject *parent = nullptr);

    //!
    //! \brief start
    //! \return
    //!
    virtual bool start();

    //!
    //! \brief stop
    //! \return
    //!
    virtual bool stop();

    //!
    //! \brief print
    //!
    virtual void print() const;

    //!
    //! \brief isRunning
    //! \return
    //!
    virtual bool isRunning();
private:
    CircuitBreakerPvt *p=nullptr;
};

}
