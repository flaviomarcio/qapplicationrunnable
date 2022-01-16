# QApr

**Application runnable, control suite with Orm and WebServer using QOrm and QRpc.**


## Working architecture
>##Support QRpc
>## Support
>>## Request
>```
>HTTP/REST, LocalSocket, TCP, UPD, WebSocket 
>```
>>## Streaming
>```
> Brokers Database, AMQP, MQTT, KAFKA
>```
>## Support QOrm
>```
> ANSI, MySQL, SQlite, Oracle, Postgres-PSQL, SQLServer-TSQL
>```

## Prerequisits
>```bash
> mkdir myproject;
> cd myproject;
> git clone git@github.com:flaviomarcio/qstm.git;
> git clone git@github.com:flaviomarcio/qrpc.git;
> git clone git@github.com:flaviomarcio/qorm.git;
> git clone git@github.com:flaviomarcio/qapr.git;
>```
>Check example in QApr/example/application


## CMake Build information

>```
>## initial CMake parameters 
>
>-GNinja
>-DCMAKE_BUILD_TYPE:STRING=Debug
>-DCMAKE_PROJECT_INCLUDE_BEFORE:PATH=%{IDE:ResourcePath}/package-manager/auto-setup.cmake
>-DQT_QMAKE_EXECUTABLE:STRING=%{Qt:qmakeExecutable}
>-DCMAKE_PREFIX_PATH:STRING=%{Qt:QT_INSTALL_PREFIX}
>-DCMAKE_C_COMPILER:STRING=%{Compiler:Executable:C}
>-DCMAKE_CXX_COMPILER:STRING=%{Compiler:Executable:Cxx}
>-DCMAKE_INSTALL_PREFIX=~/build/myproject/install/Debug
>```

>```bash
> cd qapr
> mkdir build;
> cd build;
> cmake ..;
> make;
> make install;
>```

## QMake Build information

>```bash
> cd qapr
> qmake qapr.pro
> make;
> make install;
> ls -l;
>```

## Configure QMake project

>```c++
>CONFIG += c++17
>CONFIG += console
>CONFIG += silent
>QT += gui core
>
>TEMPLATE = app
>TARGET = demo
>
>include($$PWD/../../../qstm/qstm.pri)
>include($$PWD/../../../qrpc/qrpc.pri)
>include($$PWD/../../../qorm/qorm.pri)
>include($$PWD/../../qapr/qapr.pri)
>
>HEADERS += \
>    $$PWD/controllerMethods.h \
>    $$PWD/personModel.h \
>    $$PWD/personController.h
>
>SOURCES += \
>     $$PWD/controllerMethods.cpp \
>    $$PWD/personController.cpp \
>    $$PWD/main.cpp
>
>RESOURCES += \
>    $$PWD/settings.qrc
>```

## Resource settings for QRpc/QOrm
>Settings for webservers and connection database server saved in setting.json and included in settings.qrc
>```json
>{
>   "arguments":["ws"],
>   "protocol":{
>      "default":{
>         "minThreads":1,
>         "maxThreads":2000,
>         "cleanupInterval":1000,
>         "readTimeout":60000,
>         "maxRequestSize":104857600,
>         "maxMultiPartSize":1048576000,
>         "enabled":false,
>         "sslKeyFile":"",
>         "sslCertFile":""
>      },
>      "http":{
>        "enabled":true,"port":[8888]
>      }
>   },
>   "connection":{
>       "secret": "YzUxNDFhMDA5",
>       "enviroment" : "debug",
>       "paramaters" : {
>           "debug":{
>               "driver":"QPSQL",
>               "hostName":"localhost",
>               "userName":"localuser",
>               "password":"localuser",
>               "port":5432,
>               "dataBaseName":"postgres",
>               "schemaNames":"public"
>           }
>       }
>   }
>}
>```


## Using QRpc
>## Implementation main.cpp
>Check example in QApr/example/application
>```c++
>//main implementation
>
>#include <QApr>
>
>QAPR_MAIN_DECLARE()
>```
>## Webservices methods implementation
>Check example in QApr/example/application
>
>```c++
>#include <QRpc/Server>
>
>//!
>//! \brief The ControllerMethods class
>//!
>//! inhetihed of QRpc::QRPCController
>class ControllerMethods : public QRpc::QRPCController
>{
>    Q_OBJECT
>public:
>    //!
>    //! \brief ControllerMethods
>    //! \param parent
>    //!
>    //! necessary Q_INVOKABLE
>    Q_INVOKABLE explicit ControllerMethods(QObject *parent = nullptr);
>
>    //!
>    //! \brief authorization
>    //! \return
>    //!
>    //! authorization validations
>    //! check more authorization methods:
>    //!     canAuthorization
>    //!     beforeAuthorization
>    //!     afterAuthorization
>    //!
>    virtual bool authorization();
>
>    //!
>    //! \brief listObjects
>    //! \return
>    //!
>    //! method to return object list, necessary Q_INVOKABLE
>    //! check more Invoke methods:
>    //!     requestBeforeInvoke
>    //!     requestAfterInvoke
>    Q_INVOKABLE QVariant listObjects();
>
>};
>
>//auto register interface
>QRPC_CONTROLLER_AUTO_REGISTER(ControllerMethods)   
>```
>## Example of request
>Check example in QRpc/example/server-rest
>```bash
> curl -i http://localhost:8888/listObjects?limit=1
> ```
>```bash
>HTTP/1.1 200 (OK)
>Vary: Accept-Encoding, Origin
>Date: Sat Jan 15 18:50:02 2022
>Content-Length: 59
>Server: Undef
>```
>
>```json
>[{"index":1,"uuid":"ba01138f-f7b0-4874-a879-caa591ab9e55"}]
>```

## Using QOrm
>## Implementation PersonModel Mapper
>Check example in QApr/example/application
>
>```c++
>#include <QOrm>
>//!
>//! \brief The PersonModel class
>//! 
>//! inherits of QOrm::Model
>class PersonModel : public QOrm::Model
>{
>    Q_OBJECT
>public:
>    QORM_MODEL(PersonModel)//make default methods for PersonModel
>    QORM_DECLARE_MODEL_DESCRIPTION(tr("Person"))//description
>    QORM_DECLARE_TABLE_SCHEMA()//declare schema name in database
>    QORM_DECLARE_TABLE(tab000,_,person)//table in database
>    QORM_DECLARE_TABLE_PRIMARY_KEY(uuid)//inform PK name
>    QORM_DECLARE_TABLE_FOREIGN_KEY(parent_uuid)//declare FK
>    QORM_DESCRIPTOR_ORDERBY(name)//declare default order by
>public:
>    QORM_DECLARE_PROPERTY(QUuid  , uuid         , uuidGenerator()   )//declare field and default value
>    QORM_DECLARE_PROPERTY(QDate  , dt_record    , QDate()           )//declare field and default value
>    QORM_DECLARE_PROPERTY(QUuid  , parent_uuid  , QUuid()           )//declare field and default value
>    QORM_DECLARE_PROPERTY(QString, name         , QString()         )//declare field and default value
>    QORM_DECLARE_PROPERTY(bool   , enabled      , false             )//declare field and default value
>public:
>    QORM_MODEL_CONSTRUCTOR(PersonModel, QOrm::Model)//contructors for PersonModel
>};
>
>//REGISTER MODEL AND DECLARE CRUD AND DAO
>QORM_DECLARE_CRUD(PersonModel)
>```

>## Implementation PersonController
>Check example in QApr/example/application
>
>```c++
>#include <QOrm>
>
>//!
>//! \brief The PersonController class
>//!
>//! inherits of QOrm::Controller
>class PersonController : public QOrm::Controller
>{
>    Q_OBJECT
>public:
>    //!
>    //! \brief personController
>    //! \param parent
>    //!
>    //! constructor controller to pass current connection
>    QORM_CONTROLLER_CONSTRUCTOR(PersonController)
>
>    //!
>    //! \brief crud
>    //! \param vBody
>    //! \return
>    //! example of CRUD
>    ResultValue &crud(const QVariant &vBody);
>
>    .....more
>
>};
>```

>## Using QOrm::ModelDao
>Check example in QApr/example/application
>## Using CRUD methods
>
>```c++
>ResultValue &PersonController::crud(const QVariant &vBody)
>{
>    //Person CRUD
>    PersonModelCRUD crud(vBody, this);
>    return this->lr(crud.crudify());//return success/error and created object
>}
>```
>## Using  upsert/insert/update/remove methods
>
>```c++
>ResultValue &PersonController::upsert(const QVariant &v)
>{
>    //Person generic dao
>    PersonModelGDao dao(this);
>    PersonModel model(v);
>
>    //insert or update record
>    if(!dao.upsert(model))
>        return this->lr();//return error
>
>    return this->lr(model.toHash());//return success and created object
>}
>
>ResultValue &PersonController::insert(const QVariant &v)
>{
>    //Person generic dao
>    PersonModelGDao dao(this);
>
>    //remove record
>    return this->lr(dao.insert(v));//return successful/error
>}
>
>ResultValue &PersonController::update(const QVariant &v)
>{
>    //Person generic dao
>    PersonModelGDao dao(this);
>
>    //remove record
>    return this->lr(dao.update(v));//return successful/error
>}
>
>ResultValue &PersonController::remove(const QVariant &v)
>{
>    //Person generic dao
>    PersonModelGDao dao(this);
>
>    //remove record
>    return this->lr(dao.remove(v));//return successful/error
>}
>```
>## Using record/recordList methods
>```c++
>ResultValue &PersonController::record(const QVariant &v)
>{
>    //Person generic dao
>    PersonModelGDao dao(this);
>
>    //get record
>    return this->lr(dao.record(v));//return success/error and record
>}
>
>ResultValue &PersonController::recordList(const QVariant &v)
>{
>    //Person generic dao
>    PersonModelGDao dao(this);
>
>    //get record
>    return this->lr(dao.recordList(v));//return success/error and record
>}
>```
>
>## Using  query buider strategy
>```c++
>ResultValue &PersonController::listPerson(const QVariant &v)
>{
>    QOrm::Query query(this);
>
>    auto&select=query.builder().select();
>
>    select
>        .fields(PersonModelR)
>        .from(PersonModelR);
>
>    select
>        .where()
>        .equal(PersonModelM.parent_uuid_field(), v);
>
>    //execute query
>    if(!query.exec())
>        return this->lr();//return error
>
>    //make record list
>    return this->lr(query.makeRecordList());//return successful and record list
>}
>
>ResultValue &PersonController::listPersonParent(const QVariant &v)
>{
>    QOrm::Query query(this);
>
>    auto&select=query.builder().select();
>
>    select
>        .fields()
>        .f(PersonModelM.uuid_field("a"))
>        .f(PersonModelM.parent_uuid_field("a"))
>        .f(PersonModelM.name_field("a"))
>        .f(PersonModelM.enabled_field("a"));
>    select
>        .from(PersonModelR, "a");
>
>    select
>        .joinInner(PersonModelR).alias("b")
>        .equal(PersonModelM.parent_uuid_field("b"), PersonModelM.parent_uuid_field("a"));
>
>    select
>        .where()
>        .equal(PersonModelM.parent_uuid_field("a"), v);
>
>    //execute query
>    if(!query.exec())
>        return this->lr();//return error
>
>    //make record list
>    return this->lr(query.makeRecordList());//return successful and record list
>}
>```
>
>## Using  multi tasks execute
>```c++
>ResultValue &PersonController::runMultiTasks()
>{
>    //lamba to async update record
>
>    auto errorUpdateDate=[](QSqlDatabase&, const QVariant&)
>    {
>        qWarning()<<"It\'s not OK";
>        //..action to error
>        return QVariant();
>    };
>
>    auto successUpdateDate=[](QSqlDatabase&, const QVariant&)
>    {
>        qWarning()<<"it\'s OK";
>        //..action to success
>        return QVariant();
>    };
>
>    auto executeUpdateDate=[](QSqlDatabase&connection, const QVariant&record){
>        //Person generic dao
>        PersonModelGDao dao;
>        dao.setConnection(connection);//individual connection to task
>        PersonModel model(record);//load body model
>        model.set_dt_record(QDate::currentDate());//update dt
>
>        if(!dao.upsert(model))//insert/update person
>            return dao.lr().toHash();//record body error
>
>        return model.toHash();//return new body object
>    };
>
>    PersonModelGDao dao(this);//dao to current connection
>    auto vList=dao.recordList().resultToList();//list all personModel record
>
>    QOrm::TaskRunner taskRunner(this);//task runner controller
>    taskRunner
>        .slotCount(QThread::idealThreadCount())//maximum thread in actual machine
>        .values(vList)//personModel records
>        .onExecute(executeUpdateDate)//lambda to async execute
>        .onFailed(errorUpdateDate)//lambda to error notify
>        .onSuccess(successUpdateDate)//lambda to success notify
>        .start();//wait to finish
>
>    return this->lr(taskRunner.lr());//return successful/error
>
>}
>```