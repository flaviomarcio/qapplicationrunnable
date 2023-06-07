#include "./qapr_scheduler.h"
#include <QTimer>
#include <QCryptographicHash>
#include <QVariant>
#include <QMutex>
#include <QMetaProperty>

namespace QApr{

class SchedulerPvt:public QObject{
public:
    Scheduler *parent = nullptr;
    QRpc::ServiceSetting settings;
    explicit SchedulerPvt(Scheduler *parent=nullptr):QObject{parent}
    {
        this->parent=parent;
    }
};


Scheduler::Scheduler(QObject *parent):QOrm::ObjectDb{parent}, PrivateQApr::SchedulerAnnotation{this}
{
    Q_UNUSED(parent)
    this->p = new SchedulerPvt{this};
}

QRpc::ServiceSetting &Scheduler::settings()const
{
    return p->settings;
}

}
