// $Id$
#include "Factories_Define.h"

ACE_RCSID(lib, TAO_NS_Factories_Define, "$id$")

#include "rtcorba_notify_test_export.h"
#include "Name.h"
#include "Command_Factory_T.h"
#include "RTCORBA_POA_Command.h"
#include "RTCORBA_Application_Command.h"

TAO_NS_COMMAND_FACTORY_DEFINE(TAO_RTCORBA_NOTIFY_TEST,TAO_NS_RTCORBA_Application_Command,TAO_NS_Name.application_command_factory);
TAO_NS_COMMAND_FACTORY_DEFINE(TAO_RTCORBA_NOTIFY_TEST,TAO_NS_RTCORBA_POA_Command,TAO_NS_Name.poa_command_factory);
