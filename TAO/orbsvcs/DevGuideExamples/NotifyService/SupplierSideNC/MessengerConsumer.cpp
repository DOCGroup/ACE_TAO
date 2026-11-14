#include "orbsvcs/CosNotifyChannelAdminC.h"
#include "orbsvcs/CosNotifyCommC.h"
#include "orbsvcs/CosNamingC.h"
#include "StructuredEventConsumer_i.h"
#include "ace/Get_Opt.h"

#include <cstdio>
#include <iostream>
#include <memory>

constexpr char CA_FILTER[] = "From == 'person@company.com'";
constexpr char TCL_GRAMMAR[] = "TCL";

int ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
  try
    {
      CORBA::ORB_var const orb = CORBA::ORB_init (argc, argv);
      ACE_TCHAR const *readiness_file { ACE_TEXT ("MessengerConsumer.ready") };
      ACE_Get_Opt get_opts (argc, argv, ACE_TEXT ("o:"));
      int c {};
      while ((c = get_opts ()) != -1)
        if (c == 'o')
          readiness_file = get_opts.opt_arg ();

      CORBA::Object_var const naming_obj = orb->resolve_initial_references ("NameService");
      if (CORBA::is_nil (naming_obj.in ()))
        {
          std::cerr << "Unable to find naming service" << std::endl;
          return 1;
        }

      CosNaming::NamingContext_var const naming_context =
        CosNaming::NamingContext::_narrow (naming_obj.in ());
      CosNaming::Name name {};
      name.length (1);
      name[0].id = CORBA::string_dup ("MyEventChannel");
      CORBA::Object_var const event_channel_object = naming_context->resolve (name);
      CosNotifyChannelAdmin::EventChannel_var const event_channel =
        CosNotifyChannelAdmin::EventChannel::_narrow (event_channel_object.in ());
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
      CosNotifyFilter::Filter_var const consumer_filter = filter_factory->create_filter (TCL_GRAMMAR);
      if (CORBA::is_nil (consumer_filter.in ()))
        {
          std::cerr << "Unable to create filter object" << std::endl;
          return 1;
        }

      CosNotifyFilter::ConstraintExpSeq constraints {};
      constraints.length (1);
      constraints[0].event_types.length (0);
      constraints[0].constraint_expr = CORBA::string_dup (CA_FILTER);
      consumer_filter->add_constraints (constraints);
      consumer_admin->add_filter (consumer_filter.in ());

      CosNotification::EventTypeSeq added {};
      added.length (1);
      added[0].domain_name = CORBA::string_dup ("*");
      added[0].type_name = CORBA::string_dup ("*");
      CosNotification::EventTypeSeq removed {};
      consumer_admin->subscription_change (added, removed);

      CORBA::Object_var const poa_object = orb->resolve_initial_references ("RootPOA");
      if (CORBA::is_nil (poa_object.in ()))
        {
          std::cerr << "Unable to initialize the POA." << std::endl;
          return 1;
        }

      PortableServer::POA_var const poa = PortableServer::POA::_narrow (poa_object.in ());
      PortableServer::Servant_var<StructuredEventConsumer_i> const servant =
        new StructuredEventConsumer_i (orb.in ());
      PortableServer::ObjectId_var const object_id = poa->activate_object (servant.in ());
      CORBA::Object_var const consumer_object = poa->id_to_reference (object_id.in ());
      CosNotifyComm::StructuredPushConsumer_var const consumer =
        CosNotifyComm::StructuredPushConsumer::_narrow (consumer_object.in ());

      CosNotifyChannelAdmin::ProxyID proxy_id {};
      CosNotifyChannelAdmin::ProxySupplier_var const proxy_supplier =
        consumer_admin->obtain_notification_push_supplier (
          CosNotifyChannelAdmin::STRUCTURED_EVENT,
          proxy_id);
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
      {
        std::unique_ptr<FILE, int (*) (FILE *)> ready (
          ACE_OS::fopen (readiness_file, ACE_TEXT ("w")),
          ACE_OS::fclose);
        if (!ready)
          {
            std::cerr << "Could not create readiness file" << std::endl;
            return 1;
          }
        ACE_OS::fprintf (ready.get (), "Ready\n");
      }
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
