// $Id$

#include "Name.h"



#define TAO_Notify_Tests_DECLARE_NAME(target, name) \
const char* const TAO_Notify_Tests_Name::target = name;

#define TAO_Notify_Tests_DECLARE_TCHAR_NAME(target, name) \
const ACE_TCHAR* const TAO_Notify_Tests_Name::target = ACE_TEXT (name);

TAO_Notify_Tests_DECLARE_TCHAR_NAME (command_builder,"Command_Builder")
TAO_Notify_Tests_DECLARE_NAME (root_poa,"RootPOA")
TAO_Notify_Tests_DECLARE_NAME (event_channel_factory,"NotifyEventChannelFactory")
TAO_Notify_Tests_DECLARE_NAME (naming_service,"NameService")

TAO_Notify_Tests_DECLARE_NAME (application_command,"Application")
TAO_Notify_Tests_DECLARE_TCHAR_NAME (application_command_factory,"Application_Command_Factory")

TAO_Notify_Tests_DECLARE_NAME (event_channel_command,"EventChannel")
TAO_Notify_Tests_DECLARE_TCHAR_NAME (event_channel_command_factory,"EventChannel_Command_Factory")

TAO_Notify_Tests_DECLARE_NAME (consumer_admin_command,"ConsumerAdmin")
TAO_Notify_Tests_DECLARE_TCHAR_NAME (consumer_admin_command_factory,"ConsumerAdmin_Command_Factory")

TAO_Notify_Tests_DECLARE_NAME (supplier_admin_command,"SupplierAdmin")
TAO_Notify_Tests_DECLARE_TCHAR_NAME (supplier_admin_command_factory,"SupplierAdmin_Command_Factory")

TAO_Notify_Tests_DECLARE_NAME (periodic_supplier_command, "PeriodicSupplier")
TAO_Notify_Tests_DECLARE_TCHAR_NAME (periodic_supplier_command_factory, "PeriodicSupplier_Command_Factory")

TAO_Notify_Tests_DECLARE_NAME (periodic_consumer_command, "PeriodicConsumer")
TAO_Notify_Tests_DECLARE_TCHAR_NAME (periodic_consumer_command_factory, "PeriodicConsumer_Command_Factory")

TAO_Notify_Tests_DECLARE_NAME (poa_command, "POA")
TAO_Notify_Tests_DECLARE_TCHAR_NAME (poa_command_factory, "POA_Command_Factory")

TAO_Notify_Tests_DECLARE_NAME (filter_command, "Filter")
TAO_Notify_Tests_DECLARE_TCHAR_NAME (filter_command_factory, "Filter_Command_Factory")
