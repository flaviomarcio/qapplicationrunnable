#pragma once

#include <QObject>
#include <QThread>
#include <QVariantMap>
#include <QVariantHash>
#include <QVariant>
#include <QSqlDatabase>
#include "./private/p_qapr_scheduler_annotation.h"
#include "../application/qapr_global.h"
#include "../../../qorm/src/qorm_object_db.h"
#include "../../../qrpc/src/qrpc_service_setting.h"

namespace QApr {

class SchedulerAgent;
class SchedulerPvt;
//!
//! \brief The Scheduler class
//!
class Q_APR_EXPORT Scheduler : public QOrm::ObjectDb, public PrivateQApr::SchedulerAnnotation
{
    Q_OBJECT
    Q_ANNOTATION(Scheduler, nl({scObject()}))
public:
    //!
    //! \brief Scheduler
    //! \param parent
    //!
    Q_INVOKABLE explicit Scheduler(QObject *parent=nullptr);

    //!
    //! \brief settings
    //! \return
    //!
    QRpc::ServiceSetting &settings() const;
private:
    SchedulerPvt *p=nullptr;
};
}
