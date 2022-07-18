#pragma once

#include <QCoreApplication>
#include <QFile>
#include <QTimer>
#include <QDir>
#include <QMutex>
#include "../qapr_application.h"

namespace QApr {

class ApplicationPvt:public QObject
{
    Q_OBJECT
public:
    Application *application=nullptr;
    QStringList _resourceSettings;
    QVariantHash _arguments;
    QRpc::SettingManager manager;
    QOrm::ConnectionManager connectionManager;
    Settings settings;
    QApr::CircuitBreaker circuitBreaker;

    explicit ApplicationPvt(Application*parent=nullptr);

    ~ApplicationPvt();

    QStringList &resourceSettings();

    static void resourceExtract();

    QVariantHash &arguments();

    static Application &i();

    void circuitBreakerStart();
};

}
