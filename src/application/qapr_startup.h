#pragma once

#include <QStm>
#include "./qapr_global.h"

namespace QApr {

#define Q_APR_STARTUP_FUNCTION(FUNC) Q_STM_STARTUP_APPEND(QApr, FUNC)

//!
//! \brief The StartUp class
//!
class Q_APR_EXPORT StartUp : public QStm::StartUp
{
    Q_OBJECT
    Q_STM_STARTUP_CONSTUCTOR(3)
};

} // namespace QRpc
