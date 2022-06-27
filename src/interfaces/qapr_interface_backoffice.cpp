#include "./qapr_interface_backoffice.h"
#include "../../../qorm/src/qorm_transaction.h"
#include "../application/qapr_application.h"
#include "../mvc/qapr_controller_backoffice.h"
#include "../../../qrpc/src/qrpc_server.h"

namespace QApr {

#define dPvt() auto &p = *reinterpret_cast<InterfaceBackOfficePvt *>(this->p)

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
        api
                .basePath(controller.basePath)
                .display(controller.display)
                .description(controller.description)
                .host(
                    QMfe::Host{}
                    .protocol(this->request().requestProtocol())
                    .headers(this->request().requestHeader())
                    .hostName(qsl("${HOST}"))
                    .port(this->request().requestPort())
                    );

        QHash<QByteArray, QMfe::Group *> groups;
        for(auto &info:controller.invokableMethod){
            for(auto &method: info.methods){

                if(info.group.isEmpty())
                    continue;

                auto group=groups.value(info.group.toLower());
                if(!group){
                    group=new QMfe::Group{};
                    group->display(info.group).description(info.group);
                }

                api.path(
                            QMfe::Path{}
                            .method(method)
                            .path(info.path)
                         );
                group->option(
                            QMfe::Option{}
                            .display(info.name)
                            .description(info.description)
                            .apiUuid(api)
                            .path(info.name)
                            );
            }
        }
        QHashIterator<QByteArray, QMfe::Group*> i(groups);
        while(i.hasNext()){
            i.next();
            auto v=i.value();
            if(!v) continue;
            module.group(*v);
            delete v;
        }
    }
    return p->access;
}

QVariant InterfaceBackOffice::modules()
{
    return QVariantHash{{QStringLiteral("console"), qmfeAccess().toHash()}};
}
#endif

//const QVariant InterfaceBackOffice::sessionAccount()
//{
//    QRPC_METHOD_CHECK_GET();
//    ControllerBackOffice controller(this);
//    if (!controller.sessionAccount())
//        this->rq().co(controller.lr().sc());
//    else
//        return controller.lr().resultVariant();
//    QRPC_RETURN_VARIANT();
//}

//const QVariant InterfaceBackOffice::enviroment()
//{
//    QRPC_METHOD_CHECK_GET();
//    ControllerBackOffice controller(this);
//    if (!controller.enviroment())
//        this->rq().co(controller.lr().sc());
//    else
//        return controller.lr().resultVariant();
//    QRPC_RETURN_VARIANT();
//}

//const QVariant InterfaceBackOffice::menu()
//{
//    QRPC_METHOD_CHECK_GET();
//    ControllerBackOffice controller(this);
//    if (!controller.rootObject())
//        this->rq().co(controller.lr().sc());
//    else
//        return controller.lr().resultVariant();
//    QRPC_RETURN_VARIANT();
//}

//const QVariant InterfaceBackOffice::menuInfo()
//{
//    QRPC_METHOD_CHECK_GET();
//    QRPC_V_SET_BODY_HASH(body);
//    ControllerBackOffice controller(this);
//    if (!controller.menuInfo(body))
//        this->rq().co(controller.lr().sc());
//    else
//        return controller.lr().resultVariant();
//    QRPC_RETURN_VARIANT();
//}

//const QVariant InterfaceBackOffice::rootObject()
//{
//    QRPC_METHOD_CHECK_GET();
//    ControllerBackOffice controller(this);
//    if (!controller.rootObject())
//        this->rq().co(controller.lr().sc());
//    else
//        return controller.lr().resultVariant();
//    QRPC_RETURN_VARIANT();
//}

} // namespace QApr
