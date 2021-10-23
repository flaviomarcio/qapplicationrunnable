#include "./qapr_controller.h"
#include "./qapr_interface_authorization.h"
#include "./qapr_application.h"

namespace QApr {

#define dPvt()\
    auto&p = *reinterpret_cast<ControllerPvt*>(this->p)

class ControllerPvt{
public:
    Controller*parent=nullptr;
    InterfaceAuthorization*request_=nullptr;
    QVariantHash accountModel;
    explicit ControllerPvt(Controller*parent){
        this->parent=parent;
    }
    virtual ~ControllerPvt(){
    }

    InterfaceAuthorization*request(){
        if(this->request_==nullptr){
            QObject*__parent=this->parent;
            while(__parent!=nullptr){
                if(__parent->metaObject()->inherits(&InterfaceBase::staticMetaObject)){
                    request_=dynamic_cast<InterfaceAuthorization*>(__parent);
                    if(request_!=nullptr){
                        break;
                    }
                }
                __parent=__parent->parent();
            }
        }
        return this->request_;
    }
};

Controller::Controller(QObject *parent) : QOrm::Controller(parent)
{
    this->p=new ControllerPvt(this);
}

Controller::~Controller()
{
    dPvt();
    delete&p;
}

InterfaceAuthorization *Controller::interfaceRequest()
{
    dPvt();
    return p.request();
}

InterfaceAuthorization *Controller::irq()
{
    dPvt();
    return p.request();
}

bool Controller::transactionRollbackForce() const
{
    dPvt();
    if(p.request()==nullptr)
        sWarning()<<tr("Request não identificado");
    else
        return p.request()->transactionRollbackForce();
    return false;
}

void Controller::setTransactionRollbackForce(bool value)
{
    dPvt();
    if(p.request()==nullptr)
        sWarning()<<tr("Request não identificado");
    else
        p.request()->setTransactionRollbackForce(value);
}

}
