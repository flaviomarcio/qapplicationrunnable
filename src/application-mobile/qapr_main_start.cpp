#include "./qapr_main_start.h"
#include <QLocale>
#include <QtReforce/QMetaUi>

namespace QApr {

static void init(){
    QLocale::setDefault(QLocale(QLocale::Portuguese, QLocale::Brazil));
}

Q_COREAPP_STARTUP_FUNCTION(init);

MainStart::MainStart(QObject *parent):QObject(parent)
{

}

int MainStart::exec(QGuiApplication &app)
{
    MU_Application(engine);

#ifdef QT_DEBUG
    if (qEnvironmentVariableIsEmpty("QTGLESSTREAM_DISPLAY")) {
        qputenv("QT_QPA_EGLFS_PHYSICAL_WIDTH", QByteArray("213"));
        qputenv("QT_QPA_EGLFS_PHYSICAL_HEIGHT", QByteArray("120"));
    }
#endif

    const QUrl url(QStringLiteral("qrc:/main.qml"));
    QObject::connect(&engine, &QQmlApplicationEngine::objectCreated,
                     &app, [url](QObject *obj, const QUrl &objUrl) {
        if (!obj && url == objUrl)
            QGuiApplication::exit(-1);
    }, Qt::QueuedConnection);
    engine.load(url);

    return app.exec();
}

}
