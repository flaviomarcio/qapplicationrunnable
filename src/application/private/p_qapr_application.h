#pragma once

#include <QCoreApplication>
#include <QFile>
#include <QTimer>
#include <QDir>
#include <QMutex>
#include "../qapr_application.h"

namespace QApr {

Q_GLOBAL_STATIC(QString, applicationSettingDir)

class ApplicationPvt:public QObject{
    Q_OBJECT
public:
    Application *application=nullptr;
    QStringList _resourceSettings;
    QVariantHash _arguments;
    QRpc::ServiceManager manager;
    Settings settings;
    QApr::CircuitBreaker *circuitBreaker=nullptr;

    explicit ApplicationPvt(Application*parent=nullptr)
        :
          QObject{parent},
          manager{parent},
          settings{parent}
    {
        if(applicationSettingDir->isEmpty())
            *applicationSettingDir=qsl("%1/%2").arg(settings_HOME_DIR, qApp->applicationName().toLower());
        this->application=parent;
    }

    ~ApplicationPvt()
    {
        if(circuitBreaker)
            circuitBreaker->stop();
    }

    QStringList &resourceSettings()
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

    static void resourceExtract()
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

    QVariantHash &arguments()
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
};

}
