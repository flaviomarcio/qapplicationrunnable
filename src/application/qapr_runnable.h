#pragma once

#include <QObject>
#include <QCoreApplication>
#include "./qapr_global.h"

namespace QApr {

    class Q_APR_EXPORT Runnable : public QObject
    {
        Q_OBJECT
    public:

        static QCoreApplication&a();

        Q_INVOKABLE explicit Runnable(QObject *parent = nullptr);
        Q_INVOKABLE ~Runnable();

        virtual int run();

    public slots:
        /**
         * @brief resoucesInitialize
         * @return
         */
        Q_INVOKABLE virtual bool resoucesInitialize();

        /**
         * @brief settingInitialize
         * @return
         */
        Q_INVOKABLE virtual bool settingInitialize();

        /**
         * @brief serviceSecondPlanInitialize
         * @return
         */
        Q_INVOKABLE virtual bool serviceSecondPlanInitialize();

        /**
         * @brief serviceFirstPlanInitialize
         * @return
         */
        Q_INVOKABLE virtual bool serviceFirstPlanInitialize();

        /**
         * @brief serviceGUIInitialize
         * @return
         */
        Q_INVOKABLE virtual bool serviceGUIInitialize();

    };


}
