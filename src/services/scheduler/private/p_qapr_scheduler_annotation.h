#pragma once

#include "../../../qannotation/includes/QAnnotation"
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
    //! \brief scEnabled
    //! \return
    //!
    Q_ANNOTATION_DECLARE_FUNC(scEnabled, Attribute)

    //!
    //! \brief scSynchronize
    //! \return
    //!
    Q_ANNOTATION_DECLARE_FUNC(scSynchronize, Attribute)

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
    //! \brief scExecScope
    //!
    Q_ANNOTATION_DECLARE_FUNC(scExecScope, Task);

    //!
    //! \brief scExecGroup
    //! \return
    //!
    Q_ANNOTATION_DECLARE_FUNC(scExecGroup, Task)

    //!
    //! \brief scExecOrder
    //! \return
    //!
    Q_ANNOTATION_DECLARE_FUNC(scExecOrder, Task)

private:
};

} // namespace QAprPrivate
