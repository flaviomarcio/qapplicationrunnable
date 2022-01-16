QT -= gui

CONFIG += c++17 console silent

include($$PWD/../../../qstm/qstm.pri)
include($$PWD/../../../qrpc/qrpc.pri)
include($$PWD/../../../qorm/qorm.pri)
include($$PWD/../../qapr.pri)

HEADERS += \
    $$PWD/personModel.h \
    $$PWD/personController.h \
    $$PWD/controllerMethods.h

SOURCES += \
    $$PWD/personController.cpp \
    $$PWD/controllerMethods.cpp \
    $$PWD/main.cpp

RESOURCES += \
    settings.qrc
