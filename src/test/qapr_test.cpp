#include "./qapr_test.h"
#include "../../qstm/src/qstm_util_variant.h"
#include "../../qstm/src/qstm_macro.h"
#include "../../qrpc/src/qrpc_listen_protocol.h"

namespace QApr{

#ifdef QT_TESTLIB_LIB
ObjectTest::ObjectTest(QObject *parent):QObject{parent}
#else
ObjectTest::ObjectTest()
#endif
{
    QLocale::setDefault(QLocale(QLocale::Portuguese, QLocale::Brazil));
}

ObjectTest::~ObjectTest()
{

}

Server &ObjectTest::service()
{
    auto&service=QApr::Server::instance();
    auto &http = service.colletions().protocol(QRpc::Http);
    http.setPort(9999);
    return service;
}

void ObjectTest::configure()
{
    //
}

void ObjectTest::deconfigure()
{
    //
}

void ObjectTest::execute()
{
    static const QByteArray prefix="test_";

    this->configure();

    for (int i = 0; i < this->metaObject()->methodCount(); ++i) {
        auto method=this->metaObject()->method(i);

        if(method.methodType()!=QMetaMethod::Slot)
            continue;

        if(method.parameterCount()>0)
            continue;

        if(!method.name().startsWith(prefix))
            continue;

        method.invoke(this, Qt::DirectConnection);
    }

    this->deconfigure();
}

bool ObjectTest::clear()
{
    return true;
}

bool ObjectTest::serviceStart()
{
    return service().start();
}

bool ObjectTest::serviceStop()
{
    return service().stop();

}

QStringList ObjectTest::arguments()
{
    return qApp->arguments();
}

QByteArray ObjectTest::toMd5(const QVariant &v)
{
    Q_DECLARE_VU;
    return vu.toMd5(v);
}

QUuid ObjectTest::toMd5Uuid(const QVariant &v)
{
    Q_DECLARE_VU;
    return vu.toMd5Uuid(v);
}

QVariant ObjectTest::toVar(const QVariant &v)
{
    switch (v.typeId()) {
    case QMetaType::QString:
    case QMetaType::QByteArray:
    case QMetaType::QBitArray:
    case QMetaType::QChar:
        return QJsonDocument::fromJson(v.toByteArray()).toVariant();
    default:
        return v;
    }
}

QByteArray ObjectTest::fakeBody(int maxloop)
{
    QByteArray __return;
    Q_LOOP_LIMIT(loops, maxloop){
        __return.append(QUuid::createUuid().toByteArray());
    }
    return __return;
}

QUuid ObjectTest::toUUID(const QVariant &v)
{
    return VariantUtil().toUuid(v);
}

}
