#include "./qapr_settings.h"
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

Host *Settings::host()
{
    return &_host;
}

Settings &Settings::setHost(Host *newHost)
{
    Q_UNUSED(newHost)
//    if (_host == newHost)
//        return;
//    _host = newHost;
//    emit hostChanged();
    return *this;
}

Settings &Settings::resetHost()
{
    return setHost({});
}

const QString &Settings::version() const
{
    return _version;
}

Settings &Settings::setVersion(const QString &newVersion)
{
    if (_version == newVersion)
        return *this;
    _version = newVersion;
    emit versionChanged();
    return *this;
}

Settings &Settings::resetVersion()
{
    return setVersion({});
}

const QString &Settings::envFile() const
{
    return this->_envFile;
}

Settings &Settings::setEnvFile(const QString &newVersion)
{
    if (_version == newVersion)
        return *this;
    _version = newVersion;
    emit versionChanged();
    return *this;
}

Settings &Settings::resetEnvFile()
{
    return this->setEnvFile({});
}

} // namespace QApr
