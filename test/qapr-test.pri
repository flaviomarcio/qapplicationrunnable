include($$PWD/../qapr.pri)

#INCLUDEPATH+=$$PWD

CONFIG += console
CONFIG -= debug_and_release
CONFIG += testcase
LIBS += -L/usr/local/lib -lgmock
LIBS += -L/usr/local/lib -lgtest

Q_APR_RUNNABLE_TEST=true
QMAKE_CXXFLAGS += -DQ_APR_RUNNABLE_TEST=\\\"$$Q_APR_RUNNABLE_TEST\\\"


HEADERS += \
    $$PWD/includes/QApr

HEADERS += \
    $$PWD/qapr_test.h \
    $$PWD/qapr_test_integration.h \
    $$PWD/qapr_test_interface.h \
    $$PWD/qapr_test_unit.h

SOURCES += \
    $$PWD/qapr_test.cpp \
    $$PWD/qapr_test_int_doc.cpp \
    $$PWD/qapr_test_int_healt_check.cpp \
    $$PWD/qapr_test_int_security.cpp \
    $$PWD/qapr_test_integration.cpp \
    $$PWD/qapr_test_interface.cpp \
    $$PWD/qapr_test_unit.cpp


