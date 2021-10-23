#pragma once

#include "./qstm_message.h"
#include "./qorm_controller.h"
#include "./qrpc_service_setting.h"

namespace QApr {

    class UtilMessage:public QStm::Object
    {
    public:
        Q_INVOKABLE explicit UtilMessage(QObject *parent = nullptr);
        Q_INVOKABLE ~UtilMessage();

        ResultValue &send(const QRpc::ServiceSetting &setting, const QByteArray &serviceType, const QByteArray &serviceToken, const QString &to, const QString &body, const QVariantList&attachment);
        ResultValue &sendSMS(const QRpc::ServiceSetting &setting, const QByteArray &serviceToken, const QString&to, const QString&body);
        ResultValue &sendPushNotify(const QRpc::ServiceSetting &setting, const QByteArray&serviceToken, const QString&to, const QString&body);
        ResultValue &sendTelegram(const QRpc::ServiceSetting &setting, const QByteArray&serviceToken, const QString&to, const QString&body);
        ResultValue &sendEmail(const QRpc::ServiceSetting &setting, const QByteArray &serviceToken, const QString&to, const QString&body);
        ResultValue &sendWhatsApp(const QRpc::ServiceSetting &setting, const QByteArray&serviceToken, const QString&to, const QString&body);

        ResultValue &send(const QRpc::ServiceSetting &setting, const QVariant&message);
        ResultValue &sendSMS(const QRpc::ServiceSetting &setting, const QVariant&message);
        ResultValue &sendPushNotify(const QRpc::ServiceSetting &setting, const QVariant&message);
        ResultValue &sendTelegram(const QRpc::ServiceSetting &setting, const QVariant&message);
        ResultValue &sendEmail(const QRpc::ServiceSetting &setting, const QVariant&message);
        ResultValue &sendWhatsApp(const QRpc::ServiceSetting &setting, const QVariant&message);

    };

}
