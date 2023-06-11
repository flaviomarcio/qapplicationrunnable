#include "./qapr_scheduler_db.h"
#include <QTimer>
#include <QCryptographicHash>
#include <QVariant>
#include <QMutex>
#include <QMetaProperty>
#include <QSqlError>
#include "../application/qapr_application.h"
#include "../application/qapr_macro.h"
#include "../../../qorm/src/qorm_connection_pool.h"

namespace QApr{

class SchedulerDBPvt:public QObject{
public:
    SchedulerDB *parent = nullptr;
    QRpc::ServiceSetting settings;
    QOrm::ConnectionPool pool;
    explicit SchedulerDBPvt(SchedulerDB *parent=nullptr):
        parent{parent},
        pool(QApr::Application::i().connectionManager().pool())
    {
    }


};

SchedulerDB::SchedulerDB(QObject *parent):
    QApr::Scheduler{parent},
    QOrm::ObjectDbExtension{this},
    p(new SchedulerDBPvt{this})
{

}

bool SchedulerDB::invokeBefore(const QApr::SchedulerScopeGroup *scope, QMetaMethod &method)
{
    Q_UNUSED(scope)
    Q_UNUSED(method)
    QSqlDatabase db;
    if(!p->pool.get(db))
        aWarning()<<tr("%1, no connection db: %2").arg(method.name(), p->pool.lastError().text());
    return this->setConnection(db);
}

}
