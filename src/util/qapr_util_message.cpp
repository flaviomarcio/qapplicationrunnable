#include "./qapr_util_message.h"
#include "../../../qrpc/src/qrpc_request.h"
#include "../../../qstm/src/qstm_message.h"
#include "../application/qapr_application.h"
#include "../../../qorm/src/qorm_controller.h"
#include "../../../qstm/src/qstm_message.h"

namespace QApr {

//enum MessageType{dsMail=1, dsSms=2, dsPushNotification=4, dsTelegram=8, dsWhatsApp=16};

static bool sendMessage(const QStm::SettingBase &setting, const QVariant &vMsg)
{
    QRpc::Request request;
    request = setting;
    QVariantHash vHash;
    QStm::Message message(vMsg);
    vHash.insert(QStringLiteral("uuid"), message.uuid().toByteArray());
    vHash.insert(QStringLiteral("type"), message.typeId());
    vHash.insert(QStringLiteral("from"), message.from());
    vHash.insert(QStringLiteral("to"), message.to());
    vHash.insert(QStringLiteral("subject"), message.subject());
    if (!message.bodyHtml().isEmpty())
        vHash.insert(QStringLiteral("payload"), message.bodyHtml());
    else if (!message.bodyText().isEmpty())
        vHash.insert(QStringLiteral("payload"), message.bodyText());

    if (!message.attachment().isEmpty()) {
        QVariantList attachment;
        for (auto &v : message.attachment()) {
            QVariantMap vMap;
            Url url(v);
            vMap[QStringLiteral("name")] = url.name();
            vMap[QStringLiteral("body")] = url.readBody().toHex();
            attachment << vMap;
        }

        vHash.insert(QStringLiteral("attachment"), attachment);
    }
    auto &response = request.call(QStringLiteral("v1/message/send"), vHash);
#if Q_APR_LOG
    if (!response.isOk()) {
        setting.print();
        request.header().print();
    }
#endif
    return response.isOk();
}

static bool sendMessage(const QStm::SettingBase &setting,
                        const QUuid &serviceToken,
                        const QString &type,
                        const QString &to,
                        const QString &payload,
                        const QVariantList &attachment,
                        QVariant &responseBody)
{
    auto base
        = QStringLiteral("%1%2").arg(__PRETTY_FUNCTION__, QDateTime::currentDateTime().toString()).toUtf8();
    auto uuid = QUuid::createUuidV3(QUuid::createUuid(), base);
    QRpc::Request request;
    request = setting;
    if (!serviceToken.isNull())
        request.header().setAuthorization(QRpc::Service, serviceToken);
    QVariantHash map;
    map.insert(QStringLiteral("uuid"), uuid);
    map.insert(QStringLiteral("type"), type);
    map.insert(QStringLiteral("to"), to);
    map.insert(QStringLiteral("payload"), payload);
    map.insert(QStringLiteral("attachment"), attachment);
    auto &response = request.call(QStringLiteral("v1/message/send"), map);
#if Q_APR_LOG
    if (!response.isOk()) {
        setting.print();
        request.header().print();
    }
#endif
    responseBody = response.body();
    return response.isOk();
}

UtilMessage::UtilMessage(QObject *parent) : QStm::Object{parent} {}

ResultValue &UtilMessage::send(const QStm::SettingBase &setting,
                               const QUuid &serviceToken,
                               const QByteArray &serviceType,
                               const QString &to,
                               const QString &body,
                               const QVariantList &attachment)
{
    QVariant response;
    if (!sendMessage(setting, serviceToken, serviceType, to, body, attachment, response))
        return this->lr().setCritical("No send sms message");
    return this->lr();
}

ResultValue &UtilMessage::sendSMS(const QStm::SettingBase &setting,
                                  const QUuid &serviceToken,
                                  const QString &to,
                                  const QString &body)
{
    QVariant response;
    if (!sendMessage(setting, serviceToken, QStringLiteral("sms"), to, body, QVariantList(), response))
        return this->lr().setCritical("No send sms message");
    return this->lr();
}

ResultValue &UtilMessage::sendPushNotify(const QStm::SettingBase &setting,
                                         const QUuid &serviceToken,
                                         const QString &to,
                                         const QString &body)
{
    QVariant response;
    if (!sendMessage(
            setting, serviceToken, QStringLiteral("pushnotification"), to, body, QVariantList(), response))
        return this->lr().setCritical("No send push notification");
    return this->lr();
}

ResultValue &UtilMessage::sendTelegram(const QStm::SettingBase &setting,
                                       const QUuid &serviceToken,
                                       const QString &to,
                                       const QString &body)
{
    QVariant response;
    if (!sendMessage(setting, serviceToken, QStringLiteral("telegram"), to, body, QVariantList(), response))
        return this->lr().setCritical("No send telegram message");
    return this->lr();
}

ResultValue &UtilMessage::sendEmail(const QStm::SettingBase &setting,
                                    const QUuid &serviceToken,
                                    const QString &to,
                                    const QString &body)
{
    QVariant response;
    if (!sendMessage(setting, serviceToken, QStringLiteral("email"), to, body, QVariantList(), response))
        return this->lr().setCritical("No send email");
    return this->lr();
}

ResultValue &UtilMessage::sendWhatsApp(const QStm::SettingBase &setting,
                                       const QUuid &serviceToken,
                                       const QString &to,
                                       const QString &body)
{
    QVariant response;
    if (!sendMessage(setting, serviceToken, QStringLiteral("whatsapp"), to, body, QVariantList(), response))
        return this->lr().setCritical("No send whatsapp message");
    return this->lr();
}

ResultValue &UtilMessage::send(const QStm::SettingBase &setting, const QVariant &message)
{
    if (!sendMessage(setting, message))
        return this->lr().setCritical("No send message");
    return this->lr();
}

ResultValue &UtilMessage::sendSMS(const QStm::SettingBase &setting, const QVariant &message)
{
    QStm::Message msg(message);
    if (!sendMessage(setting, msg.setType(QStringLiteral("sms"))))
        return this->lr().setCritical("No send message");
    return this->lr();
}

ResultValue &UtilMessage::sendPushNotify(const QStm::SettingBase &setting,
                                         const QVariant &message)
{
    QStm::Message msg(message);
    if (!sendMessage(setting, msg.setType(QStringLiteral("PushNotify"))))
        return this->lr().setCritical("No send message");
    return this->lr();
}

ResultValue &UtilMessage::sendTelegram(const QStm::SettingBase &setting, const QVariant &message)
{
    QStm::Message msg(message);
    if (!sendMessage(setting, msg.setType(QStringLiteral("telegram"))))
        return this->lr().setCritical("No send message");
    return this->lr();
}

ResultValue &UtilMessage::sendEmail(const QStm::SettingBase &setting, const QVariant &message)
{
    QStm::Message msg(message);
    if (!sendMessage(setting, msg.setType(QStringLiteral("email"))))
        return this->lr().setCritical("No send message");
    return this->lr();
}

ResultValue &UtilMessage::sendWhatsApp(const QStm::SettingBase &setting, const QVariant &message)
{
    QStm::Message msg(message);
    if (!sendMessage(setting, msg.setType(QStringLiteral("whatsApp"))))
        return this->lr().setCritical("No send message");
    return this->lr();
}

} // namespace QApr
