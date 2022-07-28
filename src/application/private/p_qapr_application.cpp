#include "./p_qapr_application.h"
#include "../qapr_startup.h"
#include "../qapr_consts.h"
#include "../../qorm/src/qorm_macro.h"

namespace QApr{

Q_GLOBAL_STATIC(QString, applicationSettingDir)
Q_GLOBAL_STATIC(Application, staticInstance);
Q_GLOBAL_STATIC(QRpc::ServiceSetting, circuitBreakerSettings);

static void startCircuitBreaker()
{
    auto &manager=staticInstance->manager();
    *circuitBreakerSettings=manager.setting(QStringLiteral("circuit-breaker"));
}

static void startSettings()
{
    auto&i=*staticInstance;
    auto settingFiles=i.resourceSettings().toStringList();
    auto &manager=i.manager();
    for(auto &settingFile: settingFiles){
        if(manager.load(settingFile)){
            i.settings().setValues(settingFile);
            if(!i.connectionManager().load(settingFile))
                oWarning()<<QObject::tr("Connection manager is not loaded for %1").arg(settingFiles.join(','));
            break;
        }
    }
}

static void startUp(Application &i)
{
    Q_UNUSED(i)
#ifdef QT_DEBUG
    i.resourceExtract();
#endif
    startSettings();
    startCircuitBreaker();
}

static void init()
{
    startUp(*staticInstance);
}

Q_APR_STARTUP_FUNCTION(init)

ApplicationPvt::ApplicationPvt(Application *parent)
    :
      QObject{parent}
{
    if(applicationSettingDir->isEmpty())
        *applicationSettingDir=QStringLiteral("%1/%2").arg(settings_HOME_DIR, qApp->applicationName().toLower());
    this->application=parent;
}

ApplicationPvt::~ApplicationPvt()
{
    circuitBreaker.stop();
}

QStringList &ApplicationPvt::resourceSettings()
{
    if(!_resourceSettings.isEmpty())
        return this->_resourceSettings;

    QStringList vList;
    {
        auto settingsFile=QStringLiteral("%1.json").arg(qApp->applicationFilePath());
        if(!QFile::exists(settingsFile))
            settingsFile=QStringLiteral("%1/%2").arg(*applicationSettingDir, resourceSettings_FILE);

        if(QFile::exists(settingsFile))
            vList.append(settingsFile);
    }

    {
        auto settingsFile=QStringLiteral("%1/%2/%3.json").arg(settings_HOME_DIR, qAppName(), resourceSettings_FILE);
        if(QFile::exists(settingsFile))
            vList.append(settingsFile);
    }
    return (this->_resourceSettings=vList);
}

void ApplicationPvt::resourceExtract()
{
    QDir dirHome(*applicationSettingDir);
    if(!dirHome.exists())
        dirHome.mkpath(*applicationSettingDir);

    if(!dirHome.exists())
        return;

    QDir dir(QStringLiteral(":"));
    dir.setNameFilters(QStringList{QStringLiteral("*.json")});
    for(auto &info:dir.entryInfoList()){
        QFile fileSrc(info.filePath());
        QFile fileDst(QStringLiteral("%1/%2").arg(*applicationSettingDir,info.fileName()));
        if(fileDst.exists())
            continue;

        if(!fileSrc.open(fileSrc.ReadOnly)){
#if Q_RPC_LOG
            oWarning()<<QStringLiteral("No open file:")<<fileSrc.fileName()<<QStringLiteral(", error: ")<<fileSrc.errorString();
#endif
            continue;
        }
        if(!fileDst.open(fileDst.Truncate | fileDst.Unbuffered | fileDst.WriteOnly)){
#if Q_RPC_LOG
            oWarning()<<QStringLiteral("No open file:")<<fileDst.fileName()<<QStringLiteral(", error: ")<<fileDst.errorString();
#endif
            fileSrc.close();
            continue;
        }

        fileDst.write(fileSrc.readAll());
        fileDst.flush();
        fileDst.close();
        fileSrc.close();
    }
}

QVariantHash &ApplicationPvt::arguments()
{
    if(!_arguments.isEmpty())
        return _arguments;

    for(auto &v:qApp->arguments()){
        auto l=v.split(QStringLiteral("="));
        if(l.isEmpty())
            continue;

        if(l.size()==1){
            auto key=l.first();
            auto value=l.last();
            _arguments[key]=value;
            continue;
        }

        auto key=l.first().toLower();
        auto value=l.last();
        _arguments[key]=value;
    }

    QHashIterator<QString, QVariant> i(manager.arguments());
    while (i.hasNext()) {
        i.next();
        _arguments[i.key()]=i.value();
    }
    return _arguments;
}

Application &ApplicationPvt::i()
{
    return *staticInstance;
}

void ApplicationPvt::circuitBreakerStart()
{
    if(circuitBreakerSettings->enabled()){
        this->circuitBreaker.setSettings(circuitBreakerSettings->toHash());
        if(this->circuitBreaker.start())
            this->circuitBreaker.print();
    }
}

}
