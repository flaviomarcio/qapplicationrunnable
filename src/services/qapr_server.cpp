#include "./qapr_server.h"
#include "../application/qapr_application.h"

namespace QApr {

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

const QVariant Server::resourceSettings()
{
    return QApr::Application::i().resourceSettings();
}

}
