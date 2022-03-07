#pragma once

#include <QtSql/QSqlDatabase>
#include "../../../qorm/src/qorm_object_db.h"
#include "../../../qrpc/src/qrpc_controller.h"
#include "./qapr_menu_object.h"
#include "../sessions/qapr_session.h"
#include "../application/qapr_global.h"

Q_GLOBAL_STATIC_WITH_ARGS(QVariantHash, __flags_connection_db_ignore,(QVariantHash{{"connection_db_ignore", true}}));
Q_GLOBAL_STATIC_WITH_ARGS(QVariantHash, __flags_connection_db_transaction,(QVariantHash{{"connection_db_transaction", true}}));

static const auto &flags_connection_db_ignore=*__flags_connection_db_ignore;
static const auto &flags_connection_db_transaction=*__flags_connection_db_transaction;


namespace QApr {

#define QAPR_DECLARE_IRQ()                                                          \
public:                                                                             \
    QApr::Interface*irq()                                                           \
    {                                                                               \
        if(this->___irq!=nullptr)                                                   \
            return this->___irq;                                                    \
        QObject*__parent=this->parent();                                            \
        while(__parent!=nullptr){                                                   \
            if(!__parent->metaObject()->inherits(&Interface::staticMetaObject))     \
                continue;                                                           \
            ___irq=dynamic_cast<QApr::Interface*>(__parent);                        \
            if(___irq!=nullptr)                                                     \
                break;                                                              \
            __parent=__parent->parent();                                            \
        }                                                                           \
        return this->___irq;                                                        \
    }                                                                               \
    virtual bool transactionRollbackForce() const                                   \
    {                                                                               \
        dPvt();                                                                     \
        if(this->irq()==nullptr){                                                   \
            sWarning()<<tr("Request não identificado");                             \
            return false;                                                           \
        }                                                                           \
        this->irq()->transactionRollbackForce();                                    \
        return true;                                                                \
    }                                                                               \
    virtual void setTransactionRollbackForce(bool value)                            \
    {                                                                               \
        dPvt();                                                                     \
        if(this->irq()==nullptr){                                                   \
            sWarning()<<tr("Request não identificado");                             \
            return;                                                                 \
        }                                                                           \
        this->irq()->setTransactionRollbackForce(value);                            \
    }                                                                               \
private:                                                                            \
    QApr::Interface*___irq=nullptr;                                                 \
public:

//!
//! \brief The InterfaceDatabase class
//!
//!camada criada para implementacao da parte de banco de dados
class Q_APR_EXPORT Interface : public QRpc::QRPCController
{
    Q_OBJECT
    Q_DECLARE_OBJECT()
    QORM_CONNECTION_SUPPORT()
    QRPC_DECLARE_MODULE(qsl_fy(QApr))
    QRPC_DECLARE_DESCRIPTION(qsl_fy(QApr::Interface))
public:

    //!
    //! \brief InterfaceDatabase
    //! \param parent
    //!
    Q_INVOKABLE explicit Interface(QObject *parent = nullptr);

    //!
    //! \brief ~InterfaceDatabase
    //!
    ~Interface();

    //!
    //! \brief backOfficeMenu
    //! \return
    //!
    Q_INVOKABLE virtual QVariantList backOfficeMenu() const;


    //!
    //! \brief check
    //! \return
    //!
    Q_INVOKABLE virtual QVariant check();

    //!
    //! \brief ping
    //! \return
    //!
    Q_INVOKABLE virtual QVariant ping();

    //!
    //! \brief fullCheck
    //! \return
    //!
    Q_INVOKABLE virtual QVariant fullCheck();

    //!
    //! \brief connectionsCheck
    //! \return
    //!
    Q_INVOKABLE virtual QVariant connectionsCheck();

    //!
    //! \brief businessCheck
    //! \return
    //!
    Q_INVOKABLE virtual QVariant businessCheck();

    //!
    //! \brief sessionObject
    //! \return
    //!
    virtual QApr::Session&sessionObject();

    //!
    //! \brief credential
    //! \return
    //!
    virtual const QApr::SessionCredential&credential();

    //!
    //! \brief requestBeforeInvoke
    //! \return
    //!
    virtual bool requestBeforeInvoke()override;

    //!
    //! \brief requestAfterInvoke
    //! \return
    //!
    virtual bool requestAfterInvoke()override;

    //!
    //! \brief connectionDb
    //! \return
    //!
    virtual bool connectionDb() const ;

    //!
    //! \brief setConnectionDb
    //! \param value
    //!
    virtual void setConnectionDb(bool value);

    //!
    //! \brief transactionRollbackForce
    //! \return
    //!se true mesmo com tudo correto ocorrerá o rollback da transacao
    virtual bool transactionRollbackForce() const;

    //!
    //! \brief setTransactionRollbackForce
    //! \param value
    //!
    virtual void setTransactionRollbackForce(bool value);

private:
    void *p = nullptr;
};

}
