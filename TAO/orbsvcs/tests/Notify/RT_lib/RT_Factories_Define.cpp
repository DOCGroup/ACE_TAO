// $Id$
#include "../lib/Factories_Define.h"

ACE_RCSID(lib, TAO_NS_Factories_Define, "$id$")

#include "rt_notify_test_export.h"
#include "../lib/Name.h"
#include "../lib/Command_Factory_T.h"
#include "RT_POA_Command.h"
#include "RT_Application_Command.h"

TAO_NS_COMMAND_FACTORY_DEFINE(TAO_RT_NOTIFY_TEST,TAO_NS_RT_POA_Command,TAO_NS_Name::poa_command_factory);
TAO_NS_COMMAND_FACTORY_DEFINE(TAO_RT_NOTIFY_TEST,TAO_NS_RT_Application_Command,TAO_NS_Name::application_command_factory);

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)

template class TAO_NS_Command_Factory_T<TAO_NS_RT_POA_Command>;
template class TAO_NS_Command_Factory_T<TAO_NS_RT_Application_Command>;

#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)

#pragma instantiate TAO_NS_Command_Factory_T<TAO_NS_RT_POA_Command>
#pragma instantiate TAO_NS_Command_Factory_T<TAO_NS_RT_Application_Command>

#endif /*ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */
