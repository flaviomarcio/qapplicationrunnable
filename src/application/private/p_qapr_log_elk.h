#pragma once

#include <QObject>
#include <QThread>
#include <QJsonDocument>

namespace QApr {

class LogELK : public QThread
{
    Q_OBJECT
public:
    explicit LogELK(QObject *parent = nullptr):QThread(nullptr)
    {
        Q_UNUSED(parent)
    }
    ~LogELK()
    {
    }

    void run()override
    {
        this->exec();
    }
public slots:
    void recebeMessage(const QVariant&v){
        auto msg=QJsonDocument::fromVariant(v).toJson(QJsonDocument::Indented);
    }
};

}
