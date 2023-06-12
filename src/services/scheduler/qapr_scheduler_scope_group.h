#pragma once

#include "../application/qapr_global.h"
#include <QUuid>
#include <QObject>
#include <QVector>
#include <QDateTime>
#include <QVariant>

namespace QApr {

class SchedulerScopeGroupPvt;
class Scheduler;

//!
//! \brief The SchedulerScopeGroup class
//!
class Q_APR_EXPORT SchedulerScopeGroup: public QObject
{
    Q_OBJECT
public:
    //!
    //! \brief SchedulerScopeGroup
    //! \param parent
    //!
    explicit SchedulerScopeGroup(QObject *parent=nullptr);
private:
    explicit SchedulerScopeGroup(const QUuid &uuid, const QString &scope, const QString &group, const QMetaObject &scopeMetaObject, QObject *parent=nullptr);
public:
    enum InvokeState{
        NOTHING, STARTED, FAIL, SUCCESSFUL, SKIPPED
    };
    Q_ENUM(InvokeState)
    //!
    //! \brief import
    //! \param metaObject
    //!
    static void reg(const QMetaObject &metaObject);

    //!
    //! \brief scopes
    //! \return
    //!
    static const QVector<SchedulerScopeGroup *> &scopes();

    //!
    //! \brief uuid
    //! \return
    //!
    const QUuid &uuid()const;

    //!
    //! \brief scope
    //! \return
    //!
    const QString &scope() const;

    //!
    //! \brief group
    //! \return
    //!
    const QString &group() const;

    //!
    //! \brief isScope
    //! \param scope
    //! \return
    //!
    bool isScope(const QStringList &scope)const;

    //!
    //! \brief invoke
    //! \param parent
    //!
    void invoke();

signals:
    void invokeState(QUuid stackId, QDateTime dt, QByteArray methodName, int state);
private:
    SchedulerScopeGroupPvt *p=nullptr;
};

} // namespace QApr

