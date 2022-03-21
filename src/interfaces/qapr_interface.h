#pragma once

#include "../../../qorm/src/qorm_object_db.h"
#include "../../../qrpc/src/qrpc_controller.h"
#include "../application/qapr_global.h"
#include "../sessions/qapr_session.h"
#include "./qapr_interface_notations.h"
#include "./qapr_menu_object.h"
#include <QtReforce/QApiDoc>
#include <QtSql/QSqlDatabase>

namespace QApr {

#define QAPR_DECLARE_IRQ() \
public: \
    Interface *irq() \
    { \
        if (this->___irq != nullptr) \
            return this->___irq; \
        QObject *__parent = this->parent(); \
        while (__parent != nullptr) { \
            if (!__parent->metaObject()->inherits(&Interface::staticMetaObject)) \
                continue; \
            ___irq = dynamic_cast<Interface *>(__parent); \
            if (___irq != nullptr) \
                break; \
            __parent = __parent->parent(); \
        } \
        return this->___irq; \
    } \
    virtual bool transactionRollbackForce() const \
    { \
        dPvt(); \
        if (this->irq() == nullptr) { \
            sWarning() << tr("Request não identificado"); \
            return false; \
        } \
        this->irq()->transactionRollbackForce(); \
        return true; \
    } \
    virtual void setTransactionRollbackForce(bool value) \
    { \
        dPvt(); \
        if (this->irq() == nullptr) { \
            sWarning() << tr("Request não identificado"); \
            return; \
        } \
        this->irq()->setTransactionRollbackForce(value); \
    } \
\
private: \
    Interface *___irq = nullptr; \
\
public:

//!
//! \brief The InterfaceDatabase class
//!
//!camada criada para implementacao da parte de banco de dados
class Q_APR_EXPORT Interface : public QRpc::Controller, public QAprPrivate::InterfaceNotations
{
    Q_OBJECT
    Q_DECLARE_OBJECT()
    QORM_CONNECTION_SUPPORT()
    QRPC_DECLARE_MODULE(qsl_fy(QApr))
    QRPC_DECLARE_DESCRIPTION(qsl_fy(Interface))
public:
    Q_API_DOC_INFO()
    {
        document->host(qsl("localhost"))
            .basePath(this->basePath())
            .consumes(qsl("application/json"))
            .produces(qsl("application/json"))
            .schemes(stpsHttp);

        document->info()
            .title(qsl("QApr API Interface"))
            .version(qsl("1.0.0"))
            .termsOfService(qsl("http://www.apache.org/licenses/LICENSE-2.0.txt"))
            .description(qsl("Basic interface object for implementation in application"));

        document->info()
            .contact()
            .name(qsl("Flavio Portela"))
            .email(qsl("fmspx@hotmail.com"))
            .url(qsl("https://github.com/flaviomarcio/qtreforce-sdk"));

        document->info()
            .license()
            .name(qsl("Apache License - Version 2.0, January 2004"))
            .url(qsl("http://www.apache.org/licenses/LICENSE-2.0"));
    }

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
    Q_API_DOC_PATH(backOfficeMenu)
    {
        path->operation(sptoGet)
            .operationId(qsl_null)
            .description(qsl("Menus for currenty API"))
            .responses(QApiResponse().statusCode(200).examples(
                qvh{{qsl("response"), this->backOfficeMenu()}}));
    }

    //!
    //! \brief check
    //! \return
    //!
    Q_NOTATION(check, qvl({opGet, dbNoConnection, healtCheck, rqSecurityIgnore}))
    Q_INVOKABLE virtual QVariant check();
    Q_API_DOC_PATH(check)
    {
        path->operation(sptoGet)
            .operationId(qsl_null)
            .description(qsl("Simple check method for tests"))
            .responses(QApiResponse().statusCode(200));
    }

    //!
    //! \brief ping
    //! \return
    //!
    Q_INVOKABLE virtual QVariant ping();
    Q_NOTATION(ping, qvl({opGet, dbNoConnection, healtCheck, rqSecurityIgnore}))
    Q_API_DOC_PATH(ping)
    {
        path->operation(sptoGet)
            .operationId(qsl_null)
            .description(qsl("Simple method check with response body"))
            .responses(
                QApiResponse().statusCode(200).examples(qvh{{qsl("response"), this->ping()}}));
    }

    //!
    //! \brief fullCheck
    //! \return
    //!
    Q_NOTATION(fullCheck, {healtCheck})
    Q_INVOKABLE virtual QVariant fullCheck();
    Q_API_DOC_PATH(fullCheck)
    {
        path->operation(sptoGet)
            .operationId(qsl_null)
            .description(qsl("Execute healt check"))
            .responses(
                QApiResponse().statusCode(200).examples(qvh{{qsl("response"), this->fullCheck()}}));
    }

    //!
    //! \brief connectionsCheck
    //! \return
    //!
    Q_NOTATION(connectionsCheck, {healtCheck})
    Q_INVOKABLE virtual QVariant connectionsCheck();
    Q_API_DOC_PATH(connectionsCheck)
    {
        path->operation(sptoGet)
            .operationId(qsl_null)
            .description(qsl("Implements connection database check"))
            .responses(QApiResponse().statusCode(200).examples(
                qvh{{qsl("response"), this->connectionsCheck()}}));
    }

    //!
    //! \brief businessCheck
    //! \return
    //!
    Q_NOTATION(businessCheck, {healtCheck})
    Q_INVOKABLE virtual QVariant businessCheck();
    Q_API_DOC_PATH(businessCheck)
    {
        path->operation(sptoGet)
            .operationId(qsl_null)
            .description(qsl("Implements business check"))
            .responses(QApiResponse().statusCode(200).examples(
                qvh{{qsl("response"), this->businessCheck()}}));
    }

    //!
    //! \brief sessionObject
    //! \return
    //!
    virtual Session &sessionObject();

    //!
    //! \brief credential
    //! \return
    //!
    virtual const SessionCredential &credential();

    //!
    //! \brief requestBeforeInvoke
    //! \return
    //!
    virtual bool requestBeforeInvoke() override;

    //!
    //! \brief requestAfterInvoke
    //! \return
    //!
    virtual bool requestAfterInvoke() override;

    //!
    //! \brief connectionDb
    //! \return
    //!
    virtual bool connectionDb() const;

    //!
    //! \brief setConnectionDb
    //! \param value
    //!
    virtual Interface &setConnectionDb(bool value);

    //!
    //! \brief transactionRollbackForce
    //! \return
    //!se true mesmo com tudo correto ocorrerá o rollback da transacao
    virtual bool transactionRollbackForce() const;

    //!
    //! \brief setTransactionRollbackForce
    //! \param value
    //!
    virtual Interface &setTransactionRollbackForce(bool value);

private:
    void *p = nullptr;
};

} // namespace QApr
