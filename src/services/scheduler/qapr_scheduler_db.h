#pragma once

#include "../../../qorm/src/qorm_object_db_extension.h"
#include "./qapr_scheduler.h"

namespace QApr {
class SchedulerDBPvt;

#define Q_APR_SCHEDULE_DB_CONSTRUCTOR(CLASS, ANNOTATIONS)\
Q_ANNOTATION(CLASS, QVariant(ANNOTATIONS).toList())\
public: Q_INVOKABLE explicit CLASS(QObject *parent=nullptr):QApr::SchedulerDB{parent}{}

//!
//! \brief The SchedulerDB class
//!
class Q_APR_EXPORT SchedulerDB : public QApr::Scheduler, public QOrm::ObjectDbExtension
{
    Q_OBJECT
public:
    //!
    //! \brief SchedulerDB
    //! \param parent
    //!
    Q_INVOKABLE explicit SchedulerDB(QObject *parent=nullptr);

    //!
    //! \brief invokeBefore
    //! \param scope
    //! \param method
    //! \return
    //!
    virtual bool invokeBefore(const QApr::SchedulerScopeGroup *scope, QMetaMethod &method);

    //!
    //! \brief connectionId
    //! \return
    //!
    Q_INVOKABLE QByteArray connectionId() const;

private:
    SchedulerDBPvt *p=nullptr;
};
}
