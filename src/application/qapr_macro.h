#pragma once

#include <QObject>
#include <QVariant>
#include <QDate>
#include <QTime>
#include <QDateTime>
#include <QCoreApplication>

#define Q_APR_DECLARE_INTERFACE_METHOD_CHECK() \
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

#define ___a_party_1 QString::number(qlonglong(QThread::currentThreadId()))+QStringLiteral(":")

#define ___a_party_2 QString(__PRETTY_FUNCTION__).replace(QStringLiteral("virtual"),QLatin1String("")).split(QStringLiteral("(")).first().trimmed()+QStringLiteral(":")

#define aDebug()qDebug()<<___a_party_1<<___a_party_2

#define aCritical()qCritical()<<___a_party_1<<___a_party_2

#define aInfo()qInfo()<<___a_party_1<<___a_party_2

#define aWarning()qWarning()<<___a_party_1<<___a_party_2

#define aDebugMethodStart() qDebug()<<___a_party_1<<___a_party_2<<QStringLiteral(":start")

#define aDebugMethodFinish()qDebug()<<___a_party_1<<___a_party_2<<QStringLiteral(":finish")

#define QAPR_CRUD_BLOCK_MAKER_ARGS(parent, crudBlock)\
QOrm::CRUDBlock crudBlock(parent); \
crudBlock.host(QApr::Application::i().settings().host()->toHash()); \
auto irq=parent->irq();\
if(irq!=nullptr)\
    crudBlock.host().addHeaders(irq->request().authorizationHeaders());\
QOrm::CRUDBlock::makeBlock(crudBlock, vBody)

#define QAPR_CRUD_BLOCK_MAKER(crudBlock) QAPR_CRUD_BLOCK_MAKER_ARGS(this, crudBlock)

#define QAPR_CRUD_REPORT_MAKER(crudBlock)QAPR_CRUD_BLOCK_MAKER(crudBlock); \
crudBlock.type(crudBlock.ReportForm);crudBlock
