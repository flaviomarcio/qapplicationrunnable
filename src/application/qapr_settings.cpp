#include "./qapr_settings.h"

namespace QApr {

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
    for(auto &v:vList){
        auto vHash=ObjectWrapper::parserVariant(v).toHash();
        if(vHash.contains(QStringLiteral("mfe")))
            vHash=vHash.value(QStringLiteral("mfe")).toHash();
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

const QString &Settings::version() const
{
    return _version;
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
