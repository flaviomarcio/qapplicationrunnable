#pragma once

#include <QCoreApplication>
#include <QObject>
#include "../../../qrpc/src/qrpc_setting_manager.h"
#include "../../../qorm/src/qorm_connection_manager.h"
#include "../../../qstm/src/qstm_envs.h"
#include "./qapr_global.h"
#include "./qapr_setting_file.h"
#include "./qapr_circuit_breaker.h"
#include "./qapr_settings.h"
#include "./qapr_setting_file.h"

namespace QApr {
class ApplicationPvt;

//!
//! \brief The Application class
//!
class Q_APR_EXPORT Application : public QObject
{
    Q_OBJECT
public:
    Q_INVOKABLE explicit Application(QObject *parent = nullptr);

    //!
    //! \brief resourceSettings
    //! \return
    //!
    virtual const SettingFile &resourceSettings();

    //!
    //! \brief manager
    //! \return
    //!configuracoes para servicos
    virtual QRpc::SettingManager &manager();

    //!
    //! \brief connectionManager
    //! \return
    //! configuracoes para conexoes com bancos de dados
    virtual QOrm::ConnectionManager &connectionManager();

    //!
    //! \brief pool
    //! \return
    //!
    virtual QOrm::ConnectionPool &pool();

    //!
    //! \brief envs
    //! \return
    //!
    virtual QStm::Envs &envs() const;

    //!
    //! \brief exec
    //! \param a
    //! \return
    //!init application and resource to Application
    virtual int exec(QCoreApplication &a);

    //!
    //! \brief i
    //! \return
    //!
    static Application &i();

    //!
    //! \brief instanceUuid
    //! \return
    //!application instance
    static const QUuid &instanceUuid();

    //!
    //! \brief arguments
    //! \return
    //!mix entre qApp->arguments() e manager->arguments();
    virtual QVariantHash &arguments() const;

    //!
    //! \brief printArguments
    //! \return
    //!
    virtual Application &printArguments();

    //!
    //! \brief circuitBreaker
    //! \return
    //!
    virtual CircuitBreaker &circuitBreaker();

    //!
    //! \brief settings
    //! \return
    //!
    virtual Settings &settings();

private:
    ApplicationPvt *p=nullptr;
};

}
