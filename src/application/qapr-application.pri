QT += core
QT += network

#INCLUDEPATH+=$$PWD

HEADERS += \
    $$PWD/private/p_qapr_circuit_breaker.h \
    $$PWD/private/p_qapr_application.h \
    $$PWD/private/p_qapr_log_telegram.h \
    $$PWD/private/p_qapr_log_elk.h \
    $$PWD/qapr.h \
    $$PWD/qapr_app_startup.h \
    $$PWD/qapr_consts.h \
    $$PWD/qapr_global.h \
    $$PWD/qapr_host.h \
    $$PWD/qapr_circuit_breaker.h \
    $$PWD/qapr_macro.h \
    $$PWD/qapr_runnable.h \
    $$PWD/qapr_mainservice.h \
    $$PWD/qapr_application.h \
    $$PWD/qapr_setting_file.h \
    $$PWD/qapr_settings.h \
    $$PWD/qapr_startup.h


SOURCES += \
    $$PWD/private/p_qapr_application.cpp \
    $$PWD/private/p_qapr_circuit_breaker.cpp \
    $$PWD/private/p_qapr_log_elk.cpp \
    $$PWD/private/p_qapr_log_telegram.cpp \
    $$PWD/qapr_app_startup.cpp \
    $$PWD/qapr_host.cpp \
    $$PWD/qapr_circuit_breaker.cpp \
    $$PWD/qapr_runnable.cpp \
    $$PWD/qapr_mainservice.cpp \
    $$PWD/qapr_application.cpp \
    $$PWD/qapr_setting_file.cpp \
    $$PWD/qapr_settings.cpp \
    $$PWD/qapr_startup.cpp

RESOURCES += \
    $$PWD/qapr.qrc
