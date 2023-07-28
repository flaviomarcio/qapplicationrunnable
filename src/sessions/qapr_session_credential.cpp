#include "./qapr_session_credential.h"
#include "../../../qstm/src/qstm_util_variant.h"

namespace QApr {

static auto const __id="id";
static auto const __scopeId="scopeId";
static auto const __profile="profile";
static auto const __session="session";
static auto const __token="token";
static auto const __permits="permits";

class CredentialsPvt:public QObject{
public:
    SessionCredential *parent=nullptr;
    QVariantHash body;
    QUuid id;
    QUuid scopeId;
    QVariantHash permits;
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
    p->id=vu.toUuid(value.value(__id));
    p->scopeId=vu.toUuid(value.value(__scopeId));
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
        ,{QT_STRINGIFY(scopeId), this->scopeId()}
        ,{QT_STRINGIFY(profile), this->profile()}
        ,{QT_STRINGIFY(session), this->session()}
        ,{QT_STRINGIFY(permits), this->permits()}
    };
    return vHash;
}

bool SessionCredential::isValid() const
{
    auto &credentials=*this;
    if(credentials.id().isNull() || !p->token.isValid() || p->token.isNull())
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

SessionCredential &SessionCredential::setId(const QVariant &value)
{
    Q_DECLARE_VU;
    p->id = vu.toUuid(value);
    return *this;
}

QUuid SessionCredential::scopeId() const
{
    return p->scopeId;
}

SessionCredential &SessionCredential::setScopeId(const QVariant &uuid)
{
    Q_DECLARE_VU;
    p->scopeId=vu.toUuid(uuid);
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
