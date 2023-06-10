QT += core
QT += network
QT += sql

CONFIG -= c++11
#CONFIG+=c++17
CONFIG+=silent

!defined(QTREFORCE_QAPR):QTREFORCE_QAPR=true
DEFINES-=QTREFORCE_QAPR
DEFINES+=QTREFORCE_QAPR
#QMAKE_CXXFLAGS += -DQTREFORCE_QAPR=\\\"$$QTREFORCE_QAPR\\\"

#application api
equals(QAPR_APP_TYPE,api)QAPR_APP_API=true
equals(QAPR_APP_TYPE,api)DEFINES+=QAPR_APP_API

#application service
equals(QAPR_APP_TYPE,service)QAPR_APP_SERVICE=true
equals(QAPR_APP_TYPE,service)DEFINES+=QAPR_APP_SERVICE

#application tests
equals(QAPR_APP_TYPE,tests):QAPR_APP_TESTS=true
equals(QAPR_APP_TYPE,tests):QAPR_TEST=true
equals(QAPR_APP_TYPE,tests):DEFINES+=QAPR_APP_TESTS

#SOURCES
QAPR_APPLICATION_SOURCE=$$PWD/src/application/qapr-application.pri
QAPR_SERVICES_SOURCE=$$PWD/src/services/qapr-services.pri
QAPR_SESSIONS_SOURCE=$$PWD/src/sessions/qapr-sessions.pri
QAPR_INTERFACES_SOURCE=$$PWD/src/interfaces/qapr-interfaces.pri
QAPR_MVC_SOURCE=$$PWD/src/mvc/qapr-mvc.pri
QAPR_UTIL_SOURCE=$$PWD/src/util/qapr-util.pri
QAPR_TEST_SOURCE=$$PWD/src/test/qapr-test.pri

#SOURCES CHECK
exists($$QAPR_APPLICATION_SOURCE) {QAPR_APPLICATION=true} else {QAPR_APPLICATION=false}
exists($$QAPR_SERVICES_SOURCE) {QAPR_SERVICES=true} else {QAPR_SERVICES=false}
exists($$QAPR_SESSIONS_SOURCE) {QAPR_SESSIONS=true} else {QAPR_SESSIONS=false}
exists($$QAPR_INTERFACES_SOURCE) {QAPR_INTERFACES=true} else {QAPR_INTERFACES=false}
exists($$QAPR_MVC_SOURCE) {QAPR_MVC=true} else {QAPR_MVC=false}
exists($$QAPR_UTIL_SOURCE) {QAPR_UTIL=true} else {QAPR_UTIL=false}
exists($$QAPR_TEST_SOURCE) {QAPR_TEST=true} else {QAPR_TEST=false}

equals(QAPR_NO_APPLICATION,true):  QAPR_APPLICATION=false
equals(QAPR_NO_SERVICES,true):     QAPR_SERVICES=false
equals(QAPR_NO_SESSIONS,true):     QAPR_SESSIONS=false
equals(QAPR_NO_INTERFACES,true):   QAPR_INTERFACES=false
equals(QAPR_NO_MVC,true):          QAPR_MVC=false
equals(QAPR_NO_UTIL,true):         QAPR_UTIL=false
equals(QAPR_NO_TEST,true):         QAPR_TEST=false


#SOURCES INCLUDE
equals(QAPR_APPLICATION,true):  include($$QAPR_APPLICATION_SOURCE)
equals(QAPR_SERVICES,true):     include($$QAPR_SERVICES_SOURCE)
equals(QAPR_SESSIONS,true):     include($$QAPR_SESSIONS_SOURCE)
equals(QAPR_INTERFACES,true):   include($$QAPR_INTERFACES_SOURCE)
equals(QAPR_MVC,true):          include($$QAPR_MVC_SOURCE)
equals(QAPR_UTIL,true):         include($$QAPR_UTIL_SOURCE)
equals(QAPR_TEST,true):         include($$QAPR_TEST_SOURCE)

#MODULE DEFINITION
equals(QAPR_APPLICATION,true):  DEFINES+=QAPR_APPLICATION
equals(QAPR_SERVICES,true):     DEFINES+=QAPR_SERVICES
equals(QAPR_SESSIONS,true):     DEFINES+=QAPR_SESSIONS
equals(QAPR_INTERFACES,true):   DEFINES+=QAPR_INTERFACES
equals(QAPR_MVC,true):          DEFINES+=QAPR_MVC
equals(QAPR_UTIL,true):         DEFINES+=QAPR_UTIL
equals(QAPR_TEST,true):         DEFINES+=QAPR_TEST


message(" ")
message(-QTREFORCE QAPR-MODULES-DEFINITION-ACCEPT)
message("   -QAPR_APP_TYPE: "$$QAPR_APP_TYPE)
equals(QAPR_APP_API,true)      {message("      +QAPR_APP_API............ accepted") } else { message("      +QAPR_APP_API............ ignored") }
equals(QAPR_APP_SERVICE,true)  {message("      +QAPR_APP_SERVICE........ accepted") } else { message("      +QAPR_APP_SERVICE........ ignored") }
equals(QAPR_APP_TESTS,true)    {message("      +QAPR_APP_TESTS.......... accepted") } else { message("      +QAPR_APP_TESTS.......... ignored") }
message("   -MODULES")
equals(QAPR_APPLICATION,true)  {message("      +QAPR_APPLICATION........ accepted") } else { message("      +QAPR_APPLICATION........ ignored") }
equals(QAPR_SERVICES,true)     {message("      +QAPR_SERVICES........... accepted") } else { message("      +QAPR_SERVICES........... ignored") }
equals(QAPR_SESSIONS,true)     {message("      +QAPR_SESSIONS........... accepted") } else { message("      +QAPR_SESSIONS........... ignored") }
equals(QAPR_INTERFACES,true)   {message("      +QAPR_INTERFACES......... accepted") } else { message("      +QAPR_INTERFACES......... ignored") }
equals(QAPR_MVC,true)          {message("      +QAPR_MVC................ accepted") } else { message("      +QAPR_MVC................ ignored") }
equals(QAPR_UTIL,true)         {message("      +QAPR_UTIL............... accepted") } else { message("      +QAPR_UTIL............... ignored") }
equals(QAPR_TEST,true)         {message("      +QAPR_TEST............... accepted") } else { message("      +QAPR_TEST............... ignored") }
message("   -SOURCES")
equals(QAPR_APPLICATION,true):  message("      +QAPR_APPLICATION........" $$QAPR_APPLICATION_SOURCE)
equals(QAPR_SERVICES,true):     message("      +QAPR_SERVICES..........." $$QAPR_SERVICES_SOURCE)
equals(QAPR_SESSIONS,true):     message("      +QAPR_SESSIONS..........." $$QAPR_SESSIONS_SOURCE)
equals(QAPR_INTERFACES,true):   message("      +QAPR_INTERFACES........." $$QAPR_INTERFACES_SOURCE)
equals(QAPR_MVC,true):          message("      +QAPR_MVC................" $$QAPR_MVC_SOURCE)
equals(QAPR_UTIL,true):         message("      +QAPR_UTIL..............." $$QAPR_UTIL_SOURCE)
equals(QAPR_TEST,true):         message("      +QAPR_UTIL..............." $$QAPR_TEST_SOURCE)
