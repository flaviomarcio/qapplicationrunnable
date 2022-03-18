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
    QRpc::Controller *parent = nullptr;
    Session session;

    explicit InterfaceDatabasePvt(QRpc::Controller *parent)
        : transaction{parent}, pool{QApr::Application::i().pool()}, session{parent}
    {
        this->parent = parent;
    }

    virtual ~InterfaceDatabasePvt()
    {
        transaction.rollback();
    }

    auto &credentials()
    {
        return this->session.credential();
    }
};

Interface::Interface(QObject *parent) : QRpc::Controller(parent), QAprPrivate::NotationsExtended(this)
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
    auto &rq = this->rq();
    if (rq.isMethodOptions())
        return true;

    const auto &notations=this->notation();

    auto dbConnection = !notations.contains(this->dbNoConnection);
    if(!dbConnection)//if no connection return true
        return true;

    auto dbReadOnly = notations.contains(this->dbReadOnly);
    auto dbTransaction = !notations.contains(this->dbNoTransaction);


    dPvt();
    auto &pool = p.pool;

    QSqlDatabase connection;

    auto connectionCreate=[this, &p, &dbTransaction, &dbReadOnly, &pool, &connection](){

        auto dbSuccess=dbReadOnly?(pool.getReadOnly(connection)):(pool.get(connection));

        if (!dbSuccess)//normal connection
            return false;

        if(dbReadOnly)
            return true;

        if(!dbTransaction)
            return true;

        if (!this->setConnection(connection))
            return false;

        if(!p.transaction.transaction())//transation
            return false;

        return true;
    };

    if(!connectionCreate()){
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

    dPvt();
    auto connection = this->connection();
    if (!connection.isValid())
        return {};

    auto finish=[&p, this]()
    {
        if (p.transactionRollbackForce){
            p.transaction.rollback(); //rollback obrigatorio
            return;
        }
        if (this->rq().co().isOK()){
            p.transaction.commit();
            return;
        }
        p.transaction.rollback();
    };

    finish();
    connection.close();
    p.pool.finish(connection);

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
