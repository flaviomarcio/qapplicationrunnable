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
#include <QTest>

namespace QApr{

#define Q_APR_OBJECT_TEST(OBJECT) \
public:\
Q_INVOKABLE explicit OBJECT(QObject *parent=nullptr):QApr::ObjectTest{parent}{};

class ObjectTestPvt;

//!
//! \brief The ObjectTest class
//!
class ObjectTest : public QObject
{
    Q_OBJECT
public:
    Q_INVOKABLE explicit ObjectTest(QObject *parent=nullptr);

    //!
    //! \brief clear
    //! \return
    //!
    virtual bool clear();

    //!
    //! \brief configure
    //!
    virtual void configure();

    //!
    //! \brief deconfigure
    //!
    virtual void deconfigure();

    //!
    //! \brief execute
    //!
    void execute();

    //!
    //! \brief arguments
    //! \return
    //!
    static QStringList arguments();

    //!
    //! \brief toByteArray
    //! \param v
    //! \return
    //!
    static QByteArray toByteArray(const QVariant &v);

    //!
    //! \brief toMd5
    //! \param v
    //! \return
    //!
    static QByteArray toMd5(const QVariant &v);

    //!
    //! \brief toMd5Uuid
    //! \param v
    //! \return
    //!
    static QUuid toMd5Uuid(const QVariant &v);

    //!
    //! \brief toUuid
    //! \param v
    //! \return
    //!
    static QUuid toUuid(const QVariant &v);

    //!
    //! \brief toVar
    //! \param v
    //! \return
    //!
    static QVariant toVar(const QVariant &v);

    //!
    //! \brief toVarObject
    //! \param v
    //! \return
    //!
    static QVariant toVarObject(const QVariant &v);

    //!
    //! \brief fakeBody
    //! \param maxloop
    //! \return
    //!
    static QByteArray fakeBody(int maxloop=1);;

private:
    ObjectTestPvt *p=nullptr;
};

}
