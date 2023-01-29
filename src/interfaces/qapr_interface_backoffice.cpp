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

static int QAPR_SERVER_PORT=0;
Q_GLOBAL_STATIC_WITH_ARGS(QByteArray, QAPR_SERVER_PROTOCOL, ());
Q_GLOBAL_STATIC_WITH_ARGS(QByteArray, QAPR_SERVER_HOSTNAME, (getenv("QAPR_SERVER_HOSTNAME")));
Q_GLOBAL_STATIC_WITH_ARGS(QVariantHash, QAPR_SERVER_HEADERS, ());
#ifdef QTREFORCE_QRMK
Q_GLOBAL_STATIC_WITH_ARGS(QByteArray, QAPR_SERVER_BASEPATH, ());
static const auto __console="console";
#endif

static void init()
{
    Q_DECLARE_VU;
    *QAPR_SERVER_PROTOCOL=getenv("QAPR_SERVER_PROTOCOL");
    if(QAPR_SERVER_PROTOCOL->isEmpty())
        *QAPR_SERVER_PROTOCOL="http";

    *QAPR_SERVER_HOSTNAME=getenv("QAPR_SERVER_PROTOCOL");
    if(QAPR_SERVER_HOSTNAME->isEmpty())
        *QAPR_SERVER_HOSTNAME="localhost";

    *QAPR_SERVER_HEADERS=vu.toHash(QByteArray(getenv("QAPR_SERVER_HEADERS")));

    QAPR_SERVER_PORT=QByteArray{getenv("QAPR_SERVER_PORT")}.toInt();
    if(QAPR_SERVER_PORT<=0)
#ifdef QT_DEBUG
        QAPR_SERVER_PORT=28084;
#else
        QAPR_SERVER_PORT=28080;
#endif
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
        mutexInfo.lock();
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
        mutexInfo.unlock();
    }
    const auto &host=QApr::Application::i().settings().host();
    auto LOCAL_QAPR_SERVER_PROTOCOL=QAPR_SERVER_PROTOCOL->isEmpty()?host->protocol():(*QAPR_SERVER_PROTOCOL);
    auto LOCAL_QAPR_SERVER_HOSTNAME=QAPR_SERVER_HOSTNAME->isEmpty()?host->hostName():(*QAPR_SERVER_HOSTNAME);
    auto LOCAL_QAPR_SERVER_PORT=QAPR_SERVER_PORT<=0?host->port():(QAPR_SERVER_PORT);
    auto LOCAL_QAPR_SERVER_HEADERS=(*QAPR_SERVER_HEADERS);
    auto LOCAL_QAPR_SERVER_BASEPATH=QAPR_SERVER_BASEPATH?host->basePath():(*QAPR_SERVER_BASEPATH);

    for(auto &controller:infoCache){
        QMfe::Api api;
        QMfe::Module module;
        static const QStm::Network network;



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
                    .basePath(LOCAL_QAPR_SERVER_BASEPATH)
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
