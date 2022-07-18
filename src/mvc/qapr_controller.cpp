#include "./qapr_controller.h"
#include "../interfaces/qapr_interface.h"
#include "../../../qrpc/src/qrpc_controller.h"
#include "../../qorm/src/qorm_macro.h"

namespace QApr {

#define dPvt()\
    auto &p = *reinterpret_cast<ControllerPvt*>(this->p)

class ControllerPvt:public QObject{
public:
    Controller*parent=nullptr;
    Interface *request=nullptr;
    QVariantHash accountModel;

    explicit ControllerPvt(Controller*parent):QObject{parent}
    {
        this->parent=parent;
    }
    Interface *req()
    {
        if(this->request)
            return this->request;

        QObject *__parent=this->parent;
        while(__parent){
            if(!__parent->metaObject()->inherits(&Interface::staticMetaObject)){
                __parent=__parent->parent();
                continue;
            }

            request=dynamic_cast<Interface*>(__parent);
            if(request)
                break;
            __parent=__parent->parent();
        }
        return this->request;
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
    return p->req();
}

Interface *Controller::irq()
{
    return p->req();
}

bool Controller::transactionRollbackForce() const
{
    if(p->req()==nullptr){
        oWarning()<<tr("Request não identificado");
        return false;
    }
    return p->req()->transactionRollbackForce();
}

void Controller::setTransactionRollbackForce(bool value)
{
    if(p->req()==nullptr){
        oWarning()<<tr("Request não identificado");
        return;
    }
    p->req()->setTransactionRollbackForce(value);
}

}
