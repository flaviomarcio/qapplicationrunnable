QT += core
QT += network
QT += sql

CONFIG -= c++11
CONFIG+=c++17
CONFIG+=silent

QTREFORCE_QAPR=true
DEFINES+=QTREFORCE_QAPR

#SOURCES
QAPR_APPLICATION_SOURCE=$$PWD/src/application/qapr-application.pri
QAPR_SERVICES_SOURCE=$$PWD/src/services/qapr-services.pri
QAPR_SESSIONS_SOURCE=$$PWD/src/sessions/qapr-sessions.pri
QAPR_INTERFACES_SOURCE=$$PWD/src/interfaces/qapr-interfaces.pri
QAPR_MVC_SOURCE=$$PWD/src/mvc/qapr-mvc.pri
QAPR_UTIL_SOURCE=$$PWD/src/util/qapr-util.pri

#SOURCES CHECK
exists($$QAPR_UTIL_SOURCE)          {QAPR_APPLICATION=true} else {QAPR_APPLICATION=false}
exists($$QAPR_MVC_SOURCE)           {QAPR_SERVICES=true} else {QAPR_SERVICES=false}
exists($$QAPR_INTERFACES_SOURCE)    {QAPR_SESSIONS=true} else {QAPR_SESSIONS=false}
exists($$QAPR_SESSIONS_SOURCE)      {QAPR_INTERFACES=true} else {QAPR_INTERFACES=false}
exists($$QAPR_SERVICES_SOURCE)      {QAPR_MVC=true} else {QAPR_MVC=false}
exists($$QAPR_APPLICATION_SOURCE)   {QAPR_UTIL=true} else {QAPR_UTIL=false}

#SOURCES INCLUDE
equals(QAPR_APPLICATION,true):  include($$QAPR_APPLICATION_SOURCE)
equals(QAPR_SERVICES,true):     include($$QAPR_SERVICES_SOURCE)
equals(QAPR_SESSIONS,true):     include($$QAPR_SESSIONS_SOURCE)
equals(QAPR_INTERFACES,true):   include($$QAPR_INTERFACES_SOURCE)
equals(QAPR_MVC,true):          include($$QAPR_MVC_SOURCE)
equals(QAPR_UTIL,true):         include($$QAPR_UTIL_SOURCE)

#MODULE DEFINITION
equals(QAPR_APPLICATION,true):  DEFINES+=QAPR_APPLICATION
equals(QAPR_SERVICES,true):     DEFINES+=QAPR_SERVICES
equals(QAPR_SESSIONS,true):     DEFINES+=QAPR_SESSIONS
equals(QAPR_INTERFACES,true):   DEFINES+=QAPR_INTERFACES
equals(QAPR_MVC,true):          DEFINES+=QAPR_MVC
equals(QAPR_UTIL,true):         DEFINES+=QAPR_UTIL

message(" ")
message(-QTREFORCE QAPR-MODULES-DEFINITION-ACCEPT)
message("   -MODULES")
equals(QAPR_APPLICATION,true)  {message("      +QAPR_APPLICATION........ accepted") } else { message("      +QAPR_APPLICATION........ ignored") }
equals(QAPR_SERVICES,true)     {message("      +QAPR_SERVICES........... accepted") } else { message("      +QAPR_SERVICES........... ignored") }
equals(QAPR_SESSIONS,true)     {message("      +QAPR_SESSIONS........... accepted") } else { message("      +QAPR_SESSIONS........... ignored") }
equals(QAPR_INTERFACES,true)   {message("      +QAPR_INTERFACES......... accepted") } else { message("      +QAPR_INTERFACES......... ignored") }
equals(QAPR_MVC,true)          {message("      +QAPR_MVC................ accepted") } else { message("      +QAPR_MVC................ ignored") }
equals(QAPR_UTIL,true)         {message("      +QAPR_UTIL............... accepted") } else { message("      +QAPR_UTIL............... ignored") }
message("   -SOURCES")
equals(QAPR_APPLICATION,true):  message("      +QAPR_APPLICATION........" $$QAPR_APPLICATION_SOURCE)
equals(QAPR_SERVICES,true):     message("      +QAPR_SERVICES..........." $$QAPR_SERVICES_SOURCE)
equals(QAPR_SESSIONS,true):     message("      +QAPR_SESSIONS..........." $$QAPR_SESSIONS_SOURCE)
equals(QAPR_INTERFACES,true):   message("      +QAPR_INTERFACES........." $$QAPR_INTERFACES_SOURCE)
equals(QAPR_MVC,true):          message("      +QAPR_MVC................" $$QAPR_MVC_SOURCE)
equals(QAPR_UTIL,true):         message("      +QAPR_UTIL..............." $$QAPR_UTIL_SOURCE)
