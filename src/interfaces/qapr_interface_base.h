#pragma once

#include "./qapr_consts.h"
#include "./qapr_global.h"
#include "./qorm_object_db.h"
#include "./qrpc_interface_record.h"
#include <QSqlDatabase>

namespace QApr {

/**
 * @brief The Interface class
 *
 * base inicial criada para aplicacoes
 *  contem metodos e verificacao e alguns utilitarios
 */
class Q_APR_EXPORT InterfaceBase : public QRpc::QRPCInterfaceCheck
{
    Q_OBJECT
    Q_DECLARE_OBJECT()
    QORM_CONNECTION_SUPPORT
public:
    /**
     * @brief Interface
     * @param parent
     */
    Q_INVOKABLE explicit InterfaceBase(QObject *parent = nullptr);

    /**
     * @brief ~Interface
     */
    Q_INVOKABLE ~InterfaceBase();

    /**
     * @brief vu
     * @return
     */
    QT_DEPRECATED_X(" trocar o uso por Q_DECLARE_VU;")
    virtual VariantUtil &vu();

    /**
     * @brief check
     * @return
     *
     * method de verificacao simples
     */
    Q_INVOKABLE virtual QVariant check();
    Q_INVOKABLE virtual QVariant ping();
    Q_INVOKABLE virtual QVariant fullCheck();
    Q_INVOKABLE virtual QVariant connectionsCheck();
    Q_INVOKABLE virtual QVariant businessCheck();


private:
    void *p = nullptr;
};

} // namespace QApr
