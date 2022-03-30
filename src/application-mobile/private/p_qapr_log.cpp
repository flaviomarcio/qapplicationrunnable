#include "./p_qapr_log.h"
#include <QtCore/QCoreApplication>

namespace QApr {

//QHash<int, QString>&makeQtMsgTypeMap()
//{
//    static QHash<int,QString> vHash;
//    vHash[QtDebugMsg   ]=QStringLiteral("D");
//    vHash[QtWarningMsg ]=QStringLiteral("W");
//    vHash[QtCriticalMsg]=QStringLiteral("C");
//    vHash[QtFatalMsg   ]=QStringLiteral("F");
//    vHash[QtInfoMsg    ]=QStringLiteral("I");
//    vHash[QtSystemMsg  ]=QStringLiteral("S");
//    qputenv(QByteArrayLiteral("QT_LOGGING_RULES"), QByteArrayLiteral("qt.network.ssl.warning=false;qml.debug=true;*.debug=true;*.warning=true;*.critical=true;*.info=true"));
//    return vHash;
//}

static void init()
{
    static QHash<int,QString> vHash;
    vHash[QtDebugMsg   ]=QStringLiteral("D");
    vHash[QtWarningMsg ]=QStringLiteral("W");
    vHash[QtCriticalMsg]=QStringLiteral("C");
    vHash[QtFatalMsg   ]=QStringLiteral("F");
    vHash[QtInfoMsg    ]=QStringLiteral("I");
    vHash[QtSystemMsg  ]=QStringLiteral("S");
    qputenv(QByteArrayLiteral("QT_LOGGING_RULES"), QByteArrayLiteral("qt.network.ssl.warning=false;qml.debug=true;*.debug=true;*.warning=true;*.critical=true;*.info=true"));
    qtMsgTypeMap=vHash;
}

Q_COREAPP_STARTUP_FUNCTION(init)


}

