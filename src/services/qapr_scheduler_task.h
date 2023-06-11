#pragma once

#include <QObject>
#include <QThread>
#include <QVariant>
#include <QMetaMethod>
#include "../application/qapr_global.h"
#include "../../qstm/src/qstm_macro.h"
#include "./qapr_scheduler_scope_group.h"

namespace QApr{
class Schedule;
class SchedulerTaskPvt;

//!
//! \brief The SchedulerTask class
//!
class Q_APR_EXPORT SchedulerTask : public QThread
{
    Q_OBJECT
    Q_STM_DECLARE_BUILDER(SchedulerTask,SchedulerScopeGroup)
public:

    //!
    //! \brief SchedulerTask
    //! \param parent
    //!
    explicit SchedulerTask(QObject *parent=nullptr);

    //!
    //! \brief SchedulerTask
    //! \param scope
    //!
    explicit SchedulerTask(const SchedulerScopeGroup *scope);

    //!
    //! \brief run
    //!
    void run() override;

    //!
    //! \brief uuid
    //! \return
    //!
    QUuid &uuid() const;

signals:
    void taskStarted(SchedulerTask *parent);
    void taskUpdate(SchedulerTask *parent);
    void taskFinished(SchedulerTask *parent);
private:
    SchedulerTaskPvt *p=nullptr;
};

}
