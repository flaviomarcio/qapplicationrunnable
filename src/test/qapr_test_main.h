#pragma once

#include <QObject>
#include "../../src/application/qapr_global.h"

namespace QApr {

#define Q_APR_OBJECT_TEST_MAIN(OBJECT) \
public:\
    Q_INVOKABLE explicit OBJECT(QObject *parent=nullptr):QApr::TestMain{parent}{};

//!
//! \brief The TestMain class
//!
class Q_APR_EXPORT TestMain : public QObject
{
    Q_OBJECT
public:

    //!
    //! \brief TestMain
    //! \param parent
    //!
    Q_INVOKABLE explicit TestMain(QObject *parent = nullptr);

    //!
    //! \brief configure
    //!
    virtual void configure();

    //!
    //! \brief deconfigure
    //!
    virtual void deconfigure();

    //!
    //! \brief serviceStart
    //! \return
    //!
    virtual bool serviceStart();

    //!
    //! \brief serviceStop
    //! \return
    //!
    virtual bool serviceStop();

    //!
    //! \brief execute
    //!
    virtual void execute();

private slots:

    //!
    //! \brief run
    //!
    void run();

};

} // namespace QApr

