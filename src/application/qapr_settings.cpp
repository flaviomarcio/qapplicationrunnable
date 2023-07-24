#include "./qapr_settings.h"
#include "./qapr_application.h"
#include "../../../qstm/src/qstm_envs.h"

namespace QApr {
static const auto __apr="apr";

class SettingsPvt:public QObject{
public:
    Settings *parent=nullptr;
    QString name;
    Host host;
    QString version;
    QString envFile;
    explicit SettingsPvt(Settings *parent):QObject{parent},parent{parent},host{parent}
    {
    }
};

Settings::Settings(QObject *parent): QStm::ObjectWrapper{parent}, p{new SettingsPvt{this}}
{
}

bool Settings::setValues(const QVariant &v)
{
    QVariantList vList;
    auto vParser=ObjectWrapper::parserVariant(v);
    switch (vParser.typeId()) {
    case QMetaType::QVariantList:
    case QMetaType::QStringList:
        vList=vParser.toList();
        break;
    case QMetaType::QVariantHash:
    case QMetaType::QVariantMap:
        vList.append(vParser.toMap());
        break;
    default:
        break;
    }
    QStm::Envs envs;
    for(auto &v:vList){
        v=envs.parser(v);
        auto vHash=ObjectWrapper::parserVariant(v).toHash();
        if(vHash.contains(__apr))
            vHash=vHash.value(__apr).toHash();
        if(vHash.isEmpty())
            continue;
        if(ObjectWrapper::setValues(vHash))
            return true;
    }
    return {};
}

QString Settings::name() const
{
    return qAprApp.envs().parser(p->name).toString();
}

Settings &Settings::setName(const QString &newName)
{
    if(this->p->name==newName)
        return *this;
    this->p->name=newName.trimmed();
    emit nameChanged();
    return *this;
}

Settings &Settings::resetName()
{
    return this->setName({});
}

Host *Settings::host()
{
    return &p->host;
}

Settings &Settings::setHost(Host *newHost)
{
    Q_UNUSED(newHost)
//    if (p->host == newHost)
//        return;
//    p->host = newHost;
//    emit hostChanged();
    return *this;
}

Settings &Settings::resetHost()
{
    return setHost({});
}

const QString Settings::version() const
{
    return qAprApp.envs().parser(p->version).toString();
}

Settings &Settings::setVersion(const QString &newVersion)
{
    if (p->version == newVersion)
        return *this;
    p->version = newVersion;
    emit versionChanged();
    return *this;
}

Settings &Settings::resetVersion()
{
    return setVersion({});
}

const QString Settings::envFile() const
{
    return qAprApp.envs().parser(p->envFile).toString();
}

Settings &Settings::setEnvFile(const QString &newVersion)
{
    if (p->version == newVersion)
        return *this;
    p->version = newVersion;
    emit versionChanged();
    return *this;
}

Settings &Settings::resetEnvFile()
{
    return this->setEnvFile({});
}

} // namespace QApr
