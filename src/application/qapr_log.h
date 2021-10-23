#pragma once

#include <QObject>
#include "./qapr_global.h"

namespace QApr {

class Q_APR_EXPORT Log : public QObject
{
    Q_OBJECT
public:
    Q_INVOKABLE explicit Log(QObject *parent = nullptr);
    Q_INVOKABLE ~Log();

    /**
     * @brief enabled
     */
    static void enabled();

    /**
     * @brief disabled
     */
    static void disabled();
};

} // namespace QApr
