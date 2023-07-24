#include "./qapr_settings.h"
#include "./qapr_application.h"
#include "../../../qstm/src/qstm_envs.h"

namespace QApr {
static const auto __apr="apr";
Settings::Settings(QObject *parent): QStm::ObjectWrapper{parent}
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
    return qAprApp.envs().parser(_name).toString();
}

void Settings::setName(const QString &newName)
{
    if(this->_name==newName)
        return;
    this->_name=newName.trimmed();
    emit nameChanged();
}

void Settings::resetName()
{

}

Host *Settings::host()
{
    return &_host;
}

void Settings::setHost(Host *newHost)
{
    Q_UNUSED(newHost)
//    if (_host == newHost)
//        return;
//    _host = newHost;
//    emit hostChanged();
}

void Settings::resetHost()
{
    setHost({});
}

QString Settings::version() const
{
    return qAprApp.envs().parser(_version).toString();
}

void Settings::setVersion(const QString &newVersion)
{
    if (_version == newVersion)
        return;
    _version = newVersion;
    emit versionChanged();
}

void Settings::resetVersion()
{
    setVersion({});
}

} // namespace QApr
