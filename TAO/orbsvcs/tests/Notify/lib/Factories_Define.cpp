// $Id$
#include "Factories_Define.h"

ACE_RCSID(lib, TAO_NS_Factories_Define, "$id$")

#include "Name.h"
#include "Command_Factory_T.h"

#include "EventChannel_Command.h"
#include "Application_Command.h"
#include "SupplierAdmin_Command.h"
#include "ConsumerAdmin_Command.h"
#include "PeriodicSupplier_Command.h"
#include "PeriodicConsumer_Command.h"
#include "Filter_Command.h"

TAO_NS_COMMAND_FACTORY_DEFINE(TAO_NOTIFY_TEST,TAO_NS_EventChannel_Command,TAO_NS_Name.event_channel_command_factory);
TAO_NS_COMMAND_FACTORY_DEFINE(TAO_NOTIFY_TEST,TAO_NS_Application_Command,TAO_NS_Name.application_command_factory);
TAO_NS_COMMAND_FACTORY_DEFINE(TAO_NOTIFY_TEST,TAO_NS_SupplierAdmin_Command,TAO_NS_Name.supplier_admin_command_factory);
TAO_NS_COMMAND_FACTORY_DEFINE(TAO_NOTIFY_TEST,TAO_NS_ConsumerAdmin_Command,TAO_NS_Name.consumer_admin_command_factory);
TAO_NS_COMMAND_FACTORY_DEFINE(TAO_NOTIFY_TEST,TAO_NS_PeriodicSupplier_Command,TAO_NS_Name.periodic_supplier_command_factory);
TAO_NS_COMMAND_FACTORY_DEFINE(TAO_NOTIFY_TEST,TAO_NS_PeriodicConsumer_Command,TAO_NS_Name.periodic_consumer_command_factory);
TAO_NS_COMMAND_FACTORY_DEFINE(TAO_NOTIFY_TEST,TAO_NS_Filter_Command,TAO_NS_Name.filter_command_factory);
