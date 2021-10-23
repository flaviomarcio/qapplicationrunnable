#include "./qapr_interface.h"
#include "./qapr_application.h"
#include "./qapr_menu_object.h"

namespace QApr {

Interface::Interface(QObject *parent):InterfaceAuthorization(parent)
{
}

Interface::~Interface()
{
}

QVariantList Interface::backOfficeMenu() const
{
    return qvl_null;
}

}
