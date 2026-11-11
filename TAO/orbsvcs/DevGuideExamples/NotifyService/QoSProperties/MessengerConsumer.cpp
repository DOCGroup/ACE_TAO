#include "orbsvcs/CosNotifyChannelAdminC.h"
#include "orbsvcs/CosNotifyCommC.h"
#include "orbsvcs/CosNamingC.h"
#include "StructuredEventConsumer_i.h"
#include <iostream>

int ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
  try
    {
      CORBA::ORB_var orb = CORBA::ORB_init (argc, argv);
      CORBA::Object_var naming_obj = orb->resolve_initial_references ("NameService");
      CosNaming::NamingContext_var naming_context =
        CosNaming::NamingContext::_narrow (naming_obj.in ());

      CosNaming::Name name;
      name.length (1);
      name[0].id = CORBA::string_dup ("MyEventChannel");
      CORBA::Object_var event_channel_object = naming_context->resolve (name);
      CosNotifyChannelAdmin::EventChannel_var event_channel =
        CosNotifyChannelAdmin::EventChannel::_narrow (event_channel_object.in ());

      CosNotifyChannelAdmin::AdminID admin_id {};
      CosNotifyChannelAdmin::InterFilterGroupOperator const ifgop = CosNotifyChannelAdmin::AND_OP;
      CosNotifyChannelAdmin::ConsumerAdmin_var consumer_admin =
        event_channel->new_for_consumers (ifgop, admin_id);

      CORBA::Object_var poa_object = orb->resolve_initial_references ("RootPOA");
      PortableServer::POA_var poa = PortableServer::POA::_narrow (poa_object.in ());
      PortableServer::Servant_var<StructuredEventConsumer_i> servant =
        new StructuredEventConsumer_i (orb.in ());
      PortableServer::ObjectId_var object_id = poa->activate_object (servant.in ());
      CORBA::Object_var consumer_object = poa->id_to_reference (object_id.in ());
      CosNotifyComm::StructuredPushConsumer_var consumer =
        CosNotifyComm::StructuredPushConsumer::_narrow (consumer_object.in ());

      CosNotifyChannelAdmin::ProxyID proxy_id {};
      CosNotifyChannelAdmin::ProxySupplier_var proxy_supplier =
        consumer_admin->obtain_notification_push_supplier (
          CosNotifyChannelAdmin::STRUCTURED_EVENT,
          proxy_id);
      CosNotifyChannelAdmin::StructuredProxyPushSupplier_var supplier_proxy =
        CosNotifyChannelAdmin::StructuredProxyPushSupplier::_narrow (proxy_supplier.in ());

      CosNotification::QoSProperties properties;
      properties.length (1);
      properties[0].name = CORBA::string_dup (CosNotification::OrderPolicy);
      properties[0].value <<= CosNotification::FifoOrder;
      supplier_proxy->set_qos (properties);
      supplier_proxy->connect_structured_push_consumer (consumer.in ());

      CosNotification::EventTypeSeq added;
      CosNotification::EventTypeSeq removed;
      added.length (1);
      removed.length (1);
      added[0].domain_name = CORBA::string_dup ("OCI_TAO");
      added[0].type_name = CORBA::string_dup ("examples");
      removed[0].domain_name = CORBA::string_dup ("*");
      removed[0].type_name = CORBA::string_dup ("*");
      supplier_proxy->subscription_change (added, removed);

      PortableServer::POAManager_var manager = poa->the_POAManager ();
      manager->activate ();
      orb->run ();
    }
  catch (CORBA::Exception const &ex)
    {
      std::cerr << "Caught exception: " << ex << std::endl;
      return 1;
    }

  return 0;
}
