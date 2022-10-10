#include "qapr_interface_printer.h"
#include <QtReforce/QStm>
#include <QtReforce/QApr>
#include <QtReforce/QRpc>

namespace QApr {

static const auto _printer="";



class InterfacePrinterPvt:public QObject{
public:
    explicit InterfacePrinterPvt(InterfacePrinter *parent):QObject{parent}, requestService{parent}
    {
        this->parent=parent;
    }

    QRpc::Request &req()
    {
        auto &manager=QApr::Application::i().manager();
        auto &settingAcl=manager.setting(_printer);
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

QVariant InterfacePrinter::printer()
{
    QRPC_V_SET_BODY(body);
    auto &req=p->req();
    req.call(QRpc::Post, body);

    if(req.response().isOk())
        return {};

    return req.response().body();
}

} // namespace QApr


