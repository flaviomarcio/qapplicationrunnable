#pragma once

#include "../application/qapr_global.h"
#include <QObject>
#include <QVector>

namespace QApr {

class SchedulerScopeGroupPvt;

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
    explicit SchedulerScopeGroup(const QByteArray &scopeName, const QMetaObject *scopeMetaObject, QObject *parent=nullptr);

    //!
    //! \brief scopeName
    //! \return
    //!
    QByteArray &scopeName() const;

    //!
    //! \brief scopeMetaObject
    //! \return
    //!
    const QMetaObject *scopeMetaObject() const;

    //!
    //! \brief methods
    //! \return
    //!
    QVector<int> &methods() const;

    //!
    //! \brief invoke
    //! \param parent
    //!
    void invoke(QObject *parent=nullptr) const;

private:
    SchedulerScopeGroupPvt *p=nullptr;
};

} // namespace QApr

