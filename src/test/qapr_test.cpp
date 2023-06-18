#include "./qapr_test.h"

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

    static QVariant toVar(const QVariant &v)
    {
        switch (v.typeId()) {
        case QMetaType::QString:
        case QMetaType::QByteArray:
        {
            auto var=QJsonDocument::fromJson(v.toByteArray()).toVariant();
            if(var.isNull())
                return v;
            return {};
        }
        default:
            return v;
        }
    }

    static QVariant toVarObject(const QVariant &v)
    {
        QVariant var=toVar(v);
        switch (var.typeId()) {
        case QMetaType::QVariantHash:
        case QMetaType::QVariantMap:
        case QMetaType::QVariantPair:
        case QMetaType::QVariantList:
        case QMetaType::QStringList:
            return var;
        default:
            return {};
        }
    }

    static QByteArray toByteArray(const QVariant &v)
    {
        switch (v.typeId()) {
        case QMetaType::QVariantHash:
        case QMetaType::QVariantMap:
        case QMetaType::QVariantPair:
        case QMetaType::QVariantList:
        case QMetaType::QStringList:
            return QJsonDocument::fromVariant(v.toByteArray()).toJson(QJsonDocument::Compact);
            break;
        case QMetaType::QUuid:
            return v.toUuid().toByteArray();
            break;
        case QMetaType::QUrl:
            return v.toUrl().toString().toUtf8();
            break;
        default:
            return v.toByteArray();
        }
    }

    static QByteArray toMd5(const QByteArray &bytes)
    {
        return QCryptographicHash::hash(bytes, QCryptographicHash::Md5).toHex();
    }

    static bool md5ParserUuid(const QString & vtext, QByteArray &outText)
    {
        QByteArray suuid;
        auto text=vtext;
        text.replace(QStringLiteral("-"),"").replace(QStringLiteral("{"),"").replace(QStringLiteral("}"),"");
        if(text.length()==32){
            int i=0;
            for(auto &c:text){
                ++i;
                suuid.append(c.toLatin1());
                if(i==8 || i==12 || i==16 || i==20)
                    suuid.append(QByteArrayLiteral("-"));
            }
            outText=suuid;
            return true;
        }
        outText={};
        return false;
    }

    static QUuid toMd5Uuid(const QByteArray &md5)
    {
        QByteArray md5Out;
        if(!md5ParserUuid(md5, md5Out))
            return {};
        auto suuid=QStringLiteral("{")+md5Out+QStringLiteral("}");
        auto uuid=QUuid::fromString(suuid);
        return uuid;
    }

    static QUuid toUuid(const QVariant &v)
    {
        switch (v.typeId()) {
        case QMetaType::QUuid:
            return v.toUuid();
        default:
            auto bytes=ObjectTestPvt::toByteArray(v);
            auto md5=ObjectTestPvt::toMd5(bytes);
            return ObjectTestPvt::toMd5Uuid(md5);
        }
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
    return ObjectTestPvt::toByteArray(v);
}

QByteArray ObjectTest::toMd5(const QVariant &v)
{
    return ObjectTestPvt::toMd5(toByteArray(v));
}

QUuid ObjectTest::toMd5Uuid(const QVariant &v)
{
    return ObjectTestPvt::toUuid(v);
}

QUuid ObjectTest::toUUID(const QVariant &v)
{
    return ObjectTestPvt::toUuid(v);
}

QVariant ObjectTest::toVar(const QVariant &v)
{
    return ObjectTestPvt::toVar(v);
}

QVariant ObjectTest::toVarObject(const QVariant &v)
{
    return ObjectTestPvt::toVarObject(v);
}

QByteArray ObjectTest::fakeBody(int maxloop)
{
    QByteArray __return;
    for (int var = 1; var < maxloop; ++var)
        __return.append(QUuid::createUuid().toByteArray());
    return __return.toHex();
}

}
