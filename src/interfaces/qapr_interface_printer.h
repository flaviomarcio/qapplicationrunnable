#pragma once

#include "../../../qrpc/src/qrpc_macro.h"
#include "../../../qrpc/src/qrpc_controller.h"

namespace QApr {

class InterfacePrinterPvt;
class InterfacePrinter : public QRpc::Controller
{
    Q_OBJECT
    Q_ANNOTATION(InterfacePrinter, QVariantList({apiBasePath("v1/printer")}))
public:
    explicit InterfacePrinter(QObject *parent = nullptr);

    Q_ANNOTATION(printer, QVariantList({opPost}))
    Q_INVOKABLE virtual QVariant printer();
private:
    InterfacePrinterPvt *p=nullptr;
};

QRPC_CONTROLLER_AUTO_REGISTER(InterfacePrinter)

} // namespace QApr

