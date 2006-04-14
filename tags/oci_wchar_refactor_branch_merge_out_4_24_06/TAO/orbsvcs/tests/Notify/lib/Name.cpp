// $Id$

#include "Name.h"

ACE_RCSID(lib, TAO_Name, "$Id$")

#define TAO_Notify_Tests_DECLARE_NAME(target, name) \
const ACE_TCHAR* const TAO_Notify_Tests_Name::target = name;

#define TAO_Notify_Tests_DECLARE_NAME_A(target, name) \
const char* const TAO_Notify_Tests_Name::target = name;

TAO_Notify_Tests_DECLARE_NAME (command_builder, ACE_TEXT("Command_Builder"))
TAO_Notify_Tests_DECLARE_NAME_A (root_poa, "RootPOA")
TAO_Notify_Tests_DECLARE_NAME_A (event_channel_factory, "NotifyEventChannelFactory")
TAO_Notify_Tests_DECLARE_NAME_A (naming_service, "NameService")

TAO_Notify_Tests_DECLARE_NAME (application_command, ACE_TEXT("Application"))
TAO_Notify_Tests_DECLARE_NAME (application_command_factory, ACE_TEXT("Application_Command_Factory"))

TAO_Notify_Tests_DECLARE_NAME (event_channel_command, ACE_TEXT("EventChannel"))
TAO_Notify_Tests_DECLARE_NAME (event_channel_command_factory, ACE_TEXT("EventChannel_Command_Factory"))

TAO_Notify_Tests_DECLARE_NAME (consumer_admin_command, ACE_TEXT("ConsumerAdmin"))
TAO_Notify_Tests_DECLARE_NAME (consumer_admin_command_factory, ACE_TEXT("ConsumerAdmin_Command_Factory"))

TAO_Notify_Tests_DECLARE_NAME (supplier_admin_command, ACE_TEXT("SupplierAdmin"))
TAO_Notify_Tests_DECLARE_NAME (supplier_admin_command_factory, ACE_TEXT("SupplierAdmin_Command_Factory"))

TAO_Notify_Tests_DECLARE_NAME (periodic_supplier_command, ACE_TEXT("PeriodicSupplier"))
TAO_Notify_Tests_DECLARE_NAME (periodic_supplier_command_factory, ACE_TEXT("PeriodicSupplier_Command_Factory"))

TAO_Notify_Tests_DECLARE_NAME (periodic_consumer_command, ACE_TEXT("PeriodicConsumer"))
TAO_Notify_Tests_DECLARE_NAME (periodic_consumer_command_factory, ACE_TEXT("PeriodicConsumer_Command_Factory"))

TAO_Notify_Tests_DECLARE_NAME (poa_command, ACE_TEXT("POA"))
TAO_Notify_Tests_DECLARE_NAME (poa_command_factory, ACE_TEXT("POA_Command_Factory"))

TAO_Notify_Tests_DECLARE_NAME (filter_command, ACE_TEXT("Filter"))
TAO_Notify_Tests_DECLARE_NAME (filter_command_factory, ACE_TEXT("Filter_Command_Factory"))
