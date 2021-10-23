#pragma once

#include "./qapr_interface_base.h"
#include <QSqlDatabase>

const static QVariantHash flags_connection_db_ignore{{qsl("connection_db_ignore"), true}};
const static QVariantHash flags_connection_db_transaction{{qsl("connection_db_transaction"), true}};

namespace QApr {

/**
 * @brief The InterfaceDatabase class
 *
 * camada criada para implementacao da parte de banco de dados
 */
class Q_APR_EXPORT InterfaceDatabase : public QApr::InterfaceBase
{
    Q_OBJECT
    Q_DECLARE_OBJECT()
    QORM_CONNECTION_SUPPORT
public:
    /**
     * @brief InterfaceDatabase
     * @param parent
     */
    Q_INVOKABLE explicit InterfaceDatabase(QObject *parent = nullptr);

    /**
     * @brief ~InterfaceDatabase
     */
    Q_INVOKABLE ~InterfaceDatabase();

    /**
     * @brief requestBeforeInvoke
     * @return
     */
    virtual bool requestBeforeInvoke();

    /**
     * @brief requestAfterInvoke
     * @return
     */
    virtual bool requestAfterInvoke();

    /**
     * @brief connectionDb
     * @return
     */
    virtual bool connectionDb() const;
    virtual void setConnectionDb(bool value);

    /**
     * @brief transactionRollbackForce
     * @return
     *
     * se true mesmo com tudo correto ocorrerá o rollback da transacao
     */
    virtual bool transactionRollbackForce() const;
    virtual void setTransactionRollbackForce(bool value);

private:
    void *p = nullptr;
};

} // namespace QApr
