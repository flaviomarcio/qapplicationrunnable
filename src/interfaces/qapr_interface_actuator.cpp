#include "./qapr_interface_actuator.h"

namespace QApr {

static const auto __status="status";
static const auto __statusUP="UP";

InterfaceActuator::InterfaceActuator(QObject *parent)
    : QRpc::Controller(parent), QAprPrivate::InterfaceAnnotations{this}
{}

QVariant InterfaceActuator::health()
{
    return QVariantHash{{__status,__statusUP}};
}

} // namespace QApr
