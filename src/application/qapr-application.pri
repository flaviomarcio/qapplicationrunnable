QT += core
QT += network

#INCLUDEPATH+=$$PWD

HEADERS += \
    $$PWD/qapr.h \
    $$PWD/qapr_app_startup.h \
    $$PWD/qapr_consts.h \
    $$PWD/qapr_global.h \
    $$PWD/qapr_host.h \
    $$PWD/qapr_circuit_breaker.h \
    $$PWD/qapr_macro.h \
    $$PWD/qapr_mainservice.h \
    $$PWD/qapr_application.h \
    $$PWD/qapr_settings.h \
    $$PWD/qapr_startup.h


SOURCES += \
    $$PWD/qapr_app_startup.cpp \
    $$PWD/qapr_host.cpp \
    $$PWD/qapr_circuit_breaker.cpp \
    $$PWD/qapr_mainservice.cpp \
    $$PWD/qapr_application.cpp \
    $$PWD/qapr_settings.cpp \
    $$PWD/qapr_startup.cpp

RESOURCES += \
    $$PWD/qapr.qrc
