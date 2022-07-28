TARGET = QApr
TEMPLATE = lib

CONFIG += silent CONFIG += c++17

include($$PWD/../qapidoc/qapidoc.pri)
include($$PWD/../qnotation/qannotation.pri)
include($$PWD/../qstm/qstm.pri)
include($$PWD/../qorm/qorm.pri)
include($$PWD/../qrpc/qrpc.pri)
include($$PWD/qapr.pri)
