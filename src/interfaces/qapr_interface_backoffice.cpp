#include "./qapr_interface_backoffice.h"
#include "../../../qorm/src/qorm_transaction.h"
#include "../application/qapr_startup.h"
#ifdef QTREFORCE_QRMK
#include "../../../qrpc/src/qrpc_server.h"
#include "../application/qapr_application.h"
#include "../application/qapr_macro.h"
#endif
#include <QStm>

namespace QApr {

static int APR_PORT=0;
Q_GLOBAL_STATIC_WITH_ARGS(QByteArray, APR_PROTOCOL, ());
Q_GLOBAL_STATIC_WITH_ARGS(QByteArray, APR_DNS, ());
Q_GLOBAL_STATIC_WITH_ARGS(QVariantHash, APR_HEADERS, ());
#ifdef QTREFORCE_QRMK
Q_GLOBAL_STATIC_WITH_ARGS(QByteArray, APR_CONTEXT_PATH, ());
static const auto __console="console";
#endif

static void init()
{
    QStm::Envs envs;
    Q_DECLARE_VU;
    *APR_PROTOCOL=envs.value("APR_PROTOCOL").toByteArray();
    if(APR_PROTOCOL->isEmpty())
        *APR_PROTOCOL="http";

    *APR_DNS=envs.value("APR_DNS").toByteArray();
    if(APR_DNS->isEmpty())
        *APR_DNS="localhost";

    *APR_CONTEXT_PATH=envs.value("APR_CONTEXT_PATH").toByteArray();
    if(APR_CONTEXT_PATH->isEmpty())
        *APR_CONTEXT_PATH="/";

    *APR_HEADERS=vu.toHash(envs.value("APR_HEADERS"));

    APR_PORT=envs.value("APR_PORT").toInt();
    if(APR_PORT<=0)
        APR_PORT=80;
}

Q_APR_STARTUP_FUNCTION(init)

class InterfaceBackOfficePvt:public QObject
{
public:
    bool connectionDb = true;
    bool transactionRollbackForce = false;
    QRpc::Controller *parent = nullptr;
#ifdef QTREFORCE_QRMK
    QMfe::Access access;
#endif
    QOrm::Transaction transaction;
    explicit InterfaceBackOfficePvt(QRpc::Controller *parent)
        : QObject{parent},
#ifdef QTREFORCE_QRMK
          access{parent},
      #endif
          transaction{parent}
    {
        this->parent = parent;
    }

    virtual ~InterfaceBackOfficePvt()
    {
        transaction.rollback();
    }
};

InterfaceBackOffice::InterfaceBackOffice(QObject *parent) : QApr::Interface{parent}
{
    this->p = new InterfaceBackOfficePvt{this};
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
    //static QHash<QString,ControllerInfo> infoCache;
    static QVector<ControllerInfo> infoCache;

    if(infoCache.isEmpty()){
        QMutexLocker loker(&mutexInfo);
        if(infoCache.isEmpty()){
            for(auto &m:metaControllers){
                auto metaClassName=m->className();
                if(this->staticMetaObject.className()==metaClassName)
                    continue;
                QScopedPointer<QObject> sp(m->newInstance(Q_ARG(QObject*, nullptr )));
                if(!sp.data()){
                    aWarning()<<QString("%1: fail on newInstance").arg(metaClassName, QRpc::Controller::staticMetaObject.className());
                    continue;
                }

                auto controller=dynamic_cast<QRpc::Controller*>(sp.data());

                if(!controller){
                    aWarning()<<QString("%1: Invalid inherits of %2").arg(metaClassName, QRpc::Controller::staticMetaObject.className());
                    continue;
                }

                const auto &ann = controller->annotation();

                auto display=ann.find(apiName()).toValueByteArray().trimmed();
                if(display.isEmpty()){
                    aWarning()<<QString("%1: apiName is empty").arg(metaClassName);
                    continue;
                }

                ControllerInfo info/*=infoCache.value(display.toLower())*/;
                for(auto &method:controller->invokableMethod())
                    info.invokableMethod.append(method);

                if(info.invokableMethod.isEmpty())
                    continue;

                info.basePath=ann.find(apiBasePath()).toValueByteArray().trimmed();
                info.display=display;
                info.description=ann.find(apiDescription()).toValueByteArray().trimmed();

                if(info.display.isEmpty())
                    continue;

                infoCache.append(info);
            }
        }
    }
    const auto &host=QApr::Application::i().settings().host();
    auto LOCAL_APR_PROTOCOL=APR_PROTOCOL->isEmpty()?host->protocol():(*APR_PROTOCOL);
    auto LOCAL_APR_DNS=APR_DNS->isEmpty()?host->hostName():(*APR_DNS);
    auto LOCAL_APR_PORT=APR_PORT<=0?host->port():(APR_PORT);
    auto LOCAL_APR_HEADERS=(*APR_HEADERS);
    auto LOCAL_APR_CONTEXT_PATH=APR_CONTEXT_PATH?host->basePath():(*APR_CONTEXT_PATH);

    for(auto &controller:infoCache){
        QMfe::Api api;
        QMfe::Module module;
        static const QStm::Network network;



        if(!this->request().authorizationHeaders().isEmpty()){
            LOCAL_APR_HEADERS.clear();
            QHashIterator<QString, QVariant> i(this->request().authorizationHeaders());
            while(i.hasNext()){
                i.next();
                LOCAL_APR_HEADERS.insert(i.key(), i.value());
            }
        }

        api
                .basePath(controller.basePath)
                .display(controller.display)
                .description(controller.description)
                .host(
                    QMfe::Host{}
                    .protocol(LOCAL_APR_PROTOCOL)
                    .hostName(LOCAL_APR_DNS)
                    .headers(LOCAL_APR_HEADERS)
                    .port(LOCAL_APR_PORT)
                    .basePath(LOCAL_APR_CONTEXT_PATH)
                    );
        module.display(controller.display);
        QHash<QString, QMfe::Group *> groups;
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
        auto keys=groups.keys();
        keys.sort();
        for(auto&key:keys){
            auto v=groups.value(key);
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
    return QVariantHash{{__console, qmfeAccess().toHash()}};
}
#endif


} // namespace QApr
