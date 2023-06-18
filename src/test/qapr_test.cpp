#include "./qapr_test.h"

#include "../../../qstm/src/qstm_util_variant.h"

#ifdef QTREFORCE_QAPR
#include <QtReforce/QApr>
#endif

#ifdef QTREFORCE_QRPC
#include <QtReforce/QRpc>
#endif

namespace QApr{

#ifdef QT_TESTLIB_LIB
class ObjectTestPvt:public QObject{
public:
    ObjectTest *parent=nullptr;
    explicit ObjectTestPvt(ObjectTest *parent):QObject{parent}, parent{parent}
    {
    }
};
#endif

#ifdef QT_TESTLIB_LIB
ObjectTest::ObjectTest(QObject *parent):QObject{parent}, p{new ObjectTestPvt{this}}
#else
ObjectTest::ObjectTest()
#endif
{
    QLocale::setDefault(QLocale(QLocale::Portuguese, QLocale::Brazil));
}

#ifdef QTREFORCE_QAPR
QApr::Server &ObjectTest::service()
{
    auto &service=QApr::Server::i();
    auto &http = service.colletions().protocol(QRpc::Http);
    http.setPort(9999);
    return service;
}
#endif

void ObjectTest::configure()
{
    QVERIFY(this->serviceStart());
}

void ObjectTest::deconfigure()
{
    QVERIFY(this->serviceStop());
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
#ifdef QTREFORCE_QAPR
    auto &server=QApr::Server::i();
    auto &listaner=server.colletions().protocol(QRpc::Http);
    listaner.setPort(9999);
    return service().start();
#else
    return false;
#endif
}

bool ObjectTest::serviceStop()
{
#ifdef QTREFORCE_QAPR
    return service().stop();
#else
    return false;
#endif
}

QStringList ObjectTest::arguments()
{
    return qApp->arguments();
}

QByteArray ObjectTest::toByteArray(const QVariant &v)
{
    Q_DECLARE_VU;
    return vu.toByteArray(v);
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

QUuid ObjectTest::toUUID(const QVariant &v)
{
    Q_DECLARE_VU;
    return vu.toUuid(v);
}

QVariant ObjectTest::toVar(const QVariant &v)
{
    Q_DECLARE_VU;
    return vu.toVariant(v);
}

QVariant ObjectTest::toVarObject(const QVariant &v)
{
    Q_DECLARE_VU;
    return vu.toVariantObject(v);
}

QByteArray ObjectTest::fakeBody(int maxloop)
{
    QByteArray __return;
    for (int var = 1; var < maxloop; ++var)
        __return.append(QUuid::createUuid().toByteArray());
    return __return.toHex();
}

}
