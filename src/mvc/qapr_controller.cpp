    #include "./qapr_controller.h"
#include "../interfaces/qapr_interface.h"
#include "../application/qapr_application.h"
#include "../../../qrpc/src/qrpc_controller.h"

namespace QApr {

#define dPvt()\
    auto &p = *reinterpret_cast<ControllerPvt*>(this->p)

class ControllerPvt{
public:
    Controller*parent=nullptr;
    Interface*request_=nullptr;
    QVariantHash accountModel;

    explicit ControllerPvt(Controller*parent)
    {
        this->parent=parent;
    }

    virtual ~ControllerPvt()
    {
    }

    Interface*request()
    {
        if(this->request_==nullptr){
            QObject*__parent=this->parent;
            while(__parent!=nullptr){
                if(__parent->metaObject()->inherits(&Interface::staticMetaObject)){
                    request_=dynamic_cast<Interface*>(__parent);
                    if(request_!=nullptr)
                        break;
                }
                __parent=__parent->parent();
            }
        }
        return this->request_;
    }
};

Controller::Controller(QObject *parent) : QOrm::Controller(parent)
{
    this->p=new ControllerPvt{this};
}

Controller::Controller(const QSqlDatabase &connection, QObject *parent) : QOrm::Controller(connection, parent)
{
    this->p=new ControllerPvt{this};
}

Controller::~Controller()
{
    dPvt();
    delete&p;
}

const QVariant Controller::resultInfo()
{
    if(this->parent()==nullptr)
        return this->lr().resultVariantInfo();

    auto interface=dynamic_cast<QRpc::Controller*>(this->parent());
    if(interface==nullptr)
        return this->lr().resultVariantInfo();

    auto &rq=interface->rq();
    if(this->lr().isNotOk())
        rq.co(this->lr().sc());
    return this->lr().resultVariantInfo();
}

Interface *Controller::interfaceRequest()
{
    dPvt();
    return p.request();
}

Interface *Controller::irq()
{
    dPvt();
    return p.request();
}

bool Controller::transactionRollbackForce() const
{
    dPvt();
    if(p.request()==nullptr){
        sWarning()<<tr("Request não identificado");
        return false;
    }
    return p.request()->transactionRollbackForce();
}

void Controller::setTransactionRollbackForce(bool value)
{
    dPvt();
    if(p.request()==nullptr){
        sWarning()<<tr("Request não identificado");
        return;
    }
    p.request()->setTransactionRollbackForce(value);
}

}
