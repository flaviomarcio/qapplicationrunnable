#pragma once

#include "./qapr_session_credential.h"
#include <QMutex>
#include <QThread>
#include <QCoreApplication>

namespace QApr {

static QHash<QThread*, QObject*> static_session_instance;

static QMutex static_session_instance_mutex;

class Session : public QStm::Object
{
    Q_OBJECT
public:
    Q_INVOKABLE explicit Session(QObject *parent = nullptr) : QStm::Object(parent), p_credential(parent){
        this->session_thread=QThread::currentThread();
    }

    Q_INVOKABLE ~Session(){
    }

    /**
     * @brief instance
     * @return
     */
    Session&instance(){
        return*dynamic_cast<Session*>(this->session);
    }

    /**
     * @brief i
     * @return
     */
    Session&i(){
        return*dynamic_cast<Session*>(this->session);
    }

    /**
     * @brief credentials
     * @return
     */
    virtual SessionCredential&credential(){
        return this->p_credential;
    }

    void init(){
        auto session=dynamic_cast<Session*>(static_session_instance.value(this->session_thread));
        if(this->session==nullptr){
            QMutexLocker locker(&static_session_instance_mutex);
            this->session=dynamic_cast<QStm::Object*>(static_session_instance.value(this->session_thread));
            if(this->session==nullptr){
                static_session_instance.insert(session_thread, session);
                this->session=dynamic_cast<QStm::Object*>(static_session_instance.value(this->session_thread));
            }
        }
    }

private:
    SessionCredential p_credential;
    QThread*session_thread=nullptr;
    QStm::Object*session=nullptr;
};

}
