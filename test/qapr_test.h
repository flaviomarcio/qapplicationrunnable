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
#include <gtest/gtest.h>
#include "../../qrpc/src/qrpc_types.h"
#include "../../qrpc/src/qrpc_request.h"
#include "../../qrpc/src/qrpc_listen_protocol.h"
#include "../../qstm/src/qstm_util_variant.h"
#include "../src/services/qapr_server.h"

namespace QApr{

class SDKGoogleTest : public testing::Test{
public:
    explicit SDKGoogleTest();

    static QApr::Server&service();

    virtual bool clear();

    virtual bool serviceStart();

    virtual bool serviceStop();

    virtual QStringList arguments();

    static QByteArray toMd5(const QVariant&v);

    static QVariant toVar(const QVariant&v);

    QByteArray fakeBody(const int maxloop=1);;

    static QUuid toUUID(const QVariant&v);

    static void SetUpTestCase();

    virtual void SetUp();

    virtual void TearDown();

    static void TearDownTestCase();

public:

};

}
