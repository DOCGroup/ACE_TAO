// $Id$

#include "Name.h"

ACE_RCSID(lib, TAO_Name, "$id$")

#define TAO_NS_DECLARE_NAME(target, name) \
const char* const TAO_NS_Name::target = ACE_TEXT (name);

TAO_NS_DECLARE_NAME (command_builder,"Command_Builder")
TAO_NS_DECLARE_NAME (root_poa,"RootPOA")
TAO_NS_DECLARE_NAME (event_channel_factory,"NotifyEventChannelFactory");
TAO_NS_DECLARE_NAME (naming_service,"NameService");

TAO_NS_DECLARE_NAME (application_command,"Application");
TAO_NS_DECLARE_NAME (application_command_factory,"Application_Command_Factory");

TAO_NS_DECLARE_NAME (event_channel_command,"EventChannel")
TAO_NS_DECLARE_NAME (event_channel_command_factory,"EventChannel_Command_Factory")

TAO_NS_DECLARE_NAME (consumer_admin_command,"ConsumerAdmin")
TAO_NS_DECLARE_NAME (consumer_admin_command_factory,"ConsumerAdmin_Command_Factory")

TAO_NS_DECLARE_NAME (supplier_admin_command,"SupplierAdmin")
TAO_NS_DECLARE_NAME (supplier_admin_command_factory,"SupplierAdmin_Command_Factory")

TAO_NS_DECLARE_NAME (periodic_supplier_command, "PeriodicSupplier");
TAO_NS_DECLARE_NAME (periodic_supplier_command_factory, "PeriodicSupplier_Command_Factory");

TAO_NS_DECLARE_NAME (periodic_consumer_command, "PeriodicConsumer");
TAO_NS_DECLARE_NAME (periodic_consumer_command_factory, "PeriodicConsumer_Command_Factory");

TAO_NS_DECLARE_NAME (poa_command, "POA");
TAO_NS_DECLARE_NAME (poa_command_factory, "POA_Command_Factory");

TAO_NS_DECLARE_NAME (filter_command, "Filter");
TAO_NS_DECLARE_NAME (filter_command_factory, "Filter_Command_Factory");
