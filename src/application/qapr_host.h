#pragma once

#include "../../qstm/src/qstm_object_wrapper.h"

namespace QApr {
class HostPvt;

//!
//! \brief The Host class
//!
class Q_STM_EXPORT Host : public QStm::ObjectWrapper
{
    Q_OBJECT
    Q_STM_OBJECT_WRAPPER(Host)

    Q_PROPERTY(QByteArray protocol READ protocol WRITE setProtocol RESET resetProtocol NOTIFY protocolChanged)

    Q_PROPERTY(QByteArray hostName READ hostName WRITE setHostName RESET resetHostName NOTIFY hostNameChanged)

    Q_PROPERTY(int port READ port WRITE setPort RESET resetPort NOTIFY portChanged)

    Q_PROPERTY(QVariantHash headers READ headers WRITE setHeaders RESET resetHeaders NOTIFY headersChanged)

    Q_PROPERTY(QByteArray basePath READ basePath WRITE setBasePath RESET resetBasePath NOTIFY basePathChanged)
public:
    Q_INVOKABLE explicit Host(QObject *parent = nullptr);

    //!
    //! \brief isValid
    //! \return
    //!
    Q_INVOKABLE bool isValid()const;

    //!
    //! \brief isEmpty
    //! \return
    //!
    Q_INVOKABLE bool isEmpty()const;

    //!
    //! \brief protocol
    //! \return
    //!
    const QByteArray &protocol() const;
    void setProtocol(const QByteArray &newProtocol);
    void resetProtocol();

    //!
    //! \brief hostName
    //! \return
    //!
    const QByteArray &hostName() const;
    void setHostName(const QByteArray &newHostName);
    void resetHostName();

    //!
    //! \brief port
    //! \return
    //!
    int port() const;
    void setPort(int newPort);
    void resetPort();

    //!
    //! \brief headers
    //! \return
    //!
    QVariantHash &headers()const;
    void setHeaders(const QVariantHash &newHeaders);
    void addHeaders(const QVariantHash &newHeaders);
    void resetHeaders();

    //!
    //! \brief basePath
    //! \return
    //!
    const QByteArray &basePath() const;
    void setBasePath(const QByteArray &newBasePath);
    void resetBasePath();

private:
    HostPvt *p=nullptr;
signals:

    void protocolChanged();
    void hostNameChanged();
    void portChanged();
    void headersChanged();
    void basePathChanged();
};

} // namespace QMFE
