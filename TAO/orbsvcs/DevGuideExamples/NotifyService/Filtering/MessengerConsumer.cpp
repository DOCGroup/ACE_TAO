#include "orbsvcs/CosNotifyChannelAdminC.h"
#include "orbsvcs/CosNotifyCommC.h"
#include "orbsvcs/CosNamingC.h"
#include "StructuredEventConsumer_i.h"

#include <iostream>

constexpr char ca_filter_expression[] =
  "($.From == 'sysadmin@company.com') and ($.Subject == 'urgent') ";
constexpr char tcl_grammar[] = "EXTENDED_TCL";

int
ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
  try
    {
      CORBA::ORB_var const orb = CORBA::ORB_init (argc, argv);
      CORBA::Object_var const naming_obj = orb->resolve_initial_references ("NameService");
      if (CORBA::is_nil (naming_obj.in ()))
        {
          std::cerr << "Unable to find naming service" << std::endl;
          return 1;
        }
      CosNaming::NamingContext_var const naming_context =
        CosNaming::NamingContext::_narrow (naming_obj.in ());

      CosNaming::Name name (1);
      name.length (1);
      name[0].id = CORBA::string_dup ("NotifyEventChannelFactory");
      CORBA::Object_var const factory_obj = naming_context->resolve (name);
      CosNotifyChannelAdmin::EventChannelFactory_var const notify_factory =
        CosNotifyChannelAdmin::EventChannelFactory::_narrow (factory_obj.in ());
      if (CORBA::is_nil (notify_factory.in ()))
        {
          std::cerr << "Unable to find notify factory" << std::endl;
          return 1;
        }

      name[0].id = CORBA::string_dup ("MyEventChannel");
      CORBA::Object_var const event_channel_obj = naming_context->resolve (name);
      CosNotifyChannelAdmin::EventChannel_var const event_channel =
        CosNotifyChannelAdmin::EventChannel::_narrow (event_channel_obj.in ());
      if (CORBA::is_nil (event_channel.in ()))
        {
          std::cerr << "Unable to find event channel" << std::endl;
          return 1;
        }

      CosNotifyChannelAdmin::AdminID admin_id {};
      CosNotifyChannelAdmin::InterFilterGroupOperator const ifgop = CosNotifyChannelAdmin::AND_OP;
      CosNotifyChannelAdmin::ConsumerAdmin_var const consumer_admin =
        event_channel->new_for_consumers (ifgop, admin_id);
      if (CORBA::is_nil (consumer_admin.in ()))
        {
          std::cerr << "Unable to find consumer admin" << std::endl;
          return 1;
        }
      CosNotifyFilter::FilterFactory_var const filter_factory =
        event_channel->default_filter_factory ();
      CosNotifyFilter::Filter_var const consumer_filter =
        filter_factory->create_filter (tcl_grammar);
      if (CORBA::is_nil (consumer_filter.in ()))
        {
          std::cerr << "Unable to create filter" << std::endl;
          return 1;
        }

      CosNotifyFilter::ConstraintExpSeq constraint_list (1);
      constraint_list.length (1);
      constraint_list[0].event_types.length (0);
      constraint_list[0].constraint_expr = CORBA::string_dup (ca_filter_expression);
      consumer_filter->add_constraints (constraint_list);
      consumer_admin->add_filter (consumer_filter.in ());

      CosNotification::EventTypeSeq added (1);
      CosNotification::EventTypeSeq removed (0);
      added.length (1);
      removed.length (0);
      added[0].domain_name = CORBA::string_dup ("*");
      added[0].type_name = CORBA::string_dup ("*");
      consumer_admin->subscription_change (added, removed);

      CORBA::Object_var const poa_obj = orb->resolve_initial_references ("RootPOA");
      PortableServer::POA_var const poa = PortableServer::POA::_narrow (poa_obj.in ());
      if (CORBA::is_nil (poa.in ()))
        {
          std::cerr << "Unable to initialize the POA" << std::endl;
          return 1;
        }
      PortableServer::Servant_var<StructuredEventConsumer_i> const servant =
        new StructuredEventConsumer_i (orb.in ());
      PortableServer::ObjectId_var const oid = poa->activate_object (servant.in ());
      CORBA::Object_var const consumer_obj = poa->id_to_reference (oid.in ());
      CosNotifyComm::StructuredPushConsumer_var const consumer =
        CosNotifyComm::StructuredPushConsumer::_narrow (consumer_obj.in ());

      CosNotifyChannelAdmin::ProxyID consumer_admin_proxy_id {};
      CosNotifyChannelAdmin::ProxySupplier_var const proxy_supplier =
        consumer_admin->obtain_notification_push_supplier (
          CosNotifyChannelAdmin::STRUCTURED_EVENT,
          consumer_admin_proxy_id);
      CosNotifyChannelAdmin::StructuredProxyPushSupplier_var const supplier_proxy =
        CosNotifyChannelAdmin::StructuredProxyPushSupplier::_narrow (proxy_supplier.in ());
      if (CORBA::is_nil (supplier_proxy.in ()))
        {
          std::cerr << "Unable to create structured push supplier proxy" << std::endl;
          return 1;
        }
      supplier_proxy->connect_structured_push_consumer (consumer.in ());

      PortableServer::POAManager_var const poa_manager = poa->the_POAManager ();
      poa_manager->activate ();
      orb->run ();
      orb->destroy ();
    }
  catch (CORBA::Exception const &ex)
    {
      std::cerr << ex << std::endl;
      return 1;
    }

  return 0;
}
