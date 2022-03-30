#pragma once

#include <QCoreApplication>
#include <QObject>
#include "../../../qrpc/src/qrpc_service_manager.h"
#include "./qapr_global.h"
#include "./qapr_consts.h"

namespace QApr {

//!
//! \brief The Application class
//!
class Q_APR_EXPORT Application : public QObject
{
    Q_OBJECT
public:
    Q_INVOKABLE explicit Application(QObject *parent = nullptr);
    ~Application();

    //!
    //! \brief settings_SERVER
    //! \return
    //!
    Q_INVOKABLE virtual QVariant settings_SERVER() const;

    //!
    //! \brief manager
    //! \return
    //!configuracoes para servicos
    virtual QRpc::ServiceManager&manager();

    //!
    //! \brief exec
    //! \param a
    //! \return
    //!init application and resource to Application
    virtual int exec(QCoreApplication &a);

    //!
    //! \brief instance
    //! \return
    //!
    static Application&instance();

    //!
    //! \brief i
    //! \return
    //!
    static Application&i();

    //!
    //! \brief instanceUuid
    //! \return
    //!application instance
    virtual const QUuid &instanceUuid();

    //!
    //! \brief arguments
    //! \return
    //!mix entre qApp->arguments() e manager->arguments();
    virtual QVariantHash &arguments() const;

    //!
    //! \brief printArguments
    //! \return
    //!
    virtual Application &printArguments();

    //!
    //! \brief resourceExtract
    //! \return
    //!
    virtual Application &resourceExtract();

private:
    void*p=nullptr;
};

}
