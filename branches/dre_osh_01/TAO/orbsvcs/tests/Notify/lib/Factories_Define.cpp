// $Id$
#include "Factories_Define.h"

ACE_RCSID(lib, TAO_NS_Factories_Define, "$id$")

#include "Name.h"
#include "Command_Factory_T.h"

#include "EventChannel_Command.h"
#include "Application_Command.h"
#include "SupplierAdmin_Command.h"
#include "ConsumerAdmin_Command.h"
#include "Periodic_Supplier_Command.h"
#include "Periodic_Consumer_Command.h"
#include "Filter_Command.h"

TAO_NS_COMMAND_FACTORY_DEFINE(TAO_NOTIFY_TEST,TAO_NS_EventChannel_Command,TAO_NS_Name::event_channel_command_factory);
TAO_NS_COMMAND_FACTORY_DEFINE(TAO_NOTIFY_TEST,TAO_NS_Application_Command,TAO_NS_Name::application_command_factory);
TAO_NS_COMMAND_FACTORY_DEFINE(TAO_NOTIFY_TEST,TAO_NS_SupplierAdmin_Command,TAO_NS_Name::supplier_admin_command_factory);
TAO_NS_COMMAND_FACTORY_DEFINE(TAO_NOTIFY_TEST,TAO_NS_ConsumerAdmin_Command,TAO_NS_Name::consumer_admin_command_factory);
TAO_NS_COMMAND_FACTORY_DEFINE(TAO_NOTIFY_TEST,TAO_NS_Periodic_Supplier_Command,TAO_NS_Name::periodic_supplier_command_factory);
TAO_NS_COMMAND_FACTORY_DEFINE(TAO_NOTIFY_TEST,TAO_NS_Periodic_Consumer_Command,TAO_NS_Name::periodic_consumer_command_factory);
TAO_NS_COMMAND_FACTORY_DEFINE(TAO_NOTIFY_TEST,TAO_NS_Filter_Command,TAO_NS_Name::filter_command_factory);

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)

template class TAO_NS_Command_Factory_T<TAO_NS_EventChannel_Command>;
template class TAO_NS_Command_Factory_T<TAO_NS_Application_Command>;
template class TAO_NS_Command_Factory_T<TAO_NS_SupplierAdmin_Command>;
template class TAO_NS_Command_Factory_T<TAO_NS_ConsumerAdmin_Command>;
template class TAO_NS_Command_Factory_T<TAO_NS_Periodic_Supplier_Command>;
template class TAO_NS_Command_Factory_T<TAO_NS_Periodic_Consumer_Command>;
template class TAO_NS_Command_Factory_T<TAO_NS_Filter_Command>;

#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)

#pragma instantiate TAO_NS_Command_Factory_T<TAO_NS_EventChannel_Command>
#pragma instantiate TAO_NS_Command_Factory_T<TAO_NS_Application_Command>
#pragma instantiate TAO_NS_Command_Factory_T<TAO_NS_SupplierAdmin_Command>
#pragma instantiate TAO_NS_Command_Factory_T<TAO_NS_ConsumerAdmin_Command>
#pragma instantiate TAO_NS_Command_Factory_T<TAO_NS_Periodic_Supplier_Command>
#pragma instantiate TAO_NS_Command_Factory_T<TAO_NS_Periodic_Consumer_Command>
#pragma instantiate TAO_NS_Command_Factory_T<TAO_NS_Filter_Command>

#endif /*ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */
