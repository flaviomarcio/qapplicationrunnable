
CONFIG += console
CONFIG -= debug_and_release
CONFIG += testcase

LC_ALL=en_US.UTF-8

LIBS += -L/usr/local/lib -lgmock
LIBS += -L/usr/local/lib -lgtest

HEADERS += \
    $$PWD/qapr_test.h \
    $$PWD/qapr_test_integration.h \
    $$PWD/qapr_test_unit.h

SOURCES += \
    $$PWD/qapr_test.cpp \
    $$PWD/qapr_test_integration.cpp \
    $$PWD/qapr_test_unit.cpp


