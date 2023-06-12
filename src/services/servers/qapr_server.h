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
public:
    //!
    //! \brief Server
    //! \param parent
    //!
    Q_INVOKABLE explicit Server(QObject *parent = nullptr);

    //!
    //! \brief i
    //! \return
    //!
    static Server &i();

};
}
