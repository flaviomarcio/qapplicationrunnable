#pragma once

#include <QObject>
#include <QThread>
#include <QMap>
#include <QVariant>
#include <QSqlDatabase>
#include "../../../qorm/src/qorm_macro.h"
#include "../application/qapr_global.h"
#include "../../../qrpc/src/qrpc_service_setting.h"
//#include "../application/qapr_consts.h"
//#include "../../../qrpc/src/qrpc_setting_manager.h"
//#include "../../../qorm/src/qorm_connection_manager.h"

#define QORM_DECLARE_TOPIC(v1) public: Q_INVOKABLE virtual QByteArray &topic()const{ static auto v=QByteArray{v1};return v; }

namespace QApr {
class NotifyBasePvt;
class Notify;

//!
//! \brief The NotifyBase class
//!
class Q_APR_EXPORT NotifyBase : public QThread
{
    Q_OBJECT
    QORM_CONNECTION_SUPPORT()
    QORM_DECLARE_TOPIC("nothing/null")
public:

    //!
    //! \brief NotifyBase
    //! \param parent
    //!
    Q_INVOKABLE explicit NotifyBase(QObject *parent=nullptr);

    //!
    //! \brief notifySetting
    //! \return
    //!
    virtual QRpc::ServiceSetting &notifySetting();

    //!
    //! \brief run
    //!
    void run() override;

    //!
    //! \brief start
    //!
    virtual void start();

    //!
    //! \brief notifyName
    //! \return
    //!
    Q_INVOKABLE virtual QString notifyName() const;

    //!
    //! \brief setNotifyName
    //! \param v
    //!
    Q_INVOKABLE virtual void setNotifyName(const QVariant &v);

    //!
    //! \brief resourceSettings
    //! \return
    //!
    Q_INVOKABLE static const QVariant resourceSettings();

private slots:

    //!
    //! \brief onNotifyReceived
    //! \param channel
    //! \param payload
    //!
    virtual void onNotifyReceived(const QString &channel, const QVariant &payload);
signals:

    //!
    //! \brief notifyReceived
    //! \param channel
    //! \param payload
    //!
    void notifyReceived(const QString &channel, const QVariant &payload);
private:
    NotifyBasePvt *p=nullptr;
};
}
