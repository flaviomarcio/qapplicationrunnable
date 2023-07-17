#pragma once

#include "../../qstm/src/qstm_object_wrapper.h"
#include "./qapr_host.h"

namespace QApr {

//!
//! \brief The Settings class
//!
class Q_STM_EXPORT Settings : public QStm::ObjectWrapper
{
    Q_OBJECT
    Q_STM_OBJECT_WRAPPER(Settings)
    Q_PROPERTY(Host *host READ host WRITE setHost RESET resetHost NOTIFY hostChanged)
    Q_PROPERTY(QString version READ version WRITE setVersion RESET resetVersion NOTIFY versionChanged)
    Q_PROPERTY(QString envFile READ envFile WRITE setEnvFile RESET resetenvFile NOTIFY envFileChanged)
public:

    //!
    //! \brief Settings
    //! \param parent
    //!
    explicit Settings(QObject *parent = nullptr);

    //!
    //! \brief setValues
    //! \param v
    //! \return
    //!
    virtual bool setValues(const QVariant &v);

    //!
    //! \brief host
    //! \return
    //!
    Host *host();
    Settings &setHost(Host *newHost);
    Settings &resetHost();

    //!
    //! \brief version
    //! \return
    //!
    const QString &version() const;
    Settings &setVersion(const QString &newVersion);
    Settings &resetVersion();

    //!
    //! \brief envFile
    //! \return
    //!
    const QString &envFile() const;
    Settings &setEnvFile(const QString &newVersion);
    Settings &resetEnvFile();


private:
    Host _host;
    QString _version;
    QString _envFile;
signals:
    void hostChanged();
    void versionChanged();
    void envFileChanged();
};

} // namespace QMFE
