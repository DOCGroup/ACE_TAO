#include "orbsvcs/CosNotifyChannelAdminC.h"
#include "orbsvcs/CosNotifyCommC.h"
#include "orbsvcs/CosNamingC.h"
#include "StructuredEventConsumer_i.h"

#include <iostream>

int
ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
  try
    {
      CORBA::ORB_var const orb = CORBA::ORB_init (argc, argv);
      CORBA::Object_var const naming_obj = orb->resolve_initial_references ("NameService");
      CosNaming::NamingContext_var const naming_context =
        CosNaming::NamingContext::_narrow (naming_obj.in ());

      CosNaming::Name name (1);
      name.length (1);
      name[0].id = CORBA::string_dup ("MyEventChannel");
      CORBA::Object_var const event_channel_obj = naming_context->resolve (name);
      CosNotifyChannelAdmin::EventChannel_var const event_channel =
        CosNotifyChannelAdmin::EventChannel::_narrow (event_channel_obj.in ());

      CosNotifyChannelAdmin::AdminID admin_id {};
      CosNotifyChannelAdmin::InterFilterGroupOperator const ifgop = CosNotifyChannelAdmin::AND_OP;
      CosNotifyChannelAdmin::ConsumerAdmin_var const consumer_admin =
        event_channel->new_for_consumers (ifgop, admin_id);

      CORBA::Object_var const poa_obj = orb->resolve_initial_references ("RootPOA");
      PortableServer::POA_var const poa = PortableServer::POA::_narrow (poa_obj.in ());
      PortableServer::Servant_var<StructuredEventConsumer_i> const servant =
        new StructuredEventConsumer_i (orb.in ());
      PortableServer::ObjectId_var const object_id = poa->activate_object (servant.in ());
      CORBA::Object_var const consumer_obj = poa->id_to_reference (object_id.in ());
      CosNotifyComm::StructuredPushConsumer_var const consumer =
        CosNotifyComm::StructuredPushConsumer::_narrow (consumer_obj.in ());

      CosNotifyChannelAdmin::ProxyID consumer_admin_proxy_id {};
      CosNotifyChannelAdmin::ProxySupplier_var const proxy_supplier =
        consumer_admin->obtain_notification_push_supplier (
          CosNotifyChannelAdmin::STRUCTURED_EVENT,
          consumer_admin_proxy_id);
      CosNotifyChannelAdmin::StructuredProxyPushSupplier_var const supplier_proxy =
        CosNotifyChannelAdmin::StructuredProxyPushSupplier::_narrow (proxy_supplier.in ());
      supplier_proxy->connect_structured_push_consumer (consumer.in ());

      CosNotification::EventTypeSeq added (1);
      CosNotification::EventTypeSeq removed (1);
      added.length (1);
      removed.length (1);
      added[0].domain_name = CORBA::string_dup ("OCI_TAO");
      added[0].type_name = CORBA::string_dup ("examples");
      removed[0].domain_name = CORBA::string_dup ("*");
      removed[0].type_name = CORBA::string_dup ("*");
      supplier_proxy->subscription_change (added, removed);

      PortableServer::POAManager_var const manager = poa->the_POAManager ();
      manager->activate ();
      orb->run ();
      orb->destroy ();
    }
  catch (CORBA::Exception const &ex)
    {
      std::cerr << "Caught exception: " << ex << std::endl;
      return 1;
    }

  return 0;
}
