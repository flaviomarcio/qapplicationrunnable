//application
#include "../application/qapr_consts.h"
#include "../application/qapr_global.h"
#include "../application/qapr_circuit_breaker.h"
#include "../application/qapr_macro.h"
#include "../application/qapr_runnable.h"
#include "../application/qapr_mainservice.h"
#include "../application/qapr_application.h"
//services
#include "../services/qapr_notify.h"
#include "../services/qapr_notify_base.h"
#include "../services/qapr_notify_dispatch.h"
#include "../services/qapr_agent.h"
#include "../services/qapr_agent_base.h"
#include "../services/qapr_server.h"
//session
#include "../sessions/qapr_session.h"
#include "../sessions/qapr_session_credential.h"
//interface
#include "../interfaces/qapr_interface.h"
#include "../interfaces/qapr_interface_backoffice.h"
#include "../interfaces/qapr_interface_maintence.h"
//mvc
#include "../mvc/qapr_controller.h"
//util
#include "../util/qapr_util_message.h"

//startUp
#include "../application/qapr_app_startup.h"


#ifdef QAPR_TEST
#include "../test/qapr_test.h"
#include "../test/qapr_test_unit.h"
#include "../test/qapr_test_integration.h"
#endif
