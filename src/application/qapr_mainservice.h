#pragma once

#include <QCoreApplication>

#ifdef QAPR_TEST
#include <gmock/gmock.h>
#include <QCoreApplication>
#endif

#define QAPR_MAIN_NORMAL_DECLARE() \
int main(int argc, char* argv[])\
{\
    QCoreApplication a(argc, argv);\
    return QApr::MainService(nullptr).exec(a);\
}


#define QAPR_MAIN_TEST_DECLARE() \
int main(int argc, char* argv[])\
{\
    QCoreApplication a(argc, argv);\
    testing::InitGoogleTest(&argc, argv);\
    testing::InitGoogleMock(&argc, argv);\
    return RUN_ALL_TESTS();\
}

#ifdef QAPR_TEST
#define QAPR_MAIN_DECLARE() QAPR_MAIN_NORMAL_DECLARE()
#else
#define QAPR_MAIN_DECLARE() QAPR_MAIN_TEST_DECLARE()
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
    static int exec(QCoreApplication &a);
};

}
