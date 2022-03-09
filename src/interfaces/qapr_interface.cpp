#include "./qapr_interface.h"
#include "../application/qapr_application.h"
#include "../sessions/qapr_session.h"
#include "../../../qorm/src/qorm_transaction.h"

namespace QApr {


#define dPvt() auto &p = *reinterpret_cast<InterfaceDatabasePvt *>(this->p)

class InterfaceDatabasePvt
{
public:
    bool connectionDb = true;
    bool transactionRollbackForce = false;
    QOrm::Transaction transaction;
    QOrm::ConnectionPool pool;
    QRpc::QRPCController *parent = nullptr;
    Session session;

    explicit InterfaceDatabasePvt(QRpc::QRPCController *parent)
        : transaction(parent), pool(QApr::Application::i().pool()), session(parent)
    {
        this->parent = parent;
    }

    virtual ~InterfaceDatabasePvt() { transaction.rollback(); }

    auto &credentials() { return this->session.credential(); }
};

Interface::Interface(QObject *parent) : QRpc::QRPCController(parent)
{
    this->p = new InterfaceDatabasePvt(this);
}

Interface::~Interface()
{
    dPvt();
    delete &p;
}

QVariantList Interface::backOfficeMenu() const
{
    return {};
}

QVariant Interface::check()
{
    this->rq().co().setOK();
    return {};
}

QVariant Interface::ping()
{
    this->rq().co().setOK();
    return QDateTime::currentDateTime();
}

QVariant Interface::fullCheck()
{
    this->rq().co().setOK();
    return {};
}

QVariant Interface::connectionsCheck()
{
    this->rq().co().setOK();
    return {};
}

QVariant Interface::businessCheck()
{
    this->rq().co().setNotImplemented();
    return {};
}

Session &Interface::sessionObject()
{
    dPvt();
    return p.session;
}

const SessionCredential &Interface::credential()
{
    dPvt();
    return p.credentials();
}

bool Interface::requestBeforeInvoke()
{
    if (!QRpc::QRPCController::requestBeforeInvoke())
        return false;


    auto &rq = this->rq();
    if (rq.isMethodOptions())
        return true;

    const auto requestPath = rq.requestPath();
    const auto flg = this->routeFlags(requestPath);
    const auto connection_db_ignore = flg.value(qsl("connection_db_ignore"))
                                          .toBool(); //permitir nao criar conexao
    auto flags_connection_db_transaction
        = flg.value(qsl("connection_db_transaction"))
              .toBool(); //permite criar transacao em qualquer metodo logo get e options nao criam transacao
    if (connection_db_ignore)
        return true;

    dPvt();
    auto &pool = p.pool;

    QSqlDatabase connection;
    if (flags_connection_db_transaction) {
        //conexao normal que tera transacao iniciada
        if (!pool.get(connection))
            return false;
    } else if (!(rq.isMethodGet() || rq.isMethodOptions())) {
        flags_connection_db_transaction = true;
        if (!pool.get(connection))
            return false;
    } else {
        if (!pool.getReadOnly(connection))
            return false;
    }

    if (!this->setConnection(connection)) {
#ifdef QAPR_LOG
        sWarning() << qbl("invalid database connection");
#endif
        rq.co().setInternalServerError();
        return false;
    }

    if (!flags_connection_db_transaction) {
        return true;
    }

    return p.transaction.transaction();
}

bool Interface::requestAfterInvoke()
{
    if (!QRpc::QRPCController::requestAfterInvoke())
        return false;

    dPvt();
    auto db = this->connection();
    if (!db.isOpen())
        return false;

    if (p.transactionRollbackForce)
        p.transaction.rollback(); //rollback obrigatorio
    else if (this->rq().co().isOK())
        p.transaction.commit();
    else
        p.transaction.rollback();

    db.close();
    p.pool.finish(db);

    return true;
}

bool Interface::connectionDb() const
{
    dPvt();
    return p.connectionDb;
}

Interface &Interface::setConnectionDb(bool value)
{
    dPvt();
    p.connectionDb = value;
    return *this;
}

bool Interface::transactionRollbackForce() const
{
    dPvt();
    return p.transactionRollbackForce;
}

Interface &Interface::setTransactionRollbackForce(bool value)
{
    dPvt();
    p.transactionRollbackForce = value;
    return *this;
}

}
