QT += core
QT += network
QT += sql

CONFIG -= c++11
CONFIG += c++17
CONFIG += silent

QTREFORCE_QAPR=true
DEFINES+=QTREFORCE_QAPR

include($$PWD/src/application-mobile/qapr-application.pri)
