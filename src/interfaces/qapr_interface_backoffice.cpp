#include "./qapr_interface_backoffice.h"
#include "../../../qorm/src/qorm_transaction.h"
#include "../../../qstm/src/qstm_envs.h"
#include "../application/qapr_startup.h"
#include "../application/qapr_macro.h"
#ifdef QTREFORCE_QRMK
#include "../../../qrpc/src/qrpc_server.h"
#include "../application/qapr_application.h"
#endif
#include <QStm>

namespace QApr {

struct ControllerInfo{
public:
    QByteArray basePath;
    QByteArray display;
    QByteArray description;
    int order=-1;
    QRpc::Controller::MethodInfoCollection invokableMethod;
};


Q_GLOBAL_STATIC_WITH_ARGS(QByteArray, APR_PROTOCOL, ());
Q_GLOBAL_STATIC_WITH_ARGS(QByteArray, APR_DNS, ());
Q_GLOBAL_STATIC_WITH_ARGS(QVariantHash, APR_HEADERS, ());
#ifdef QTREFORCE_QRMK
static int APR_PORT=0;
Q_GLOBAL_STATIC(QList<ControllerInfo>, staticInfoCache);
Q_GLOBAL_STATIC_WITH_ARGS(QByteArray, APR_CONTEXT_PATH, ());
static const auto __console="console";
#endif

static void initConsts()
{
    QStm::Envs envs;
    Q_DECLARE_VU;
    *APR_PROTOCOL=envs.value("APR_PROTOCOL").toByteArray();
    if(APR_PROTOCOL->isEmpty())
        *APR_PROTOCOL="http";

    *APR_DNS=envs.value("APR_DNS").toByteArray();
    if(APR_DNS->isEmpty())
        *APR_DNS="localhost";
#ifdef QTREFORCE_QRMK
    *APR_CONTEXT_PATH=envs.value("APR_CONTEXT_PATH").toByteArray();
    if(APR_CONTEXT_PATH->isEmpty())
        *APR_CONTEXT_PATH="/";
    APR_PORT=envs.value("APR_PORT").toInt();
    if(APR_PORT<=0)
        APR_PORT=80;
#endif
    *APR_HEADERS=vu.toHash(envs.value("APR_HEADERS"));

}

static void init()
{
    initConsts();
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

#ifdef QTREFORCE_QRMK
    QVector<const QRpc::Controller*> controllers(QRpc::Server *server){
        QVector<const QRpc::Controller*> __return;
        static QList<const QMetaObject *> metaControllers=server->controllers();

        static auto apiNameOrder=this->parent->apiNameOrder();
        static auto apiName=this->parent->apiName();

        QMultiHash<QByteArray, QRpc::Controller*> controllers;
        for(auto &m:metaControllers){
            auto metaClassName=m->className();
            if(this->staticMetaObject.className()==metaClassName)
                continue;
            QObject *object=m->newInstance(Q_ARG(QObject*, nullptr ));
            if(!object){
                aWarning()<<QString("%1: fail on newInstance").arg(metaClassName, QRpc::Controller::staticMetaObject.className());
                continue;
            }
            auto controller=dynamic_cast<QRpc::Controller*>(object);
            if(controller==nullptr){
                delete object;
                aWarning()<<QString("%1: fail on newInstance").arg(metaClassName, QRpc::Controller::staticMetaObject.className());
                continue;
            }

            auto &ann = controller->annotation();
            auto nameOrder=ann.find(apiName).toValueByteArray().trimmed().toLower();
            if(nameOrder.isEmpty())
                nameOrder=controller->metaObject()->className();
            controllers.insert(nameOrder, controller);
        }

        auto &ann = this->parent->annotation();
        auto nameOrder=ann.find(apiNameOrder).toValueStringVector();
        if(nameOrder.isEmpty())
            return __return;

        for(auto&item:nameOrder){
            auto orderName=item.toLower().trimmed();
            auto controller=controllers.value(orderName);
            if(!controller)
                continue;
            auto list=controllers.values(orderName);
            controllers.remove(orderName);
            for(auto&v:list)
                __return.append(v);
        }
        if(!controllers.isEmpty()){
            auto list=controllers.values();
            for(auto&v:list){
                qWarning()<<QStringLiteral("Controller without annotation of %1::apiNameOrder").arg(this->parent->metaObject()->className());
                __return.append(v);
            }
        }
        return __return;
    }

    void initControllersCache()
    {
        static QMutex mutexInfo;

        if(!staticInfoCache->isEmpty())
            return;

        QMutexLocker loker(&mutexInfo);
        if(!staticInfoCache->isEmpty())
            return;

        auto server=this->parent->server();
        if(!server)
            return;

        if(!staticInfoCache->isEmpty())
            return;

        auto apiName=this->parent->apiName();
        auto apiOrder=this->parent->apiOrder();
        auto apiBasePath=this->parent->apiBasePath();
        auto apiDescription=this->parent->apiDescription();

        auto controllers=this->controllers(server);

        for(auto &controller:controllers){

            const auto &ann = controller->annotation();

            auto displaName=ann.find(apiName).toValueByteArray().trimmed();
            if(displaName.isEmpty()){
                aWarning()<<QStringLiteral("%1: apiName is empty").arg(controller->metaObject()->className());
                continue;
            }

            ControllerInfo info/*=infoCache.value(display.toLower())*/;
            for(auto &method:controller->invokableMethod())
                info.invokableMethod.append(method);

            if(info.invokableMethod.isEmpty())
                continue;

            info.basePath=ann.find(apiBasePath).toValueByteArray().trimmed();
            info.display=displaName;
            info.description=ann.find(apiDescription).toValueByteArray().trimmed();
            info.order=ann.find(apiOrder).toValueLongLong();

            if(info.display.isEmpty())
                continue;

            staticInfoCache->append(info);
        }
        qDeleteAll(controllers);
        Q_SORT(staticInfoCache, [](const ControllerInfo &d1, const ControllerInfo &d2){ return d1.order<d2.order;});
    }

    QMfe::Access &initAccess(){

        initControllersCache();

        auto &request=this->parent->request();

        const auto &host=QApr::Application::i().settings().host();
        auto LOCAL_APR_PROTOCOL=APR_PROTOCOL->isEmpty()?host->protocol():(*APR_PROTOCOL);
        auto LOCAL_APR_DNS=APR_DNS->isEmpty()?host->hostName():(*APR_DNS);
        auto LOCAL_APR_PORT=APR_PORT<=0?host->port():(APR_PORT);
        auto LOCAL_APR_HEADERS=(*APR_HEADERS);
        auto LOCAL_APR_CONTEXT_PATH=APR_CONTEXT_PATH?host->basePath():(*APR_CONTEXT_PATH);

        for(auto &controller:*staticInfoCache){
            QMfe::Api api;
            QMfe::Module module;
            static const QStm::Network network;

            if(!request.authorizationHeaders().isEmpty()){
                LOCAL_APR_HEADERS.clear();
                QHashIterator<QString, QVariant> i(request.authorizationHeaders());
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
            this->access.api(api).module(module);
        }

        return this->access;
    }
#endif
};

InterfaceBackOffice::InterfaceBackOffice(QObject *parent) : QApr::Interface{parent}
{
    this->p = new InterfaceBackOfficePvt{this};
}

#ifdef QTREFORCE_QMFE

QMfe::Access &InterfaceBackOffice::qmfeAccess()
{
    return p->initAccess();
}

QVariant InterfaceBackOffice::modules()
{
    return QVariantHash{{__console, qmfeAccess().toHash()}};
}
#endif


} // namespace QApr
