#pragma once

#include <QObject>
#include <QThread>
#include "./qapr_global.h"
#include "./qrpc_service_manager.h"
#include "./qorm_connection_manager.h"
#include "./qapr_consts.h"
#include "./qapr_agent_base.h"


#define DECLARE_AGENT_SCHEDULE(AgentClass, methodName)\
const auto agentRegistered##AgentClass = QApr::Agent::instance().serviceRegister(AgentClass::staticMetaObject, QByteArrayLiteral(#methodName));

namespace QApr{
    class Q_APR_EXPORT Agent : public QThread
    {
        Q_OBJECT
        QORM_DECLARE_INSTANCE(Agent)
    public:

        Q_INVOKABLE explicit Agent(QObject*parent=nullptr);
        Q_INVOKABLE ~Agent();
        Q_INVOKABLE static const QVariant settings_SERVER();

        /**
         * @brief run
         */
        void run() override;

        /**
         * @brief start
         * @return
         */
        virtual bool start();

        /**
         * @brief stop
         * @return
         */
        virtual bool stop();

        /**
         * @brief serviceStart
         * @param service
         */
        virtual void serviceStart(const QByteArray &service);

        /**
         * @brief serviceStats
         * @param service
         * @return
         */
        virtual QVariantHash serviceStats(const QByteArray &service);

        /**
         * @brief serviceRegister
         * @param metaObject
         * @param service
         * @return
         */
        virtual bool serviceRegister(const QMetaObject &metaObject, const QByteArray &service);

        /**
         * @brief notifySettingsChanged
         * @param payload
         * @return
         */
        virtual bool notifySettingsChanged(const QVariant &payload);
    private:
        void*p=nullptr;
    signals:
        void settingChanged(const QString&topic, const QVariant&v);
    };

    //static auto&agentApp=Agent::instance();
}
