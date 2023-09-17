#pragma once

#include "./qapr_interface_annotations.h"
#include "../application/qapr_global.h"
#include "../application/qapr_macro.h"
#include "../../../qrpc/src/qrpc_controller.h"
#include "../../../qrpc/src/qrpc_macro.h"

namespace QApr {

//!
//! \brief The InterfaceActuator class
//!
class Q_APR_EXPORT InterfaceActuator : public QRpc::Controller,
                                       public QAprPrivate::InterfaceAnnotations
{
    Q_OBJECT
    QAPR_DECLARE_INTERFACE_METHOD_CHECK()
    Q_ANNOTATION(InterfaceActuator, QVariantList({apiBasePath("/actuator")}))
public:
    //!
    //! \brief InterfaceActuator
    //! \param parent
    //!
    Q_INVOKABLE explicit InterfaceActuator(QObject *parent = nullptr);

    Q_ANNOTATION(health, QVariantList({opGet}))
    Q_INVOKABLE QVariant health();

};

QRPC_CONTROLLER_AUTO_REGISTER(InterfaceActuator)

} // namespace QApr
