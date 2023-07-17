#include "./qapr_server.h"
#include "../application/qapr_application.h"

namespace QApr {

Q_GLOBAL_STATIC(Server, staticServer)

Server::Server(QObject *parent) : QRpc::Server{parent}
{
    const auto &vSettings=qAprApp.manager().settingBody();
    this->load(vSettings);
}

Server &Server::i()
{
    return *staticServer;
}

}
