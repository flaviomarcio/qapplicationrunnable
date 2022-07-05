#include "./qapr_session_credential.h"
#include "../../../qstm/src/qstm_util_variant.h"

#define dPvt()\
    auto &p = *reinterpret_cast<CredentialsPvt*>(this->p)

namespace QApr {

class CredentialsPvt:public QObject{
public:
    SessionCredential*parent=nullptr;
    QUuid uuid;
    QVariantHash permits;
    QVariantHash body;
    explicit CredentialsPvt(SessionCredential*parent):QObject{parent}
    {
        this->parent=parent;
    }
};


SessionCredential::SessionCredential(QObject *parent) : QObject{parent}
{
    this->p=new CredentialsPvt{this};
}

QVariantHash SessionCredential::toVariant() const
{
    QVariantHash vHash;
    vHash[qsl_fy(session_uuid)]=this->session_uuid();
    vHash[qsl_fy(account_uuid)]=this->account_uuid();
    vHash[qsl_fy(profile)]=this->profile();
    vHash[qsl_fy(session)]=this->session();
    vHash[qsl_fy(permits)]=this->permits();
    return vHash;
}

bool SessionCredential::isValid() const
{
    auto &credentials=*this;
    if(!credentials.service_uuid().isNull())
        return true;

    if(!credentials.account_uuid().isNull() && !credentials.session_uuid().isNull())
        return true;

    return false;
}

QVariantHash&SessionCredential::permits()const
{
    return p->permits;
}

SessionCredential&SessionCredential::setPermits(const QVariantHash &permits)
{
    p->permits = permits;
    return*this;
}

QUuid&SessionCredential::uuid() const
{
    return p->uuid;
}

SessionCredential&SessionCredential::setUuid(const QUuid &value)
{
    p->uuid = value;
    return*this;
}

QUuid SessionCredential::domain_uuid() const
{
    Q_DECLARE_VU;
    return p->body[qsl_fy(domain_uuid)].toUuid();
}

SessionCredential &SessionCredential::set_domain_uuid(const QVariant &uuid)
{
    Q_DECLARE_VU;
    p->body[qsl_fy(domain_uuid)]=vu.toUuid(uuid);
    return*this;
}

QUuid SessionCredential::account_uuid() const
{
    Q_DECLARE_VU;
    return p->body.value(qsl_fy(account_uuid)).toUuid();
}

SessionCredential &SessionCredential::set_account_uuid(const QVariant &uuid)
{
    Q_DECLARE_VU;
    p->body[qsl_fy(account_uuid)]=vu.toUuid(uuid);
    return*this;
}

QUuid SessionCredential::session_uuid() const
{
    Q_DECLARE_VU;
    return p->body.value(qsl_fy(session_uuid)).toUuid();
}

SessionCredential &SessionCredential::set_session_uuid(const QVariant &uuid)
{
    Q_DECLARE_VU;
    p->body[qsl_fy(session_uuid)]=vu.toUuid(uuid);
    return*this;
}

QUuid SessionCredential::service_uuid() const
{
    Q_DECLARE_VU;
    return p->body.value(qsl_fy(service_uuid)).toUuid();
}

SessionCredential &SessionCredential::set_service_uuid(const QVariant &uuid)
{
    Q_DECLARE_VU;
    p->body[qsl_fy(service_uuid)]=vu.toUuid(uuid);
    return*this;
}

QVariantHash &SessionCredential::body() const
{
    return p->body;
}

SessionCredential &SessionCredential::setBody(const QVariantHash &value)
{
    p->body=value;
    return*this;
}

QVariant SessionCredential::profile() const
{
    return p->body.value(qsl_fy(profile));
}

SessionCredential &SessionCredential::setProfile(const QVariant &value)
{
    p->body[qsl_fy(profile)]=value;
    return*this;
}

QVariant SessionCredential::session() const
{
    return p->body.value(qsl_fy(session));
}

SessionCredential &SessionCredential::setSession(const QVariant &value)
{
    p->body[qsl_fy(session)]=value;
    return*this;
}

QVariant SessionCredential::token() const
{
    return p->body.value(qsl_fy(token));
}

SessionCredential &SessionCredential::setToken(const QVariant &value)
{
    p->body[qsl_fy(token)]=value;
    return*this;
}








}
