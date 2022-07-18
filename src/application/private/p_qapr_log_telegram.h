#pragma once

#include <QObject>
#include <QThread>

namespace QApr {

class LogTelegram : public QThread
{
    Q_OBJECT
public:
    Q_INVOKABLE explicit LogTelegram(QObject *parent = nullptr);
    ~LogTelegram();

    void run()override;
public slots:
    void recebeMessage(const QVariant &v);
};

}
