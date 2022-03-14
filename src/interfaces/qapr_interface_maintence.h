#pragma once

#include "../../../qrpc/src/qrpc_controller.h"
#include "../application/qapr_global.h"
#include "../application/qapr_macro.h"
#include <QtReforce/QApiDoc>

namespace QApr {

//!
//! \brief The InterfaceMaintence class
//!
class Q_APR_EXPORT InterfaceMaintence : public QRpc::Controller
{
    Q_OBJECT
    QRPC_DECLARE_BASE_PATH(QRPCInterfaceCheck,"v1/maintence")
    Q_APR_DECLARE_INTERFACE_METHOD_CHECK()

    Q_API_DOC_INFO(){
        document->
            host(qsl("localhost")).
            basePath(this->basePath()).
            consumes(qsl("application/json")).
            produces(qsl("application/json")).
            schemes(stpsHttp);

        document->info()
            .title(qsl("QApr API for custom maintenance"))
            .version(qsl("1.0.0"))
            .termsOfService(qsl("http://www.apache.org/licenses/LICENSE-2.0.txt"))
            .description(qsl("Inherited from QRpc::Controller, this is the class for builder interface for maintenance"))
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
    //! \brief InterfaceMaintence
    //! \param parent
    //!
    Q_INVOKABLE explicit InterfaceMaintence(QObject *parent = nullptr);
    ~InterfaceMaintence();

    //!
    //! \brief serverStart
    //! \return
    //!
    Q_INVOKABLE virtual QVariant serverStart();
    Q_API_DOC_PATH(serverStart){
        path->
            operation(sptoGet)
                .operationId(qsl_null)
                .description(qsl("Start custom services"))
                .responses(
                    QAPIResponse().
                    statusCode(200).
                    examples(qvh{{qsl("response"), qsl("custom implementation")}})
                    );
    }

    //!
    //! \brief serverStop
    //! \return
    //!
    Q_INVOKABLE virtual QVariant serverStop();
    Q_API_DOC_PATH(serverStop){
        path->
            operation(sptoGet)
                .operationId(qsl_null)
                .description(qsl("Stop custom services"))
                .responses(
                    QAPIResponse().
                    statusCode(200).
                    examples(qvh{{qsl("response"), qsl("custom implementation")}})
                    );
    }

    //!
    //! \brief serverRestart
    //! \return
    //!
    Q_INVOKABLE virtual QVariant serverRestart();
    Q_API_DOC_PATH(serverRestart){
        path->
            operation(sptoGet)
                .operationId(qsl_null)
                .description(qsl("Restart custom services"))
                .responses(
                    QAPIResponse().
                    statusCode(200).
                    examples(qvh{{qsl("response"), qsl("custom implementation")}})
                    );
    }

    //!
    //! \brief servicesCheck
    //! \return
    //!
    Q_INVOKABLE virtual QVariant servicesCheck();
    Q_API_DOC_PATH(servicesCheck){
        path->
            operation(sptoGet)
                .operationId(qsl_null)
                .description(qsl("Check custom services"))
                .responses(
                    QAPIResponse().
                    statusCode(200).
                    examples(qvh{{qsl("response"), qsl("custom implementation")}})
                    );
    }

    //!
    //! \brief applicationQuit
    //! \return
    //!
    Q_INVOKABLE virtual QVariant applicationQuit();
    Q_API_DOC_PATH(applicationQuit){
        path->
            operation(sptoGet)
                .operationId(qsl_null)
                .description(qsl("Quit application"))
                .responses(
                    QAPIResponse().
                    statusCode(200).
                    examples(qvh{{qsl("response"), qsl("custom implementation")}})
                    );
    }
signals:

};

QRPC_CONTROLLER_AUTO_REGISTER(InterfaceMaintence)

} // namespace QRpc
