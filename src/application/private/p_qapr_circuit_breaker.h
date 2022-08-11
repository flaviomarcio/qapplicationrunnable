#pragma once

#include <QDateTime>
#include <QTimer>
#include <QCoreApplication>
#include "../../../../qstm/src/qstm_setting_base.h"

namespace QApr {

class CircuitBreakerPvt:public QObject
{
    Q_OBJECT
public:
    QStm::SettingBase settingService;
    QVariantHash setting;
    QTimer*timerBreaker=nullptr;

    QDateTime activityLimit;
    qlonglong memoryLimitKB=0;

    explicit CircuitBreakerPvt(QObject *parent=nullptr);

    virtual ~CircuitBreakerPvt();

    bool start();

    bool stop();

private:
    void onCheck();

    void onBreak();
};


}
