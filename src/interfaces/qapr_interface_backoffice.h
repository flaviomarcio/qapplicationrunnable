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
    Q_API_DOC_INFO()
    {
        document->host(QStringLiteral("localhost"))
            .basePath(this->basePath())
            .consumes(QStringLiteral("application/json"))
            .produces(QStringLiteral("application/json"))
            .schemes(stpsHttp);

        document->info()
            .title(QStringLiteral("QApr API for back office service"))
            .version(QStringLiteral("1.0.0"))
            .termsOfService(QStringLiteral("http://www.apache.org/licenses/LICENSE-2.0.txt"))
            .description(QStringLiteral("Inherited from QApr:Interface, this is class for constructor "
                             "interface for gui applications"));

        document->info()
            .contact()
            .name(QStringLiteral("Flavio Portela"))
            .email(QStringLiteral("fmspx@hotmail.com"))
            .url(QStringLiteral("https://github.com/flaviomarcio/qtreforce-sdk"));

        document->info()
            .license()
            .name(QStringLiteral("Apache License - Version 2.0, January 2004"))
            .url(QStringLiteral("http://www.apache.org/licenses/LICENSE-2.0"));
    }

public:
    //!
    //! \brief InterfaceBackOffice
    //! \param parent
    //!
    Q_INVOKABLE explicit InterfaceBackOffice(QObject *parent = nullptr);

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
