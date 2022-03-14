#include "./qapr_interface_maintence.h"
#include "../../../qrpc/src/qrpc_server.h"

namespace QApr {

InterfaceMaintence::InterfaceMaintence(QObject *parent) : QRpc::Controller(parent)
{
    this->setEnabled(true);
}

InterfaceMaintence::~InterfaceMaintence()
{

}

QVariant InterfaceMaintence::serverStart()
{
    this->server()->interfacesStart();
    this->rq().co().setOK();
    return {};
}

QVariant InterfaceMaintence::serverStop()
{
    this->server()->interfacesStop();
    this->rq().co().setOK();
    return {};
}

QVariant InterfaceMaintence::serverRestart()
{
    this->server()->start();
    this->server()->stop();
    this->rq().co().setOK();
    return {};
}

QVariant InterfaceMaintence::servicesCheck()
{
    this->server()->start();
    this->server()->stop();
    this->rq().co().setOK();
    return {};
}

QVariant InterfaceMaintence::applicationQuit()
{
    this->server()->stop();
    qApp->quit();
    this->rq().co().setOK();
    return {};
}

} // namespace QRpc
