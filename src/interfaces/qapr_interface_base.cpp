#include "./qapr_interface_base.h"
#include "./qapr_application.h"

namespace QApr {


#define dPvt()\
    auto&p =*reinterpret_cast<InterfacePvt*>(this->p)

class InterfacePvt{
public:
    VariantUtil vu;
    QApr::InterfaceBase*parent=nullptr;
    explicit InterfacePvt(QApr::InterfaceBase*parent){
        this->parent=parent;
    }

    virtual ~InterfacePvt(){
    }
};

InterfaceBase::InterfaceBase(QObject *parent):QRpc::QRPCInterfaceCheck(parent)
{
    this->p = new InterfacePvt(this);
}

InterfaceBase::~InterfaceBase()
{
    dPvt();
    delete&p;
}

VariantUtil &InterfaceBase::vu()
{
    dPvt();
    return p.vu;
}

QVariant InterfaceBase::check()
{
    QRPC_RETURN_VARIANT();
}

QVariant InterfaceBase::ping()
{
    this->rq().co().setOK();
    return QDateTime::currentDateTime();
}

QVariant InterfaceBase::fullCheck()
{
    this->rq().co().setOK();
    return QVariant();
}

QVariant InterfaceBase::connectionsCheck()
{
    this->rq().co().setOK();
    return QVariant();
}

QVariant InterfaceBase::businessCheck()
{
    this->rq().co().setNotImplemented();
    return QVariant();
}

}
