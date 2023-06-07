#include "./qapr_interface_annotations.h"

namespace QAprPrivate {

InterfaceAnnotations::InterfaceAnnotations(QObject *parent)//:QAnnotation::Extended{parent}
{
    Q_UNUSED(parent)
}

InterfaceAnnotations::~InterfaceAnnotations() {}

} // namespace QAprPrivate
