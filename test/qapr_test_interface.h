#pragma once

#include <QObject>

namespace QApr {

class QAPRTestInterface : public QObject
{
    Q_OBJECT
public:
    explicit QAPRTestInterface(QObject *parent = nullptr);

signals:

};

} // namespace QApr
