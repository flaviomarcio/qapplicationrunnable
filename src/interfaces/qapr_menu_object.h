#pragma once

#include <QObject>
#include <QVariant>
#include <QVariantHash>
#include "./qstm_macro.h"
#include "./qapr_global.h"

namespace QApr {

    class Q_APR_EXPORT MenuObject:public QObject
    {
        Q_OBJECT
    public:
        Q_INVOKABLE explicit MenuObject(QObject *parent=nullptr);
        Q_INVOKABLE explicit MenuObject(const QVariant &v, QObject *parent=nullptr);
        Q_INVOKABLE virtual ~MenuObject();

        /**
         * @brief build
         * @return
         *
         * create QVariantMap from MenuObject
         */
        QVariantMap build() const;

        /**
         * @brief uuid
         * @return
         */
        QUuid uuid() const;
        const MenuObject &uuid(const QVariant &v);

        /**
         * @brief text
         * @return
         */
        QString text() const;
        MenuObject &text(const QVariant &v);

        /**
         * @brief route
         * @return
         */
        QString route() const;
        MenuObject &route(const QVariant &v);

        /**
         * @brief routeLoad
         * @return
         */
        QString routeLoad() const;
        MenuObject &routeLoad(const QVariant &v);

        /**
         * @brief routeLoader
         * @return
         */
        QVariantHash routeLoader() const;
        MenuObject &routeLoader(const QVariant &v);

        /**
         * @brief options
         * @return
         */
        QVariantList menu() const;
        MenuObject &menu(const QVariant &v);
        MenuObject &menu(const MenuObject &v);

        /**
         * @brief permission
         * @return
         */
        QVariantList permission() const;
        MenuObject &permission(const QVariant &v);

        /**
         * @brief toHash
         * @return
         */
        QVariantHash toHash() const;

        /**
         * @brief toMap
         * @return
         */
        QVariantMap toMap() const;

        /**
         * @brief isEmpty
         * @return
         */
        bool isEmpty() const;

        /**
         * @brief isValid
         * @return
         */
        bool isValid() const;

    private:
        void*p=nullptr;
    };

}
