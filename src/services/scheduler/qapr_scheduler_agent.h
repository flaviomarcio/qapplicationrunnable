#pragma once

#include <QObject>
#include <QThread>
#include <QVariant>
#include "../application/qapr_global.h"
#include "./private/p_qapr_scheduler_annotation.h"


#define QAPR_SCHEDULE_OBJECT(CLASS)\

#define QAPR_SCHEDULE_REGISTER(CLASS)\
static const auto CLASS##SchedulerRegistered=QApr::SchedulerAgent::reg(CLASS::staticMetaObject);

namespace QApr{
class SchedulerAgentPvt;

//!
//! \brief The SchedulerAgent class
//!
class Q_APR_EXPORT SchedulerAgent : public QThread, public PrivateQApr::SchedulerAnnotation
{
    Q_OBJECT
    QAPR_SCHEDULE_OBJECT(SchedulerAgent)
public:
    //!
    //! \brief SchedulerAgent
    //! \param parent
    //!
    Q_INVOKABLE explicit SchedulerAgent(QObject *parent=nullptr);

    //!
    //! \brief i
    //! \return
    //!
    static SchedulerAgent &i();

    //!
    //! \brief run
    //!
    void run() override;

    //!
    //! \brief start
    //! \return
    //!
    virtual bool start();

    //!
    //! \brief stop
    //! \return
    //!
    virtual bool stop();

    //!
    //! \brief serviceRegister
    //! \param metaObject
    //! \param service
    //! \return
    //!
    static bool reg(const QMetaObject &metaObject);

private:
    SchedulerAgentPvt *p=nullptr;
};

}
