#include "qapr_setting_file.h"

namespace QApr {

SettingFile::SettingFile(QObject *parent)
    : QObject{parent}
{

}

SettingFile::SettingFile(const QString &setting, const QString &envs)
{
    this->_setting=setting;
    this->_envs=envs;
}

SettingFile &SettingFile::clear()
{
    this->_setting.clear();
    this->_envs.clear();
    return *this;
}

QString SettingFile::setting() const
{
    return _setting;
}

SettingFile &SettingFile::setting(const QString &newSetting)
{
    if (_setting == newSetting)
        return *this;
    _setting = newSetting;
    emit settingChanged();
    return *this;
}

SettingFile &SettingFile::resetSetting()
{
    return setting({});
}

QString SettingFile::envs() const
{
    return _envs;
}

SettingFile &SettingFile::envs(const QString &newEnvs)
{
    if (this->_envs == newEnvs)
        return *this;
    this->_envs = newEnvs;
    emit envsChanged();
    return *this;
}

SettingFile &SettingFile::resetEnvs()
{
    return envs({});
}

} // namespace QApr
