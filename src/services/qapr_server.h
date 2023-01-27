#pragma once

#include <QObject>
#include "../../../qrpc/src/qrpc_server.h"
#include "../application/qapr_global.h"
#include "../application/qapr_setting_file.h"

namespace QApr {
class ServerPvt;
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
    //! \brief instance
    //! \return
    //!
    static Server &instance();

    //!
    //! \brief resourceSettings
    //! \return
    //!
    Q_INVOKABLE const QApr::SettingFile &resourceSettings();
private:
    ServerPvt *p=nullptr;
};
}
