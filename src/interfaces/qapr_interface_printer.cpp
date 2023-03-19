#include "qapr_interface_printer.h"
#include <QtReforce/QStm>
#include <QtReforce/QApr>
#include <QtReforce/QRpc>

namespace QApr {

class InterfacePrinterPvt:public QObject{
public:
    explicit InterfacePrinterPvt(InterfacePrinter *parent):QObject{parent}, requestService{parent}
    {
        this->parent=parent;
    }

    QRpc::Request &req()
    {
        static const auto __settings="printservice";
        auto &manager=QApr::Application::i().manager();
        auto &settingAcl=manager.setting(__settings);
        requestSession=settingAcl;
        auto headers=parent->request().requestHeader();
        this->requestSession.header().setRawHeader(headers);
        return this->requestSession;
    }
private:
    InterfacePrinter *parent=nullptr;
    QRpc::Request requestService, requestSession;
};

InterfacePrinter::InterfacePrinter(QObject *parent) : QRpc::Controller{parent}
{
    this->p=new InterfacePrinterPvt{this};
}

QVariant InterfacePrinter::execute()
{
    QApr::Controller controller{this};
    static const auto __path="/v1/service/maker/execute";
    QRPC_V_SET_BODY(body);

    auto &req=p->req();
    req.call(QRpc::Post, __path, body);

    if(!req.response().isOk()){
        auto &resultInfo=controller.lr().resultInfo();
        resultInfo.setSuccess(false);
        resultInfo.setMessages(tr("Falha no serviço de impressão"));
        return controller.resultInfo();
    }

    return req.response().body();
}

} // namespace QApr


