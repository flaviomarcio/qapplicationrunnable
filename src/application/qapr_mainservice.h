#pragma once

#include <QGuiApplication>

#define QAPR_MAIN_DECLARE()\
int main(int argc, char* argv[])\
{\
    QGuiApplication a(argc, argv);\
    return QApr::MainService(nullptr).exec(a, argc, argv);\
}

namespace QApr{

//!
//! \brief The MainService class
//!
class MainService : public QObject
{
    Q_OBJECT
public:
    //!
    //! \brief MainService
    //! \param parent
    //!
    explicit MainService(QObject *parent = nullptr);

    //!
    //! \brief exec
    //! \param a
    //! \return
    //!
    static int exec(QGuiApplication &a);
    static int exec(QGuiApplication &a, int argc, char *argv[]);
};

}
