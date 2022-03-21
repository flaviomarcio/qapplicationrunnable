#include "./qapr_interface_backoffice.h"
#include "../../../qorm/src/qorm_transaction.h"
#include "../application/qapr_application.h"
#include "../mvc/qapr_controller_backoffice.h"

namespace QApr {

#define dPvt() auto &p = *reinterpret_cast<InterfaceBackOfficePvt *>(this->p)

class InterfaceBackOfficePvt
{
public:
    bool connectionDb = true;
    bool transactionRollbackForce = false;
    QOrm::Transaction transaction;
    QRpc::Controller *parent = nullptr;

    explicit InterfaceBackOfficePvt(QRpc::Controller *parent) : transaction(parent)
    {
        this->parent = parent;
    }

    virtual ~InterfaceBackOfficePvt() { transaction.rollback(); }
};

InterfaceBackOffice::InterfaceBackOffice(QObject *parent) : QApr::Interface(parent)
{
    this->p = new InterfaceBackOfficePvt{this};
}

InterfaceBackOffice::~InterfaceBackOffice()
{
    dPvt();
    delete &p;
}

const QVariant InterfaceBackOffice::sessionAccount()
{
    QRPC_METHOD_CHECK_GET();
    ControllerBackOffice controller(this);
    if (!controller.sessionAccount())
        this->rq().co(controller.lr().sc());
    else
        return controller.lr().resultVariant();
    QRPC_RETURN_VARIANT();
}

const QVariant InterfaceBackOffice::enviroment()
{
    QRPC_METHOD_CHECK_GET();
    ControllerBackOffice controller(this);
    if (!controller.enviroment())
        this->rq().co(controller.lr().sc());
    else
        return controller.lr().resultVariant();
    QRPC_RETURN_VARIANT();
}

const QVariant InterfaceBackOffice::menu()
{
    QRPC_METHOD_CHECK_GET();
    ControllerBackOffice controller(this);
    if (!controller.rootObject())
        this->rq().co(controller.lr().sc());
    else
        return controller.lr().resultVariant();
    QRPC_RETURN_VARIANT();
}

const QVariant InterfaceBackOffice::menuInfo()
{
    QRPC_METHOD_CHECK_GET();
    QRPC_V_SET_BODY_HASH(body);
    ControllerBackOffice controller(this);
    if (!controller.menuInfo(body))
        this->rq().co(controller.lr().sc());
    else
        return controller.lr().resultVariant();
    QRPC_RETURN_VARIANT();
}

const QVariant InterfaceBackOffice::rootObject()
{
    QRPC_METHOD_CHECK_GET();
    ControllerBackOffice controller(this);
    if (!controller.rootObject())
        this->rq().co(controller.lr().sc());
    else
        return controller.lr().resultVariant();
    QRPC_RETURN_VARIANT();
}

} // namespace QApr
