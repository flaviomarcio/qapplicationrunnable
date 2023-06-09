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
    enum Classification { Attribute, CRON };
    enum Mode{SingleExec, ContinuosExec};
    //!
    //! \brief SchedulerAnnotation
    //! \param parent
    //!
    explicit SchedulerAnnotation(QObject *parent = nullptr);
    virtual ~SchedulerAnnotation();

public:

    //!
    //! \brief scObject
    //!
    Q_ANNOTATION_DECLARE_FUNC(scObject, Attribute);

    //!
    //! \brief scSchedule
    //!
    Q_ANNOTATION_DECLARE_FUNC(scSchedule, Attribute);

    //!
    //! \brief scObject
    //!
    Q_ANNOTATION_DECLARE_VALUE(scMode, Attribute, ContinuosExec);

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
    //! \brief scIntervalInitial
    //! \return
    //!
    Q_ANNOTATION_DECLARE_FUNC(scIntervalInitial, CRON)

    //!
    //! \brief scInterval
    //! \return
    //!
    Q_ANNOTATION_DECLARE_FUNC(scInterval, CRON)

    //!
    //! \brief scIntervalLimit
    //! \return
    //!
    Q_ANNOTATION_DECLARE_FUNC(scIntervalLimit, CRON)

private:
};

} // namespace QAprPrivate
