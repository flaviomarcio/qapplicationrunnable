#pragma once

#include <QCoreApplication>
#include <QCryptographicHash>
#include <QDebug>
#include <QUuid>
#include <QStringList>
#include <QTimer>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QLocale>
#include <QJsonDocument>
#ifdef QT_TESTLIB_LIB
#include <QTest>
#else
#include <gtest/gtest.h>
#endif
#include "../../src/services/servers/qapr_server.h"

namespace QApr{


#define QAPR_OBJECT_TEST(OBJECT) \
public:\
Q_INVOKABLE explicit OBJECT(QObject *parent=nullptr):ObjectTest{parent}{};

#ifdef QT_TESTLIB_LIB
class ObjectTestPvt;
class ObjectTest : public QObject{
    Q_OBJECT
#else
class ObjectTest : public testing::Test{
#endif
public:
#ifdef QT_TESTLIB_LIB
    Q_INVOKABLE explicit ObjectTest(QObject *parent=nullptr);
#else
    explicit ObjectTest();
#endif

#ifdef QTREFORCE_QAPR
    static QApr::Server &service();
#endif
    virtual bool clear();

    virtual void configure();

    virtual void deconfigure();

    virtual void execute();

    virtual bool serviceStart();

    virtual bool serviceStop();

    virtual QStringList arguments();

    static QByteArray toByteArray(const QVariant &v);

    static QByteArray toMd5(const QVariant &v);

    static QUuid toMd5Uuid(const QVariant &v);

    static QUuid toUUID(const QVariant &v);

    static QVariant toVar(const QVariant &v);

    static QVariant toVarObject(const QVariant &v);

    static QByteArray fakeBody(int maxloop=1);;


private:
#ifdef QT_TESTLIB_LIB
    ObjectTestPvt *p=nullptr;
#endif
};

typedef QApr::ObjectTest ObjectTest;

}
