#include "./p_qapr_scheduler_annotation.h"

namespace PrivateQApr {

SchedulerAnnotation::SchedulerAnnotation(QObject *parent):QAnnotation::Extended{parent}
{
    Q_UNUSED(parent)
}

SchedulerAnnotation::~SchedulerAnnotation() {}

} // namespace QAprPrivate
