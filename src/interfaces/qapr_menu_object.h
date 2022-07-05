#pragma once

#include "../../../qstm/src/qstm_macro.h"
#include "../application/qapr_global.h"
#include <QObject>
#include <QVariant>
#include <QVariantHash>

namespace QApr {
class MenuObjectPvt;
//!
//! \brief The MenuObject class
//!
class Q_APR_EXPORT MenuObject : public QObject
{
    Q_OBJECT
public:
    //!
    //! \brief MenuObject
    //! \param parent
    //!
    Q_INVOKABLE explicit MenuObject(QObject *parent = nullptr);

    //!
    //! \brief MenuObject
    //! \param v
    //! \param parent
    //!
    Q_INVOKABLE explicit MenuObject(const QVariant &v, QObject *parent = nullptr);

    //!
    //! \brief build
    //! \return
    //!create QVariantMap from MenuObject
    QVariantMap build() const;

    //!
    //! \brief uuid
    //! \return
    //!
    QUuid uuid() const;

    //!
    //! \brief uuid
    //! \param v
    //! \return
    //!
    const MenuObject &uuid(const QVariant &v);

    //!
    //! \brief text
    //! \return
    //!
    QString text() const;

    //!
    //! \brief text
    //! \param v
    //! \return
    //!
    MenuObject &text(const QVariant &v);

    //!
    //! \brief route
    //! \return
    //!
    QString basePath() const;

    //!
    //! \brief basePath
    //! \param v
    //! \return
    //!
    MenuObject &basePath(const QVariant &v);

    //!
    //! \brief routeLoad
    //! \return
    //!
    QString routeLoad() const;

    //!
    //! \brief routeLoad
    //! \param v
    //! \return
    //!
    MenuObject &routeLoad(const QVariant &v);

    //!
    //! \brief routeLoader
    //! \return
    //!
    QVariantHash routeLoader() const;

    //!
    //! \brief routeLoader
    //! \param v
    //! \return
    //!
    MenuObject &routeLoader(const QVariant &v);

    //!
    //! \brief menu
    //! \return
    //!
    QVariantList menu() const;

    //!
    //! \brief menu
    //! \param v
    //! \return
    //!
    MenuObject &menu(const QVariant &v);

    //!
    //! \brief menu
    //! \param v
    //! \return
    //!
    MenuObject &menu(const MenuObject &v);

    //!
    //! \brief permission
    //! \return
    //!
    QVariantList permission() const;

    //!
    //! \brief permission
    //! \param v
    //! \return
    //!
    MenuObject &permission(const QVariant &v);

    //!
    //! \brief toHash
    //! \return
    //!
    QVariantHash toHash() const;

    //!
    //! \brief toMap
    //! \return
    //!
    QVariantMap toMap() const;

    //!
    //! \brief isEmpty
    //! \return
    //!
    virtual bool isEmpty() const;

    //!
    //! \brief isValid
    //! \return
    //!
    virtual bool isValid() const;

private:
    MenuObjectPvt *p = nullptr;
};

} // namespace QApr
