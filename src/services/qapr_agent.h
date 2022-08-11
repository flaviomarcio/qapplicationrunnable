#pragma once

#include <QObject>
#include <QThread>
#include <QVariant>
#include "../application/qapr_global.h"

#define QAPR_AGENT_DECLARE_SCHEDULE(AgentClass, methodName)\
const auto agentRegistered##AgentClass = QApr::Agent::instance().serviceRegister(AgentClass::staticMetaObject, QByteArrayLiteral(#methodName));

namespace QApr{
class AgentPvt;
//!
//! \brief The Agent class
//!
class Q_APR_EXPORT Agent : public QThread
{
    Q_OBJECT
public:
    //!
    //! \brief Agent
    //! \param parent
    //!
    Q_INVOKABLE explicit Agent(QObject *parent=nullptr);

    //!
    //! \brief resourceSettings
    //! \return
    //!
    Q_INVOKABLE static const QVariant resourceSettings();

    //!
    //! \brief instance
    //! \return
    //!
    static Agent &instance();

    //!
    //! \brief run
    //!
    void run() override;

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
    //! \brief serviceStart
    //! \param service
    //!
    virtual void serviceStart(const QByteArray &service);

    //!
    //! \brief serviceStats
    //! \param service
    //! \return
    //!
    virtual QVariantHash serviceStats(const QByteArray &service);

    //!
    //! \brief serviceRegister
    //! \param metaObject
    //! \param service
    //! \return
    //!
    virtual bool serviceRegister(const QMetaObject &metaObject, const QByteArray &service);

    //!
    //! \brief notifySettingsChanged
    //! \param payload
    //! \return
    //!
    virtual bool notifySettingsChanged(const QVariant &payload);
private:
    AgentPvt *p=nullptr;
signals:
    //!
    //! \brief settingChanged
    //! \param topic
    //! \param v
    //!
    void settingChanged(const QString &topic, const QVariant &v);
};

}
