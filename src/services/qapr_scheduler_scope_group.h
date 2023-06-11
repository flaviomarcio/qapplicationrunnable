#pragma once

#include "../application/qapr_global.h"
#include <QUuid>
#include <QObject>
#include <QVector>

namespace QApr {

class SchedulerScopeGroupPvt;

typedef std::function<QVariantHash(const QMetaMethod &method)> SchedulerScopeSettingMethod;

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
    explicit SchedulerScopeGroup(const QByteArray &scopeName, const QByteArray &groupName, const QMetaObject *scopeMetaObject, QObject *parent=nullptr);

    //!
    //! \brief uuid
    //! \return
    //!
    QUuid &uuid()const;

    //!
    //! \brief scopeName
    //! \return
    //!
    QByteArray &scopeName() const;

    //!
    //! \brief groupName
    //! \return
    //!
    QByteArray &groupName() const;

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
    //! \brief settings
    //! \return
    //!
    SchedulerScopeSettingMethod settings();
    SchedulerScopeGroup &settings(const SchedulerScopeSettingMethod &settings);

    //!
    //! \brief invoke
    //! \param parent
    //!
    void invoke(QObject *parent=nullptr) const;

private:
    SchedulerScopeGroupPvt *p=nullptr;
};

} // namespace QApr

