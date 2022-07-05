#include "./qapr_interface_backoffice.h"
#include "../application/qapr_startup.h"
#include "../../../qorm/src/qorm_transaction.h"
#include "../application/qapr_application.h"
#include "../mvc/qapr_controller_backoffice.h"
#include "../../../qrpc/src/qrpc_server.h"

namespace QApr {

Q_GLOBAL_STATIC_WITH_ARGS(QByteArray, QAPR_SERVER_PROTOCOL, ());
Q_GLOBAL_STATIC_WITH_ARGS(QByteArray, QAPR_SERVER_HOSTNAME, (getenv("QAPR_SERVER_HOSTNAME")));
Q_GLOBAL_STATIC_WITH_ARGS(QVariantHash, QAPR_SERVER_HEADERS, ());
static int QAPR_SERVER_PORT=0;

static void init()
{
    Q_DECLARE_VU;
    *QAPR_SERVER_PROTOCOL=getenv("QAPR_SERVER_PROTOCOL");
    if(QAPR_SERVER_PROTOCOL->isEmpty())
        *QAPR_SERVER_PROTOCOL="http";

    *QAPR_SERVER_HOSTNAME=getenv("QAPR_SERVER_PROTOCOL");
    if(QAPR_SERVER_HOSTNAME->isEmpty())
        *QAPR_SERVER_HOSTNAME="localhost";

    *QAPR_SERVER_HEADERS=vu.toHash(getenv("QAPR_SERVER_HEADERS"));

    QAPR_SERVER_PORT=QByteArray{getenv("QAPR_SERVER_PORT")}.toInt();
    if(QAPR_SERVER_PORT<=0)
#ifdef QT_DEBUG
        QAPR_SERVER_PORT=8084;
#else
        QAPR_SERVER_PORT=8080;
#endif
}

Q_APR_STARTUP_FUNCTION(init)

class InterfaceBackOfficePvt
{
public:
    bool connectionDb = true;
    bool transactionRollbackForce = false;
    QOrm::Transaction transaction;
    QRpc::Controller *parent = nullptr;
    QMfe::Access access;

    explicit InterfaceBackOfficePvt(QRpc::Controller *parent)
        :
          transaction{parent},
          access{parent}
    {
        this->parent = parent;
    }

    virtual ~InterfaceBackOfficePvt() { transaction.rollback(); }
};

InterfaceBackOffice::InterfaceBackOffice(QObject *parent) : QApr::Interface(parent)
{
    this->p = new InterfaceBackOfficePvt{this};
}

InterfaceBackOffice::~InterfaceBackOffice()
{
    delete p;
}

#ifdef QTREFORCE_QMFE
QMfe::Access &InterfaceBackOffice::qmfeAccess()
{
    struct ControllerInfo{
    public:
        QByteArray basePath;
        QByteArray display;
        QByteArray description;
        Controller::MethodInfoCollection invokableMethod;
    };

    static QMutex mutexInfo;
    static QList<const QMetaObject *> metaControllers=this->server()->controllers();
    static QVector<ControllerInfo> infoCache;

    if(infoCache.isEmpty()){
        mutexInfo.lock();
        if(infoCache.isEmpty()){
            for(auto &m:metaControllers){
                if(this->staticMetaObject.className()==m->className())
                    continue;
                QScopedPointer<QObject> sp(m->newInstance(Q_ARG(QObject*, nullptr )));
                if(!sp.data())
                    continue;

                auto controller=dynamic_cast<QApr::Interface*>(sp.data());

                if(!controller)
                    continue;

                const auto &nt = controller->notation();
                ControllerInfo info;
                info.invokableMethod=controller->invokableMethod();

                if(info.invokableMethod.isEmpty())
                    continue;

                info.basePath=nt.find(apiBasePath()).toValueByteArray();
                info.display=nt.find(apiName()).toValueByteArray();
                info.description=nt.find(apiDescription()).toValueByteArray();

                infoCache.append(info);
            }
        }
        mutexInfo.unlock();
    }

    for(auto&controller: infoCache){
        QMfe::Api api;
        QMfe::Module module;
        static const QStm::Network network;

        const auto &host=QApr::Application::i().settings().host();

        auto LOCAL_QAPR_SERVER_PROTOCOL=QAPR_SERVER_PROTOCOL->isEmpty()?host->protocol():(*QAPR_SERVER_PROTOCOL);
        auto LOCAL_QAPR_SERVER_HOSTNAME=QAPR_SERVER_HOSTNAME->isEmpty()?host->hostName():(*QAPR_SERVER_HOSTNAME);
        auto LOCAL_QAPR_SERVER_PORT=QAPR_SERVER_PORT<=0?host->port():(QAPR_SERVER_PORT);
        auto LOCAL_QAPR_SERVER_HEADERS=(*QAPR_SERVER_HEADERS);
        if(!this->request().authorizationHeaders().isEmpty()){
            LOCAL_QAPR_SERVER_HEADERS.clear();
            QHashIterator<QString, QVariant> i(this->request().authorizationHeaders());
            while(i.hasNext()){
                i.next();
                LOCAL_QAPR_SERVER_HEADERS.insert(i.key(), i.value());
            }
        }

        api
                .basePath(controller.basePath)
                .display(controller.display)
                .description(controller.description)
                .host(
                    QMfe::Host{}
                    .protocol(LOCAL_QAPR_SERVER_PROTOCOL)
                    .hostName(LOCAL_QAPR_SERVER_HOSTNAME)
                    .headers(LOCAL_QAPR_SERVER_HEADERS)
                    .port(LOCAL_QAPR_SERVER_PORT)
                    );
        module.display(controller.display);
        QHash<QByteArray, QMfe::Group *> groups;
        for(auto &info:controller.invokableMethod){
            if(info.group.isEmpty())
                continue;

            auto group=groups.value(info.group.toLower());
            if(!group){
                group=new QMfe::Group{this};
                group->display(info.group).description(info.group);
                groups.insert(info.group.toLower(),group);
            }

            QMfe::Path path;
            path
                    .methods(info.methods)
                    .path(info.path);

            api.path(path);
            group->option(
                        QMfe::Option{}
                        .display(info.name)
                        .description(info.description)
                        .apiUuid(path.apiUuid())
                        .pathUuid(path.uuid())
                        );
        }
        QHashIterator<QByteArray, QMfe::Group*> i(groups);
        while(i.hasNext()){
            i.next();
            auto v=i.value();
            if(!v) continue;
            module.group(*v);
            delete v;
        }
        p->access.api(api).module(module);
    }
    return p->access;
}

QVariant InterfaceBackOffice::modules()
{
    return QVariantHash{{QStringLiteral("console"), qmfeAccess().toHash()}};
}
#endif

} // namespace QApr
