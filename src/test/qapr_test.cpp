#include "./qapr_test.h"
#include "../../../qstm/src/qstm_util_hash.h"
#include "../../../qstm/src/qstm_util_variant.h"
#include "../application/qapr.h"

namespace QApr{

class ObjectTestPvt:public QObject{
public:
    ObjectTest *parent=nullptr;
    explicit ObjectTestPvt(ObjectTest *parent):QObject{parent}, parent{parent}
    {
    }
};

ObjectTest::ObjectTest(QObject *parent):QObject{parent}, p{new ObjectTestPvt{this}}
{

}

void ObjectTest::configure()
{
}

void ObjectTest::deconfigure()
{
}

void ObjectTest::execute()
{
    qInfo()<<"";
    qInfo()<<"";
    aInfo()<<"test started, class:"<<this->metaObject()->className();
    qInfo()<<"";
    qInfo()<<"";
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
    qInfo()<<"";
    qInfo()<<"";
    aInfo()<<"test finished, class:"<<this->metaObject()->className();
    qInfo()<<"";
    qInfo()<<"";
}

bool ObjectTest::clear()
{
    return true;
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
    Q_DECLARE_HU;
    return hu.toMd5(v);
}

QUuid ObjectTest::toMd5Uuid(const QVariant &v)
{
    Q_DECLARE_HU;
    return hu.toMd5Uuid(v);
}

QUuid ObjectTest::toUuid(const QVariant &v)
{
    Q_DECLARE_HU;
    return hu.toUuid(v);
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
