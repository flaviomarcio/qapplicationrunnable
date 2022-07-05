#pragma once

#include "./qapr_interface.h"

namespace QApr {
class InterfaceBackOfficePvt;
//!
//! \brief The InterfaceBackOffice class
//!
//!camada criada para organizar menus e aesso do backoffice
class Q_APR_EXPORT InterfaceBackOffice : public QApr::Interface
{
    Q_OBJECT
    Q_DECLARE_OBJECT()
    Q_API_DOC_INFO()
    {
        document->host(qsl("localhost"))
            .basePath(this->basePath())
            .consumes(qsl("application/json"))
            .produces(qsl("application/json"))
            .schemes(stpsHttp);

        document->info()
            .title(qsl("QApr API for back office service"))
            .version(qsl("1.0.0"))
            .termsOfService(qsl("http://www.apache.org/licenses/LICENSE-2.0.txt"))
            .description(qsl("Inherited from QApr:Interface, this is class for constructor "
                             "interface for gui applications"));

        document->info()
            .contact()
            .name(qsl("Flavio Portela"))
            .email(qsl("fmspx@hotmail.com"))
            .url(qsl("https://github.com/flaviomarcio/qtreforce-sdk"));

        document->info()
            .license()
            .name(qsl("Apache License - Version 2.0, January 2004"))
            .url(qsl("http://www.apache.org/licenses/LICENSE-2.0"));
    }

public:
    //!
    //! \brief InterfaceBackOffice
    //! \param parent
    //!
    Q_INVOKABLE explicit InterfaceBackOffice(QObject *parent = nullptr);

    //!
    //! \brief ~InterfaceBackOffice
    //!
    ~InterfaceBackOffice();

#ifdef QTREFORCE_QMFE

    //!
    //! \brief qmfeAccess
    //! \return
    //!
    virtual QMfe::Access &qmfeAccess();

    //!
    //! \brief modules
    //! \return
    //!
    Q_INVOKABLE virtual QVariant modules();
#endif
private:
    InterfaceBackOfficePvt *p = nullptr;
};

} // namespace QApr
