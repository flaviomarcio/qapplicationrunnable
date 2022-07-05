#include "./p_qapr_application.h"
#include "../qapr_startup.h"

namespace QApr{

Q_GLOBAL_STATIC(QString, applicationSettingDir)
Q_GLOBAL_STATIC(Application, staticInstance);
Q_GLOBAL_STATIC(QRpc::ServiceSetting, circuitBreakerSettings);

static void startCircuitBreaker()
{
    auto &manager=staticInstance->manager();
    *circuitBreakerSettings=manager.setting(qsl("circuit-breaker"));
}

static void startSettings()
{
    auto&i=*staticInstance;
    auto settingFiles=i.resourceSettings().toStringList();
    auto &manager=i.manager();
    for(auto&settingFile: settingFiles){
        if(manager.load(settingFile)){
            i.settings().setValues(settingFile);
            i.connectionManager().load(settingFile);
            if(!i.connectionManager().isLoaded())
                sWarning()<<qtr("Connection manager is not loaded for %1").arg(settingFiles.join(','));
            break;
        }
    }
}

static void startUp(Application &i)
{
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
        *applicationSettingDir=qsl("%1/%2").arg(settings_HOME_DIR, qApp->applicationName().toLower());
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
        auto settingsFile=qsl("%1.json").arg(qApp->applicationFilePath());
        if(!QFile::exists(settingsFile))
            settingsFile=qsl("%1/%2").arg(*applicationSettingDir, resourceSettings_FILE);

        if(QFile::exists(settingsFile))
            vList.append(settingsFile);
    }

    {
        auto settingsFile=qsl("%1/%2/%3.json").arg(settings_HOME_DIR, qAppName(), resourceSettings_FILE);
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

    QDir dir(qsl(":"));
    dir.setNameFilters(QStringList{qsl("*.json")});
    for(auto &info:dir.entryInfoList()){
        QFile fileSrc(info.filePath());
        QFile fileDst(qsl("%1/%2").arg(*applicationSettingDir,info.fileName()));
        if(fileDst.exists())
            continue;

        if(!fileSrc.open(fileSrc.ReadOnly)){
#if Q_RPC_LOG
            sWarning()<<qsl("No open file:")<<fileSrc.fileName()<<qsl(", error: ")<<fileSrc.errorString();
#endif
            continue;
        }
        if(!fileDst.open(fileDst.Truncate | fileDst.Unbuffered | fileDst.WriteOnly)){
#if Q_RPC_LOG
            sWarning()<<qsl("No open file:")<<fileDst.fileName()<<qsl(", error: ")<<fileDst.errorString();
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
        auto l=v.split(qsl("="));
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
