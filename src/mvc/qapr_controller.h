#pragma once

#include "../../../qstm/src/qstm_macro.h"
#include "../../../qorm/src/qorm_controller.h"
#include "../../../qrpc/src/qrpc_listen_request.h"
#include "../../../qrpc/src/qrpc_service_setting.h"
#include "../application/qapr_global.h"
#include <QUuid>

namespace QApr {
class ControllerPvt;
class Interface;

//!
//! \brief The Controller class
//!class controller to business
class Q_APR_EXPORT Controller : public QOrm::Controller
{
    Q_OBJECT
public:
    //!
    //! \brief Controller
    //! \param parent
    //!
    Q_INVOKABLE explicit Controller(QObject *parent = nullptr);

    //!
    //! \brief Controller
    //! \param connection
    //! \param parent
    //!
    Q_INVOKABLE explicit Controller(const QSqlDatabase&connection, QObject *parent = nullptr);

    //!
    //! \brief resultInfo
    //! \return
    //!
    Q_INVOKABLE virtual const QVariant resultInfo();

    //!
    //! \brief interfaceRequest
    //! \return
    //!
    virtual Interface *interfaceRequest();

    //!
    //! \brief irq
    //! \return
    //!alias for interfaceRequest
    virtual Interface *irq();

    //!
    //! \brief transactionRollbackForce
    //! \return
    //!se true mesmo com tudo correto ocorrerá o rollback da transacao
    virtual bool transactionRollbackForce() const;

    //!
    //! \brief setTransactionRollbackForce
    //! \param value
    //!se true mesmo com tudo correto ocorrerá o rollback da transacao
    virtual void setTransactionRollbackForce(bool value);

private:
    ControllerPvt *p = nullptr;
};

}
