#pragma once

#include <QObject>
#include "./qapr_global.h"

namespace QApr {

//!
//! \brief The SettingFile class
//!
class Q_APR_EXPORT SettingFile : public QObject
{
    Q_OBJECT
public:
    Q_PROPERTY(QString setting READ setting WRITE setting RESET resetSetting NOTIFY settingChanged)
    Q_PROPERTY(QString envs READ envs WRITE envs RESET resetEnvs NOTIFY envsChanged)
public:

    //!
    //! \brief SettingFile
    //! \param parent
    //!
    Q_INVOKABLE explicit SettingFile(QObject *parent = nullptr);

    //!
    //! \brief SettingFile
    //! \param setting
    //! \param envs
    //!
    explicit SettingFile(const QString &setting, const QString &envs);

    //!
    //! \brief clear
    //! \return
    //!
    SettingFile &clear();

    //!
    //! \brief setting
    //! \return
    //!
    QString setting() const;
    SettingFile &setting(const QString &newSetting);
    SettingFile &resetSetting();

    //!
    //! \brief envs
    //! \return
    //!
    QString envs() const;
    SettingFile &envs(const QString &newEnvs);
    SettingFile &resetEnvs();

private:
    QString _setting;
    QString _envs;
signals:
    void settingChanged();
    void envsChanged();
};

} // namespace QApr

