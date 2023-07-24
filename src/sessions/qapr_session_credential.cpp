#include "./qapr_session_credential.h"
#include "../../../qstm/src/qstm_util_variant.h"

namespace QApr {

static auto const __account_uuid="accountUuid";
static auto const __domain_uuid="domainUuid";
static auto const __profile="profile";
static auto const __session="session";
static auto const __token="token";
static auto const __permits="permits";

class CredentialsPvt:public QObject{
public:
    SessionCredential *parent=nullptr;
    QVariantHash body;
    QUuid id;
    QVariantHash permits;
    QUuid accountUuid;
    QUuid domainUuid;
    QVariant profile;
    QVariant session;
    QVariant token;
    explicit CredentialsPvt(SessionCredential*parent):QObject{parent}, parent{parent}
    {
    }
};


SessionCredential::SessionCredential(QObject *parent) : QObject{parent}, p{new CredentialsPvt{this}}
{
}

QVariantHash &SessionCredential::body() const
{
    return p->body;
}

SessionCredential &SessionCredential::setBody(const QVariantHash &value)
{
    p->body=value;
    Q_DECLARE_VU;
    p->accountUuid=vu.toUuid(value.value(__account_uuid));
    p->domainUuid=vu.toUuid(value.value(__domain_uuid));
    p->profile=value.value(__profile);
    p->session=value.value(__session);
    p->token=value.value(__token);
    p->permits=vu.toHash(value.value(__permits));
    return *this;
}

QVariantHash SessionCredential::toVariant() const
{
    QVariantHash vHash{
         {QT_STRINGIFY(id), this->id()}
        ,{QT_STRINGIFY(accountUuid), this->accountUuid()}
        ,{QT_STRINGIFY(profile), this->profile()}
        ,{QT_STRINGIFY(session), this->session()}
        ,{QT_STRINGIFY(permits), this->permits()}
    };
    return vHash;
}

bool SessionCredential::isValid() const
{
    auto &credentials=*this;
    if(!credentials.id().isNull() && p->token.isValid() && !p->token.isNull())
        return {};
    return true;
}

QVariantHash &SessionCredential::permits()const
{
    return p->permits;
}

SessionCredential &SessionCredential::setPermits(const QVariantHash &permits)
{
    p->permits = permits;
    return *this;
}

QUuid &SessionCredential::id() const
{
    return p->id;
}

SessionCredential &SessionCredential::setId(const QUuid &value)
{
    p->id = value;
    return *this;
}

QUuid SessionCredential::domainUuid() const
{
    return p->domainUuid;
}

SessionCredential &SessionCredential::setDomainUuid(const QVariant &uuid)
{
    Q_DECLARE_VU;
    p->domainUuid=vu.toUuid(uuid);
    return *this;
}

QUuid SessionCredential::accountUuid() const
{
    return p->accountUuid;
}

SessionCredential &SessionCredential::setAccountUuid(const QVariant &uuid)
{
    Q_DECLARE_VU;
    p->accountUuid=vu.toUuid(uuid);
    return *this;
}

QVariant SessionCredential::profile() const
{
    return p->profile;
}

SessionCredential &SessionCredential::setProfile(const QVariant &value)
{
    p->profile=value;
    return *this;
}

QVariant SessionCredential::session() const
{
    return p->session;
}

SessionCredential &SessionCredential::setSession(const QVariant &value)
{
    p->session=value;
    return *this;
}

QVariant SessionCredential::token() const
{
    return p->token;
}

SessionCredential &SessionCredential::setToken(const QVariant &value)
{
    p->token=value;
    return *this;
}

}
