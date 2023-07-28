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
    virtual QUuid &id() const;

    //!
    //! \brief setUuid
    //! \param value
    //! \return
    //!
    virtual SessionCredential &setId(const QVariant &value);


    //!
    //! \brief scopeId
    //! \return
    //!
    virtual QUuid scopeId()const;
    virtual SessionCredential &setScopeId(const QVariant &uuid);

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
