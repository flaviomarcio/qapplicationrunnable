#pragma once

#include <QUuid>
#include <QVariantHash>
#include "../application/qapr_global.h"

namespace QApr {
class CredentialsPvt;
//!
//! \brief The SessionCredential class
//!
class Q_APR_EXPORT SessionCredential : public QObject
{
    Q_OBJECT
public:
    //!
    //! \brief SessionCredential
    //! \param parent
    //!
    Q_INVOKABLE explicit SessionCredential(QObject *parent = nullptr);

    //!
    //! \brief body
    //! \return
    //!full body of SessionCredential
    virtual QVariantHash &body()const;

    //!
    //! \brief setBody
    //! \param value
    //! \return
    //!
    virtual SessionCredential &setBody(const QVariantHash &value);

    //!
    //! \brief toVariant
    //! \return
    //!return QVariantHash with credential information
    virtual QVariantHash toVariant()const;

    //!
    //! \brief isValid
    //! \return
    //!validate values SessionCredential
    virtual bool isValid()const;

    //!
    //! \brief permits
    //! \return
    //!
    virtual QVariantHash &permits() const;

    //!
    //! \brief setPermits
    //! \param permits
    //! \return
    //!
    virtual SessionCredential &setPermits(const QVariantHash &permits);

    //!
    //! \brief uuid
    //! \return
    //!generic uuid for identification
    virtual QUuid &uuid() const;

    //!
    //! \brief setUuid
    //! \param value
    //! \return
    //!
    virtual SessionCredential &setUuid(const QUuid &value);


    //!
    //! \brief domainUuid
    //! \return
    //!isolation level for systems
    virtual QUuid domainUuid()const;
    virtual SessionCredential &setDomainUuid(const QVariant &uuid);

    //!
    //! \brief accountUuid
    //! \return
    //!account identification
    virtual QUuid accountUuid()const;
    virtual SessionCredential &set_account_uuid(const QVariant &uuid);

    //!
    //! \brief sessionUuid
    //! \return
    //!
    virtual QUuid sessionUuid()const;

    //!
    //! \brief setSessionUuid
    //! \param uuid
    //! \return
    //!
    virtual SessionCredential &setSessionUuid(const QVariant &uuid);

    //!
    //! \brief serviceUuid
    //! \return
    //!service identification
    virtual QUuid serviceUuid()const;

    //!
    //! \brief setServiceUuid
    //! \param uuid
    //! \return
    //!
    virtual SessionCredential &setServiceUuid(const QVariant &uuid);

    //!
    //! \brief profile
    //! \return
    //! account body
    virtual QVariant profile()const;

    //!
    //! \brief setProfile
    //! \param value
    //! \return
    //!
    virtual SessionCredential &setProfile(const QVariant &value);

    //!
    //! \brief session
    //! \return
    //!session body of account
    virtual QVariant session()const;

    //!
    //! \brief setSession
    //! \param value
    //! \return
    //!
    virtual SessionCredential &setSession(const QVariant &value);

    //!
    //! \brief token
    //! \return
    //!token body
    virtual QVariant token()const;

    //!
    //! \brief setToken
    //! \param value
    //! \return
    //!set token payload
    virtual SessionCredential &setToken(const QVariant &value);

private:
    CredentialsPvt *p=nullptr;
};

}
