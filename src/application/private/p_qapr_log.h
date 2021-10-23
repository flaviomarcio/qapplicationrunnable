#pragma once

#include "../qapr_log.h"
#include <QLoggingCategory>
#include <QDebug>
#include <QTime>
#include <QCoreApplication>

namespace QApr {

static const QHash<int,QString> makeQtMsgTypeMap(){
    QHash<int,QString> map;
    map.insert(QtDebugMsg   , "D");
    map.insert(QtWarningMsg , "W");
    map.insert(QtCriticalMsg, "C");
    map.insert(QtFatalMsg   , "F");
    map.insert(QtInfoMsg    , "I");
    map.insert(QtSystemMsg  , "S");
    qputenv("QT_LOGGING_RULES", "qt.network.ssl.warning=false;qml.debug=true;*.debug=true;*.warning=true;*.critical=true;*.info=true");
    return map;
}

static const QtMessageHandler qtMessageHandlerDefault = qInstallMessageHandler(0);
const static auto qtMsgTypeMap=makeQtMsgTypeMap();


}

