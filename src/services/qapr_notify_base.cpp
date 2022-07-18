#include "./qapr_notify_base.h"
#include "../application/qapr_application.h"
#include "../../qstm/src/qstm_util_variant.h"
#include <QVariant>

namespace QApr{

class NotifyBasePvt:public QObject{
public:
    NotifyBase*parent=nullptr;
    explicit NotifyBasePvt(NotifyBase*parent=nullptr):QObject{parent}
    {
        this->parent=parent;
    }
};


NotifyBase::NotifyBase(QObject *parent):QThread{nullptr}
{
    Q_UNUSED(parent)
    this->p = new NotifyBasePvt{this};
    this->moveToThread(this);
}

QRpc::ServiceSetting &NotifyBase::notifySetting()
{
    auto &manager=Application::i().manager();
    auto notifyName=this->notifyName();
    auto &setting=manager.setting(notifyName);
    if(!setting.enabled()){
        static QRpc::ServiceSetting __default;
        return __default;
    }
    else{
        return setting;
    }
}

void NotifyBase::run()
{
    QObject::connect(this, &NotifyBase::notifyReceived, this, &NotifyBase::onNotifyReceived);
    this->exec();
    QObject::disconnect(this, &NotifyBase::notifyReceived, this, &NotifyBase::onNotifyReceived);
}

void NotifyBase::start()
{
    if(this->notifySetting().enabled()){
        QThread::start();
        while(this->eventDispatcher()==nullptr)
            QThread::msleep(1);
    }
}

QString NotifyBase::notifyName() const
{
    auto v=this->property(QByteArrayLiteral("notifyName")).toString();
    return v;
}

void NotifyBase::setNotifyName(const QVariant &v)
{
    this->setProperty(QByteArrayLiteral("notifyName"),v);
    auto objectName=QStringLiteral("%1::%2").arg(QString(this->metaObject()->className()), QString(this->notifyName()));
    objectName=objectName.replace(QStringLiteral(":"),QStringLiteral("_")).replace(QStringLiteral("__"),QStringLiteral("_"));
    this->setObjectName(objectName);
}

const QVariant NotifyBase::resourceSettings()
{
    return QApr::Application::i().resourceSettings();
}

void NotifyBase::onNotifyReceived(const QString &channel, const QVariant &payload){

    Q_UNUSED(channel)
    auto metaObject=this->metaObject();
    auto methodName=this->notifyName();
    auto message=tr("Method not found");
    for(int methodIndex = 0; methodIndex < metaObject->methodCount(); ++methodIndex) {
        auto method = metaObject->method(methodIndex);
        if(method.parameterCount()>2)
            continue;

        if(method.name().toLower() != methodName)
            continue;

        int pType=QMetaType::UnknownType;
        if(method.parameterCount()==1){
            pType=method.parameterType(0);
        }
        else if(method.parameterCount()==2){
            pType=method.parameterType(1);
        }

        QVariant vPayload=payload;
        //se o tipo de parametro de entrar for um objeto tentaremos converter para compativilidade de objeto json/cbor
        if(pType==QMetaType::QVariantHash || pType==QMetaType::QVariantMap || pType==QMetaType::QVariantList || pType==QMetaType::QStringList){
            VariantUtil vu;
            vPayload=vu.toVariant(vPayload);
        }

        auto argChannel=QGenericArgument(QByteArrayLiteral("QVariant"), &channel);
        auto arcPayload=QGenericArgument(QByteArrayLiteral("QVariant"), &vPayload);
        QOrm::ConnectionManager manager(Application::i().connectionManager(), this);
        auto &pool=manager.pool();
        QSqlDatabase db;
        if(!pool.get(db)){
            pool.finish(db);
        }
        else if(this->setConnection(db)){
            pool.finish(db);
        }
        else{
            if(method.parameterCount()==0 && !method.invoke(this, Qt::DirectConnection)){
                message=tr("Method not called");
#if Q_APR_LOG_VERBOSE
                oDebug()<<tr("invoke method: error==")<<message;
#endif
            }
            else if(method.parameterCount()==1 && !method.invoke(this, Qt::DirectConnection, arcPayload)){
                message=tr("Method not called");
#if Q_APR_LOG_VERBOSE
                oDebug()<<tr("invoke method: error==")<<message;
#endif
            }
            else if(method.parameterCount()==2 && !method.invoke(this, Qt::DirectConnection, argChannel, arcPayload)){
                message=tr("Method not called");
#if Q_APR_LOG_VERBOSE
                oDebug()<<tr("invoke method: error==")<<message;
#endif
            }
            else{
#if Q_APR_LOG_VERBOSE
                oDebug()<<tr("invoke method:")<<method.name();
#endif
                pool.finish(db);
                break;
            }
        }
    }
}

}
