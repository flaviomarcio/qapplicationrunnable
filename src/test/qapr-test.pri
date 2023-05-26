
LC_ALL=en_US.UTF-8

CONFIG += console

CONFIG -= debug_and_release
CONFIG += testcase

LIBS += -L/usr/local/lib -lgmock
LIBS += -L/usr/local/lib -lgtest

QT += testlib

HEADERS += \
    $$PWD/qapr_test.h

SOURCES += \
    $$PWD/qapr_test.cpp


