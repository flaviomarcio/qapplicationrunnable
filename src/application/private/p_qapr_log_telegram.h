#pragma once

#include <QObject>
#include <QThread>
#include <QJsonDocument>

namespace QApr {

class LogTelegram : public QThread
{
    Q_OBJECT
public:
    Q_INVOKABLE explicit LogTelegram(QObject *parent = nullptr):QThread{nullptr}{
        Q_UNUSED(parent)
    }
    ~LogTelegram()
    {
    }

    void run()override
    {
        this->exec();
    }
public slots:
    void recebeMessage(const QVariant &v)
    {
        auto msg=QJsonDocument::fromVariant(v).toJson(QJsonDocument::Indented);
    }
};

}
