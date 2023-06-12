#pragma once

#include <QObject>
#include <QThread>
#include <QVariantMap>
#include <QVariantHash>
#include <QVariant>
#include "../application/qapr_global.h"
#include "./qapr_scheduler_scope_group.h"
#include "./private/p_qapr_scheduler_annotation.h"
#include "../../../qstm/src/qstm_setting_base.h"

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
    const QStm::SettingBase &settings() const;

    //!
    //! \brief invokeBefore
    //! \param scope
    //! \param method
    //! \return
    //!
    virtual bool invokeBefore(const SchedulerScopeGroup *scope, QMetaMethod &method){Q_UNUSED(scope); Q_UNUSED(method); return true;}

    //!
    //! \brief invokeFail
    //! \param scope
    //! \param method
    //! \return
    //!
    virtual bool invokeFail(const SchedulerScopeGroup *scope, QMetaMethod &method){Q_UNUSED(scope); Q_UNUSED(method); return true;}

    //!
    //! \brief invokeAfter
    //! \param scope
    //! \param method
    //! \return
    //!
    virtual bool invokeAfter(const SchedulerScopeGroup *scope, QMetaMethod &method){Q_UNUSED(scope); Q_UNUSED(method); return true;}

    //!
    //! \brief invoke
    //! \param scope
    //! \param method
    //! \return
    //!
    virtual bool invoke(const SchedulerScopeGroup *scope, QMetaMethod &method);

private:
    SchedulerPvt *p=nullptr;
};
}
