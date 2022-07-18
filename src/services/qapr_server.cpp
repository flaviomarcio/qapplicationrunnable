#include "./qapr_server.h"
#include "../application/qapr_application.h"

namespace QApr {

Q_GLOBAL_STATIC(Server, staticServer)

class ServerPvt:public QObject{
public:
    explicit ServerPvt(Server*parent=nullptr):QObject{parent}
    {
        Q_UNUSED(parent)
    }
};

Server::Server(QObject *parent) : QRpc::Server{Application::i().resourceSettings(), parent}
{
    this->p=new ServerPvt{this};
}

Server &Server::instance()
{
    return *staticServer;
}

const QVariant Server::resourceSettings()
{
    return QApr::Application::i().resourceSettings();
}

}
