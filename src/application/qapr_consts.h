#pragma once

#include <QCoreApplication>
#include <QObject>
#include <QString>
#include <QUuid>
#include <QDir>
#include <QThread>


#define settings_HOME_DIR QStringLiteral("%1/qtreforce.files").arg(QDir::homePath())

#ifdef Q_APR_TEST
#define resourceSettings_FILE QStringLiteral("settings.test.json")
#elif QT_NO_DEBUG
#define resourceSettings_FILE QStringLiteral("settings.release.json")
#else
#define resourceSettings_FILE QStringLiteral("settings.debug.json")
#endif

#define CORE_OBJECT_INSTANCE(staticType)\
static staticType&instance(){\
    static staticType*_##staticType=nullptr;\
    if(_##staticType==nullptr)\
        _##staticType=new staticType(nullptr);\
    return*_##staticType;\
}


#define Q_APR_LOG true
#define Q_APR_LOG_VERBOSE false
#define Q_APR_LOG_SUPER_VERBOSE false

#if Q_APR_LOG_VERBOSE
#undef Q_APR_LOG
#define Q_APR_LOG true
#endif

#if Q_APR_LOG_SUPER_VERBOSE
#undef Q_APR_LOG
#undef Q_APR_LOG_VERBOSE

#define Q_APR_LOG true
#define Q_APR_LOG_VERBOSE true
#endif


#if Q_APR_LOG
#ifdef QT_DEBUG
#define Q_APR_LOG_DEBUG true
#else
#define Q_APR_LOG_DEBUG false
#endif

#ifdef QT_RELEASE
#define Q_APR_LOG_RELEASE true
#else
#define Q_APR_LOG_RELEASE false
#endif
#else
#define Q_APR_LOG_DEBUG false
#define Q_APR_LOG_RELEASE false
#endif

