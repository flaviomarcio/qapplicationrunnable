#pragma once

#include <QObject>
#include <QCoreApplication>
#include "./qapr_global.h"

namespace QApr {

    class Q_APR_EXPORT CircuitBreaker : public QObject
    {
        Q_OBJECT
    public:
        Q_INVOKABLE explicit CircuitBreaker(QObject *parent = nullptr);
        Q_INVOKABLE ~CircuitBreaker();

        /**
         * @brief settings
         * @return
         */
        QVariantHash settings()const;
        CircuitBreaker&settings(const QVariantHash&v);
        CircuitBreaker&setSettings(const QVariantHash&v);

        /**
         * @brief start
         * @return
         */
        virtual bool start();

        /**
         * @brief stop
         * @return
         */
        virtual bool stop();

        /**
         * @brief print
         */
        virtual void print() const;

        /**
         * @brief isRunning
         * @return
         */
        virtual bool isRunning();
    private:
        void*p=nullptr;
    };

}
