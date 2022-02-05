#pragma once

#include "../../../qrpc/src/qrpc_controller.h"
#include "../application/qapr_global.h"
#include "../application/qapr_macro.h"

namespace QApr {

//!
//! \brief The InterfaceMaintence class
//!
class Q_APR_EXPORT InterfaceMaintence : public QRpc::QRPCController
{
    Q_OBJECT
    QRPC_DECLARE_ROUTE(QRPCInterfaceCheck,"v1/maintence")
    Q_APR_DECLARE_INTERFACE_METHOD_CHECK()
public:

    //!
    //! \brief InterfaceMaintence
    //! \param parent
    //!
    Q_INVOKABLE explicit InterfaceMaintence(QObject *parent = nullptr);
    ~InterfaceMaintence();

    //!
    //! \brief serverStart
    //! \return
    //!
    Q_INVOKABLE virtual QVariant serverStart();

    //!
    //! \brief serverStop
    //! \return
    //!
    Q_INVOKABLE virtual QVariant serverStop();

    //!
    //! \brief serverRestart
    //! \return
    //!
    Q_INVOKABLE virtual QVariant serverRestart();

    //!
    //! \brief servicesCheck
    //! \return
    //!
    Q_INVOKABLE virtual QVariant servicesCheck();

    //!
    //! \brief applicationQuit
    //! \return
    //!
    Q_INVOKABLE virtual QVariant applicationQuit();
signals:

};

QRPC_CONTROLLER_AUTO_REGISTER(InterfaceMaintence)

} // namespace QRpc
