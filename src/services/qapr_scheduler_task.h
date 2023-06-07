#pragma once

#include <QObject>
#include <QThread>
#include <QVariant>
#include <QMetaMethod>
#include "../application/qapr_global.h"
#include "../../qstm/src/qstm_macro.h"
#include "../../qrpc/src/qrpc_service_setting.h"

namespace QApr{
class Schedule;
class SchedulerTaskPvt;

//!
//! \brief The SchedulerTask class
//!
class Q_APR_EXPORT SchedulerTask : public QThread
{
    Q_OBJECT
    Q_STM_DECLARE_BUILDER(SchedulerTask)
public:

    //!
    //! \brief SchedulerTask
    //! \param parent
    //!
    Q_INVOKABLE explicit SchedulerTask(QObject *parent=nullptr);

    //!
    //! \brief run
    //!
    void run() override;

    //!
    //! \brief settings
    //! \return
    //!
    QRpc::ServiceSetting &settings() const;

    //!
    //! \brief uuid
    //! \return
    //!
    QUuid uuid() const;

    //!
    //! \brief name
    //! \return
    //!
    QByteArray name() const;
    SchedulerTask &name(const QVariant &newName);
    SchedulerTask &resetName();

    //!
    //! \brief connection
    //! \return
    //!
    QVariantHash connection() const;
    void connection(const QVariantHash &newConnection);
    void resetConnection();

    //!
    //! \brief taskMetaObject
    //! \return
    //!
    QMetaObject &taskMetaObject() const;
    SchedulerTask &taskMetaObject(const QMetaObject &newTaskMetaObject);
    SchedulerTask &resetTaskMetaObject();

    //!
    //! \brief taskMetaMethod
    //! \return
    //!
    QMetaMethod taskMetaMethod() const;
    SchedulerTask &taskMetaMethod(const QMetaMethod &newTaskMetaMethod);
    SchedulerTask &resetTaskMetaMethod();

signals:
    void taskStarted(SchedulerTask *parent);
    void taskUpdate(SchedulerTask *parent);
    void taskFinished(SchedulerTask *parent);
private:
    SchedulerTaskPvt *p=nullptr;
};

}
