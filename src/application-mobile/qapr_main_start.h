#pragma once

#include <QApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>

#define QAPR_MAIN_DECLARE()\
    int main(int argc, char* argv[])\
{\
    QApplication::setAttribute(Qt::AA_EnableHighDpiScaling);\
    QApplication app(argc, argv);\
    return QApr::MainStart(nullptr).exec(app);\
    }

namespace QApr {

class MainStart : public QObject
{
    Q_OBJECT
public:
    explicit MainStart(QObject *parent = nullptr);

    static int exec(QGuiApplication &app);
};

}
