#pragma once

#ifdef QT_GUI_LIB
#define Q_APR_APP_CLASS QGuiApplication
#include <QGuiApplication>
#else
#define Q_APR_APP_CLASS QCoreApplication
#include <QCoreApplication>
#endif

#ifdef QAPR_APP_TESTS
#define QAPR_MAIN_DECLARE()
#else
#define QAPR_MAIN_DECLARE()\
int main(int argc, char* argv[])\
{\
    Q_APR_APP_CLASS a(argc, argv);\
    return QApr::MainService(nullptr).exec(a, argc, argv);\
}
#endif

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
    static int exec(Q_APR_APP_CLASS &a);
    static int exec(Q_APR_APP_CLASS &a, int argc, char *argv[]);
};

}
