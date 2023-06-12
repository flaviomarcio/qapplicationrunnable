#pragma once

#include <QUuid>
#include "../application/qapr_global.h"
#include "../../../qorm/src/qorm_controller.h"

namespace QApr {
class ControllerPvt;
class Interface;

//!
//! \brief The Controller class
//!class controller to business
class Q_APR_EXPORT Controller : public QOrm::Controller
{
    Q_OBJECT
public:
    //!
    //! \brief Controller
    //! \param parent
    //!
    Q_INVOKABLE explicit Controller(QObject *parent = nullptr);

    //!
    //! \brief resultInfo
    //! \return
    //!
    Q_INVOKABLE virtual const QVariant resultInfo();

    //!
    //! \brief interfaceRequest
    //! \return
    //!
    virtual Interface *interfaceRequest();

    //!
    //! \brief irq
    //! \return
    //!alias for interfaceRequest
    virtual Interface *irq();

    //!
    //! \brief transactionRollbackForce
    //! \return
    //!se true mesmo com tudo correto ocorrerá o rollback da transacao
    virtual bool transactionRollbackForce() const;

    //!
    //! \brief setTransactionRollbackForce
    //! \param value
    //!se true mesmo com tudo correto ocorrerá o rollback da transacao
    virtual void setTransactionRollbackForce(bool value);

private:
    ControllerPvt *p = nullptr;
};

}
