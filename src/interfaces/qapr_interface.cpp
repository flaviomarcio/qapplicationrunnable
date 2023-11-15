#include "./qapr_interface.h"
#include "../../../qorm/src/qorm_transaction.h"
#include "../../../qorm/src/qorm_connection_pool.h"
#include "../application/qapr_application.h"
#include "../sessions/qapr_session.h"
#include "../sessions/qapr_session.h"

namespace QApr {

class InterfacePvt:public QObject
{
public:
    QRpc::Controller *parent = nullptr;
    QOrm::Transaction transaction;
    QOrm::ConnectionPool pool;
    Session session;
    bool connectionDb = true;
    bool transactionRollbackForce = false;
    explicit InterfacePvt(QRpc::Controller *parent)
        : QObject{parent},
          parent{parent},
          transaction{parent},
          pool{qAprCnn.setting()},
          session{parent}
    {
        this->transaction.lr().clear();//clear thread erros
    }

    virtual ~InterfacePvt()
    {
        transaction.rollback();
    }

    auto &credentials() { return this->session.credential(); }
};

Interface::Interface(QObject *parent)
    : QRpc::Controller(parent), QAprPrivate::InterfaceAnnotations{this}, p{new InterfacePvt{this}}
{
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
    return p->session;
}

const SessionCredential &Interface::credential()
{
    return p->credentials();
}

bool Interface::requestBeforeInvoke()
{
    auto &rq = this->rq();
    if (rq.isMethodOptions())
        return true;

    const auto &annotations = this->annotation();

    auto dbConnection = !annotations.contains(this->dbNoConnection);
    if (!dbConnection) //if no connection return true
        return true;

    auto dbReadOnly = annotations.contains(this->dbReadOnly);
    auto dbTransaction = !annotations.contains(this->dbNoTransaction);


    auto &pool = p->pool;

    QSqlDatabase connection;

    auto connectionCreate = [this, &dbTransaction, &dbReadOnly, &pool, &connection]() {
        auto dbSuccess = dbReadOnly ? (pool.getReadOnly(connection)) : (pool.get(connection));

        if (!dbSuccess) //normal connection
            return false;

        if (dbReadOnly)
            return true;

        if (!dbTransaction)
            return true;

        if (!this->setConnection(connection))
            return false;

        if (!p->transaction.transaction()) //transation
            return false;

        return true;
    };

    if (!connectionCreate()) {
        pool.finish(connection);
        this->connectionClear();
        return {};
    }

    return true;
}

bool Interface::requestAfterInvoke()
{
    if (!QRpc::Controller::requestAfterInvoke())
        return {};

    auto connection = this->connection();
    if (!connection.isValid())
        return {};

    auto finish = [this]() {
        if (p->transactionRollbackForce) {
            p->transaction.rollback(); //rollback obrigatorio
            return;
        }
        if (this->rq().co().isOK()) {
            p->transaction.commit();
            return;
        }
        p->transaction.rollback();
    };

    finish();
    connection.close();
    p->pool.finish(connection);

    return true;
}

bool Interface::connectionDb() const
{
    return p->connectionDb;
}

Interface &Interface::setConnectionDb(bool value)
{
    p->connectionDb = value;
    return *this;
}

bool Interface::transactionRollbackForce() const
{
    return p->transactionRollbackForce;
}

Interface &Interface::setTransactionRollbackForce(bool value)
{
    p->transactionRollbackForce = value;
    return *this;
}

} // namespace QApr
