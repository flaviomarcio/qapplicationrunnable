TARGET = QApr
TEMPLATE = lib

CONFIG += c++17 silent

include($$PWD/../qstm/qstm.pri)
include($$PWD/../qorm/qorm.pri)
include($$PWD/../qrpc/qrpc.pri)
include($$PWD/qapr.pri)
