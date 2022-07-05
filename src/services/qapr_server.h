#pragma once

#include <QObject>
#include "../../../qrpc/src/qrpc_server.h"
#include "../application/qapr_global.h"

namespace QApr {

//!
//! \brief The Server class
//!
class Q_APR_EXPORT Server : public QRpc::Server
{
    Q_OBJECT
    Q_DECLARE_INSTANCE(Server)
public:
    //!
    //! \brief Server
    //! \param parent
    //!
    Q_INVOKABLE explicit Server(QObject *parent = nullptr);

    //!
    //! \brief ~Server
    //!
    ~Server();

    //!
    //! \brief resourceSettings
    //! \return
    //!
    Q_INVOKABLE const QVariant resourceSettings();
private:
    void*p=nullptr;
};
}
