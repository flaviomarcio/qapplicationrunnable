#pragma once

#include <QCoreApplication>
#include <QFile>
#include <QTimer>
#include <QDir>
#include <QMutex>
#include "../qapr_application.h"
#include "../../../../qstm/src/qstm_envs.h"

namespace QApr {

class ApplicationPvt:public QObject
{
    Q_OBJECT
public:
    Application *application=nullptr;
    SettingFile settingFile;
    QVariantHash arguments;
    QRpc::SettingManager manager;
    QOrm::ConnectionManager connectionManager;
    Settings settings;
    QApr::CircuitBreaker circuitBreaker;
    QStm::Envs envs;

    explicit ApplicationPvt(Application*parent=nullptr);

    ~ApplicationPvt();

    const SettingFile &resourceSettings();

    static void resourceExtract();

    QVariantHash &getArguments();

    static Application &i();

    void circuitBreakerStart();
};

}
