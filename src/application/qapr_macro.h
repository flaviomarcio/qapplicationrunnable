#pragma once

#include <QObject>
#include <QVariant>
#include <QDate>
#include <QTime>
#include <QDateTime>
#include <QCoreApplication>

#define QAPR_METHOD_BACK_LIST {"destroyed", "objectnamechanged", "deletelater", "_q_reregistertimers", "basepathchanged", "enabledchanged", "connection", "connectionid", "connectionclose","connectionclear","connectionfinish"}

#define QAPR_DECLARE_INTERFACE_METHOD_CHECK() \
public: \
Q_INVOKABLE virtual QVariant check() \
{ \
    this->rq().co().setOK(); \
    return {}; \
} \
Q_INVOKABLE virtual QVariant ping() \
{ \
    this->rq().co().setOK(); \
    return QDateTime::currentDateTime(); \
} \
Q_INVOKABLE virtual QVariant fullCheck() \
{ \
    this->rq().co().setOK(); \
    return {}; \
} \
Q_INVOKABLE virtual QVariant connectionsCheck() \
{ \
    this->rq().co().setOK(); \
    return {}; \
} \
Q_INVOKABLE virtual QVariant businessCheck() \
{ \
    this->rq().co().setNotImplemented(); \
    return {}; \
}

#define ___a_party_1 QString::number(qlonglong(QThread::currentThreadId()))+QStringLiteral(" ")

#define ___a_party_2 QString(__PRETTY_FUNCTION__).replace(QStringLiteral("virtual"),QLatin1String("")).split(QStringLiteral("(")).first().trimmed()+QStringLiteral(" ")

#define aDebug()qDebug()<<___a_party_1 + ___a_party_2

#define aCritical()qCritical()<<___a_party_1 + ___a_party_2

#define aInfo()qInfo()<<___a_party_1 + ___a_party_2

#define aWarning()qWarning()<<___a_party_1 + ___a_party_2

#define QAPR_CRUD_BLOCK_MAKER_ARGS(parent, crudBlock)\
QOrm::CRUDBlock crudBlock(parent); \
crudBlock.host(qAprApp.settings().host()->toHash()); \
auto irq=parent->irq();\
if(irq!=nullptr)\
    crudBlock.host().addHeaders(irq->request().authorizationHeaders());\
QOrm::CRUDBlock::makeBlock(crudBlock, vBody)

#define QAPR_CRUD_BLOCK_MAKER(crudBlock) QAPR_CRUD_BLOCK_MAKER_ARGS(this, crudBlock)

#define QAPR_CRUD_REPORT_MAKER(crudBlock)QAPR_CRUD_BLOCK_MAKER(crudBlock); \
crudBlock.type(Q_ORM_REPORTFORM);crudBlock

#define QAPR_TEST_RUNNER(CLASS){CLASS var; var.execute();}

#define QAPR_RETURN_ERROR() { \
    this->rq().co(controller.lr().sc()); \
    QRPC_RETURN_ERROR() \
}
