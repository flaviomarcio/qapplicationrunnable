#pragma once

#include "../../qstm/src/qstm_startup.h"
#include "./qapr_global.h"

#define Q_APP_STARTUP_FUNCTION(FUNC) Q_STM_STARTUP_APPEND_BASE(QApr, AppStartUp, FUNC)

namespace QApr {

//!
//! \brief The StartUp class
//!
class Q_APR_EXPORT AppStartUp : public QStm::StartUp
{
    Q_OBJECT
    Q_STM_STARTUP_CONSTUCTOR_BASE(AppStartUp, 4)
};

}
