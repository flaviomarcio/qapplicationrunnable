#pragma once

#include "../../../qnotation/includes/QAnnotation"
#include <QObject>
#include <QVariant>
#include <QVariantHash>
#include <QVariantList>

namespace PrivateQApr {

//!
//! \brief The SchedulerAnnotation class
//!
class SchedulerAnnotation: public QAnnotation::Extended
{
public:
    enum Classification { Attribute, Task };
    //!
    //! \brief SchedulerAnnotation
    //! \param parent
    //!
    explicit SchedulerAnnotation(QObject *parent = nullptr):QAnnotation::Extended{parent}{}
public:

    //!
    //! \brief scSchedule
    //!
    Q_ANNOTATION_DECLARE_FUNC(scSchedule, Attribute);

    //!
    //! \brief scDescription
    //!
    Q_ANNOTATION_DECLARE_FUNC(scDescription, Attribute)

    //!
    //! \brief scTaskEnabled
    //! \return
    //!
    Q_ANNOTATION_DECLARE_FUNC(scTaskEnabled, Attribute)

    //!
    //! \brief scExecTimeInitial
    //! \return
    //!
    Q_ANNOTATION_DECLARE_FUNC(scExecTimeInitial, Task)

    //!
    //! \brief scExecTime
    //! \return
    //!
    Q_ANNOTATION_DECLARE_FUNC(scExecTime, Task)

    //!
    //! \brief scExecTimeLimit
    //! \return
    //!
    Q_ANNOTATION_DECLARE_FUNC(scExecTimeLimit, Task)

    //!
    //! \brief scScope
    //!
    Q_ANNOTATION_DECLARE_FUNC(scExecScope, Task);

    //!
    //! \brief scTaskGroup
    //! \return
    //!
    Q_ANNOTATION_DECLARE_FUNC(scExecGroup, Task)

    //!
    //! \brief scTaskOrder
    //! \return
    //!
    Q_ANNOTATION_DECLARE_FUNC(scExecOrder, Task)

private:
};

} // namespace QAprPrivate
