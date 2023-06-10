#pragma once

#include <QObject>
#include <QThread>
#include <QVariantMap>
#include <QVariantHash>
#include <QVariant>
#include "../application/qapr_global.h"
#include "./qapr_scheduler_scope_group.h"
#include "./private/p_qapr_scheduler_annotation.h"
#include "../../../qrpc/src/qrpc_service_setting.h"

namespace QApr {
class SchedulerTask;

#define Q_APR_SCHEDULE_METHOD(METHOD,ANNOTATIONS)\
signals:\
void start##METHOD();\
private: Q_ANNOTATION(METHOD,\
    QVariantList(\
                    QVariantList({scSchedule()}) + QVariant(ANNOTATIONS).toList()\
                )\
)\
public: Q_INVOKABLE void METHOD()

#define Q_APR_SCHEDULE_CONSTRUCTOR(CLASS, ANNOTATIONS) \
Q_ANNOTATION(CLASS, QVariant(ANNOTATIONS).toList()) \
public: Q_INVOKABLE explicit CLASS(QObject *parent=nullptr):QApr::Scheduler{parent}{}

class SchedulerAgent;
class SchedulerPvt;
//!
//! \brief The Scheduler class
//!
class Q_APR_EXPORT Scheduler : public QObject, public PrivateQApr::SchedulerAnnotation
{
    Q_OBJECT
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

    //!
    //! \brief beforeExec
    //! \param scope
    //! \param method
    //! \return
    //!
    virtual bool beforeExec(const SchedulerScopeGroup *scope, QMetaMethod &method){Q_UNUSED(scope); Q_UNUSED(method); return true;}

    //!
    //! \brief afterExec
    //! \param scope
    //! \param method
    //! \return
    //!
    virtual bool afterExec(const SchedulerScopeGroup *scope, QMetaMethod &method){Q_UNUSED(scope); Q_UNUSED(method); return true;}

private:
    SchedulerPvt *p=nullptr;
};
}
