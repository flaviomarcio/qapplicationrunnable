#include "./qapr_interface_authorization.h"
#include "./qapr_application.h"
#include "./qapr_session.h"
#include "./qorm_transaction.h"


namespace QApr {


#define dPvt()\
    auto&p =*reinterpret_cast<InterfaceAuthorizationPvt*>(this->p)

class InterfaceAuthorizationPvt{
public:
    Session session;
    InterfaceAuthorization*parent=nullptr;

    explicit InterfaceAuthorizationPvt(InterfaceAuthorization*parent): session(parent){
        this->parent=parent;
    }

    virtual ~InterfaceAuthorizationPvt(){
    }

    auto&credentials(){
        return this->session.credential();
    }

};

InterfaceAuthorization::InterfaceAuthorization(QObject *parent):QApr::InterfaceDatabase(parent)
{
    this->p = new InterfaceAuthorizationPvt(this);
}

InterfaceAuthorization::~InterfaceAuthorization()
{
    dPvt();
    delete&p;
}

Session &InterfaceAuthorization::sessionObject()
{
    dPvt();
    return p.session;
}

const SessionCredential &InterfaceAuthorization::credential()
{
    dPvt();
    return p.credentials();
}

const QUuid InterfaceAuthorization::domain_uuid()
{
    dPvt();
    return p.credentials().domain_uuid();
}

const QUuid InterfaceAuthorization::service_uuid()
{
    dPvt();
    return p.credentials().service_uuid();
}

const QUuid InterfaceAuthorization::account_uuid()
{
    dPvt();
    return p.credentials().account_uuid();
}

const QUuid InterfaceAuthorization::session_uuid()
{
    dPvt();
    return p.credentials().session_uuid();
}

const QVariant InterfaceAuthorization::token() const
{
    dPvt();
    return p.credentials().token();
}

}
