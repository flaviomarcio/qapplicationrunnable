#pragma once

#include "./qapr_interface.h"

namespace QApr {

//!
//! \brief The InterfaceBackOffice class
//!
//!camada criada para organizar menus e aesso do backoffice
class Q_APR_EXPORT InterfaceBackOffice : public QApr::Interface
{
    Q_OBJECT
    Q_DECLARE_OBJECT()
    QRPC_DECLARE_MODULE(qsl_fy(QApr))
    QRPC_DECLARE_DESCRIPTION(qsl_fy(QApr::BackOffice))

    Q_API_DOC_INFO(){
        document->
            host(qsl("localhost")).
            basePath(this->basePath()).
            consumes(qsl("application/json")).
            produces(qsl("application/json")).
            schemes(stpsHttp);

        document->info()
            .title(qsl("QApr API for back office service"))
            .version(qsl("1.0.0"))
            .termsOfService(qsl("http://www.apache.org/licenses/LICENSE-2.0.txt"))
            .description(qsl("Inherited from QApr:Interface, this is class for constructor interface for gui applications"))
            ;

        document->info().contact()
            .name(qsl("Flavio Portela"))
            .email(qsl("fmspx@hotmail.com"))
            .url(qsl("https://github.com/flaviomarcio/qtreforce-sdk"))
            ;

        document->info().license()
            .name(qsl("Apache License - Version 2.0, January 2004"))
            .url(qsl("http://www.apache.org/licenses/LICENSE-2.0"))
            ;
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

    //!
    //! \brief rootObject
    //! \return
    //!objeto principal com toda a informacao
    Q_INVOKABLE virtual const QVariant rootObject();
    Q_API_DOC_PATH_OPERATION(rootObject){
        path->
            operation(sptoGet)
                .operationId(qsl_null)
                .description(qsl("Full object to currenty session"))
                .responses(
                    QAPIResponse().
                    statusCode(200).
                    examples(qvh{{qsl("response"), qsl_null}})
                    );
    }

    //!
    //! \brief sessionAccount
    //! \return
    //!objeto com a informacao da sessao e conta relacionada ao request
    Q_INVOKABLE virtual const QVariant sessionAccount();
    Q_API_DOC_PATH_OPERATION(sessionAccount){
        path->
            operation(sptoGet)
                .operationId(qsl_null)
                .description(qsl("Session of account information"))
                .responses(
                    QAPIResponse().
                    statusCode(200).
                    examples(qvh{{qsl("response"), qsl_null}})
                    );
    }

    //!
    //! \brief enviroment
    //! \return
    //!variaveis de ambiente para ajustes no front
    Q_INVOKABLE virtual const QVariant enviroment();
    Q_API_DOC_PATH_OPERATION(enviroment){
        path->
            operation(sptoGet)
                .operationId(qsl_null)
                .description(qsl("Menus for currenty API"))
                .responses(
                    QAPIResponse().
                    statusCode(200).
                    examples(qvh{{qsl("response"), this->enviroment()}})
                    );
    }

    //!
    //! \brief menu
    //! \return
    //!menu principal com todos as opcoes
    Q_INVOKABLE virtual const QVariant menu();
    Q_API_DOC_PATH_OPERATION(menu){
        path->
            operation(sptoGet)
                .operationId(qsl_null)
                .description(qsl("Menus for currenty API"))
                .responses(
                    QAPIResponse().
                    statusCode(200).
                    examples(qvh{{qsl("response"), this->menu()}})
                    );
    }

    //!
    //! \brief menuInfo
    //! \return
    //!requisita da dos de um menu especifico
    Q_INVOKABLE virtual const QVariant menuInfo();
    Q_API_DOC_PATH_OPERATION(menuInfo){
        path->
            operation(sptoGet)
                .operationId(qsl_null)
                .description(qsl("Menus information"))
                .responses(
                    QAPIResponse().
                    statusCode(200).
                    examples(qvh{{qsl("response"), this->menuInfo()}})
                    );
    }

private:
    void *p = nullptr;
};

}
