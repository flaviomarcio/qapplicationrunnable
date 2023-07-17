#include "qapr_interface_printer.h"
#include "../../../qrpc/src/qrpc_request.h"
#include "../mvc/qapr_controller.h"
#include "../application/qapr_application.h"

namespace QApr {

class InterfacePrinterPvt:public QObject{
public:
    InterfacePrinter *parent=nullptr;
    QRpc::Request requestService, requestSession;
    explicit InterfacePrinterPvt(InterfacePrinter *parent):QObject{parent}, parent{parent}, requestService{parent}
    {
    }

    QRpc::Request &req()
    {
        static const auto __settings="printservice";
        auto &manager=qAprApp.manager();
        auto settingAcl=manager.setting(__settings);
        requestSession=settingAcl;
        auto headers=parent->request().requestHeader();
        this->requestSession.header().setRawHeader(headers);
        return this->requestSession;
    }
};

InterfacePrinter::InterfacePrinter(QObject *parent) : QRpc::Controller{parent}, p{new InterfacePrinterPvt{this}}
{
}

QVariant InterfacePrinter::execute()
{
    QApr::Controller controller{this};
    static const auto __path="/service/maker/execute";
    QRPC_V_SET_BODY(body);

    auto &req=p->req();
    req.call(QRpc::Types::Post, __path, body);

    if(!req.response().isOk()){
        auto &resultInfo=controller.lr().resultInfo();
        resultInfo.setSuccess(false);
        resultInfo.setMessages(tr("Falha no serviço de impressão"));
        return controller.resultInfo();
    }

    return req.response().body();
}

} // namespace QApr


