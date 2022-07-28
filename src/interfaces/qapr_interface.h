#pragma once

#include <QSqlDatabase>
#include <QApiDoc>
#include <QMfe>
#include "../../../qrpc/src/qrpc_controller.h"
#include "../../qorm/src/qorm_macro.h"
#include "../application/qapr_global.h"
#include "../sessions/qapr_session.h"
#include "./qapr_interface_notations.h"
//#include "../../../qorm/src/qorm_object_db.h"
//#include "./qapr_menu_object.h"

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
        if (this->irq() == nullptr) { \
            oWarning() << tr("Request não identificado"); \
            return false; \
        } \
        this->irq()->transactionRollbackForce(); \
        return true; \
    } \
    virtual void setTransactionRollbackForce(bool value) \
    { \
        if (this->irq() == nullptr) { \
            oWarning() << tr("Request não identificado"); \
            return; \
        } \
        this->irq()->setTransactionRollbackForce(value); \
    } \
\
private: \
    Interface *___irq = nullptr; \
\
public:


namespace QApr {

class InterfacePvt;
//!
//! \brief The InterfaceDatabase class
//!
//!camada criada para implementacao da parte de banco de dados
class Q_APR_EXPORT Interface : public QRpc::Controller, public QAprPrivate::InterfaceNotations
{
    Q_OBJECT
    QORM_CONNECTION_SUPPORT()
public:
    Q_API_DOC_INFO()
    {
        document->host(QStringLiteral("localhost"))
            .basePath(this->basePath())
            .consumes(QStringLiteral("application/json"))
            .produces(QStringLiteral("application/json"))
            .schemes(stpsHttp);

        document->info()
            .title(QStringLiteral("QApr API Interface"))
            .version(QStringLiteral("1.0.0"))
            .termsOfService(QStringLiteral("http://www.apache.org/licenses/LICENSE-2.0.txt"))
            .description(QStringLiteral("Basic interface object for implementation in application"));

        document->info()
            .contact()
            .name(QStringLiteral("Flavio Portela"))
            .email(QStringLiteral("fmspx@hotmail.com"))
            .url(QStringLiteral("https://github.com/flaviomarcio/qtreforce-sdk"));

        document->info()
            .license()
            .name(QStringLiteral("Apache License - Version 2.0, January 2004"))
            .url(QStringLiteral("http://www.apache.org/licenses/LICENSE-2.0"));
    }

public:
    //!
    //! \brief InterfaceDatabase
    //! \param parent
    //!
    Q_INVOKABLE explicit Interface(QObject *parent = nullptr);

    //!
    //! \brief backOfficeMenu
    //! \return
    //!
    Q_INVOKABLE virtual QVariantList backOfficeMenu() const;
    Q_API_DOC_PATH(backOfficeMenu)
    {
        path->operation(sptoGet)
            .operationId({})
            .description(QStringLiteral("Menus for currenty API"))
            .responses(QApiResponse().statusCode(200).examples(
                QVariantHash{{QStringLiteral("response"), this->backOfficeMenu()}}));
    }

    //!
    //! \brief check
    //! \return
    //!
    Q_ANNOTATION(check, QVariantList({opGet, dbNoConnection, healtCheck, rqSecurityIgnore}))
    Q_INVOKABLE virtual QVariant check();
    Q_API_DOC_PATH(check)
    {
        path->operation(sptoGet)
            .operationId({})
            .description(QStringLiteral("Simple check method for tests"))
            .responses(QApiResponse().statusCode(200));
    }

    //!
    //! \brief ping
    //! \return
    //!
    Q_INVOKABLE virtual QVariant ping();
    Q_ANNOTATION(ping, QVariantList({opGet, dbNoConnection, healtCheck, rqSecurityIgnore}))
    Q_API_DOC_PATH(ping)
    {
        path->operation(sptoGet)
            .operationId({})
            .description(QStringLiteral("Simple method check with response body"))
            .responses(
                QApiResponse().statusCode(200).examples(QVariantHash{{QStringLiteral("response"), this->ping()}}));
    }

    //!
    //! \brief fullCheck
    //! \return
    //!
    Q_ANNOTATION(fullCheck, {healtCheck})
    Q_INVOKABLE virtual QVariant fullCheck();
    Q_API_DOC_PATH(fullCheck)
    {
        path->operation(sptoGet)
            .operationId({})
            .description(QStringLiteral("Execute healt check"))
            .responses(
                QApiResponse().statusCode(200).examples(QVariantHash{{QStringLiteral("response"), this->fullCheck()}}));
    }

    //!
    //! \brief connectionsCheck
    //! \return
    //!
    Q_ANNOTATION(connectionsCheck, {healtCheck})
    Q_INVOKABLE virtual QVariant connectionsCheck();
    Q_API_DOC_PATH(connectionsCheck)
    {
        path->operation(sptoGet)
            .operationId({})
            .description(QStringLiteral("Implements connection database check"))
            .responses(QApiResponse().statusCode(200).examples(
                QVariantHash{{QStringLiteral("response"), this->connectionsCheck()}}));
    }

    //!
    //! \brief businessCheck
    //! \return
    //!
    Q_ANNOTATION(businessCheck, {healtCheck})
    Q_INVOKABLE virtual QVariant businessCheck();
    Q_API_DOC_PATH(businessCheck)
    {
        path->operation(sptoGet)
            .operationId({})
            .description(QStringLiteral("Implements business check"))
            .responses(QApiResponse().statusCode(200).examples(
                QVariantHash{{QStringLiteral("response"), this->businessCheck()}}));
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
    InterfacePvt *p = nullptr;
};

} // namespace QApr
