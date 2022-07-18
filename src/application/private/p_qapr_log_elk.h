#pragma once

#include <QObject>
#include <QThread>

namespace QApr {

class LogELK : public QThread
{
    Q_OBJECT
public:
    explicit LogELK(QObject *parent = nullptr);
    ~LogELK();

    void run()override;
public slots:
    void recebeMessage(const QVariant &v);
};

}
