QT += core
QT += network

#INCLUDEPATH+=$$PWD

HEADERS += \
    $$PWD/../../includes/mobile/QApr

HEADERS += \
    $$PWD/private/p_qapr_application.h \
    $$PWD/private/p_qapr_log.h \
    $$PWD/qapr_consts.h \
    $$PWD/qapr_global.h \
    $$PWD/qapr_log.h \
    $$PWD/qapr_macro.h \
    $$PWD/qapr_runnable.h \
    $$PWD/qapr_mainservice.h \
    $$PWD/qapr_application.h \


SOURCES += \
    $$PWD/qapr_log.cpp \
    $$PWD/qapr_runnable.cpp \
    $$PWD/qapr_mainservice.cpp \
    $$PWD/qapr_application.cpp \
