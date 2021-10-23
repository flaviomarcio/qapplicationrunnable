#pragma once

#include "./qapr_global.h"
#include "./qstm_macro.h"
#include "./qorm_controller.h"
#include "./qrpc_listen_request.h"
#include "./qrpc_service_setting.h"
#include <QUuid>

namespace QApr {
class InterfaceAuthorization;

/**
 * @brief The Controller class
 *
 * class controller to business
 */
class Q_APR_EXPORT Controller : public QOrm::Controller
{
    Q_OBJECT
public:
    Q_INVOKABLE explicit Controller(QObject *parent = nullptr);
    Q_INVOKABLE ~Controller();

    /**
     * @brief interfaceRequest
     * @return
     */
    virtual InterfaceAuthorization *interfaceRequest();

    /**
     * @brief irq
     * @return
     *
     * alias for interfaceRequest
     */
    virtual InterfaceAuthorization *irq();

    /**
     * @brief transactionRollbackForce
     * @return
     *
     * se true mesmo com tudo correto ocorrerá o rollback da transacao
     */
    virtual bool transactionRollbackForce() const;
    virtual void setTransactionRollbackForce(bool value);

private:
    void *p = nullptr;
};

} // namespace QApr
