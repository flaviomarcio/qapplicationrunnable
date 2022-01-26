QT += core
QT += network
QT += sql

CONFIG -= c++11
CONFIG += c++17
CONFIG += silent

QTREFORCE_QAPR=true
DEFINES+=QTREFORCE_QAPR

include($$PWD/src/application/qapr-application.pri)
include($$PWD/src/services/qapr-services.pri)
include($$PWD/src/sessions/qapr-sessions.pri)
include($$PWD/src/interfaces/qapr-interfaces.pri)
include($$PWD/src/mvc/qapr-mvc.pri)
include($$PWD/src/util/qapr-util.pri)
