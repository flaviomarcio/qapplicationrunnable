#include "./qapr_test.h"
#include "../../qstm/src/qstm_util_variant.h"
#include "../../qstm/src/qstm_macro.h"
#include "../../qrpc/src/qrpc_listen_protocol.h"

namespace QApr{

SDKGoogleTest::SDKGoogleTest()
{
    QLocale::setDefault(QLocale(QLocale::Portuguese, QLocale::Brazil));
}

Server &SDKGoogleTest::service()
{
    auto&service=QApr::Server::instance();
    auto &http = service.colletions().protocol(QRpc::Http);
    http.setPort(9999);
    return service;
}

bool SDKGoogleTest::clear()
{
    return true;
}

bool SDKGoogleTest::serviceStart()
{
    return service().start();
}

bool SDKGoogleTest::serviceStop()
{
    return service().stop();

}

QStringList SDKGoogleTest::arguments()
{
    return qApp->arguments();
}

QByteArray SDKGoogleTest::toMd5(const QVariant &v)
{
    QByteArray bytes=QMetaTypeUtilObjects.contains(v.typeId())
                           ?
                           QJsonDocument::fromVariant(v).toJson(QJsonDocument::Compact)
                           :
                           v.toByteArray();
    return QCryptographicHash::hash(bytes, QCryptographicHash::Md5).toHex();
}

QVariant SDKGoogleTest::toVar(const QVariant &v)
{
    return
        QMetaTypeUtilString.contains(v.typeId())
            ?
            QJsonDocument::fromJson(v.toByteArray()).toVariant()
            :
            v;
}

QByteArray SDKGoogleTest::fakeBody(int maxloop)
{
    QByteArray __return;
    Q_LOOP_LIMIT(loops, maxloop){
        __return.append(QUuid::createUuid().toByteArray());
    }
    return __return;
}

QUuid SDKGoogleTest::toUUID(const QVariant &v)
{
    return VariantUtil().toUuid(v);
}

void SDKGoogleTest::SetUpTestCase()
{
}

void SDKGoogleTest::SetUp()
{
}

void SDKGoogleTest::TearDown()
{
}

void SDKGoogleTest::TearDownTestCase()
{
}

}
