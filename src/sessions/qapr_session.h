#pragma once


#include <QMutex>
#include <QThread>
#include <QCoreApplication>
#include "../../../qstm/src/qstm_object.h"
#include "../application/qapr_global.h"
#include "./qapr_session_credential.h"

namespace QApr {

//!
//! \brief The Session class
//!
class Q_APR_EXPORT Session : public QStm::Object
{
    Q_OBJECT
public:
    //!
    //! \brief Session
    //! \param parent
    //!
    Q_INVOKABLE explicit Session(QObject *parent = nullptr);

    //!
    //! \brief instance
    //! \return
    //!
    Session &instance();

    //!
    //! \brief i
    //! \return
    //!
    Session &i();

    //!
    //! \brief credential
    //! \return
    //!
    virtual SessionCredential&credential();

    //!
    //! \brief init
    //!
    void init();

private:
    SessionCredential p_credential;
    QThread*session_thread=nullptr;
    QStm::Object*session=nullptr;
};

}
