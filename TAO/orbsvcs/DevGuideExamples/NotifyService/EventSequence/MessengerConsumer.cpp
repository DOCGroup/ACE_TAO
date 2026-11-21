#include "orbsvcs/CosNotifyChannelAdminC.h"
#include "orbsvcs/CosNotifyCommC.h"
#include "orbsvcs/CosNamingC.h"
#include "EventSequenceConsumer_i.h"

#include <iostream>

int
ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
  try
    {
      CORBA::ORB_var const orb = CORBA::ORB_init (argc, argv);
      CORBA::Object_var obj = orb->resolve_initial_references ("NameService");
      CosNaming::NamingContextExt_var const naming_context =
        CosNaming::NamingContextExt::_narrow (obj.in ());

      obj = naming_context->resolve_str ("MyEventChannel");
      CosNotifyChannelAdmin::EventChannel_var const event_channel =
        CosNotifyChannelAdmin::EventChannel::_narrow (obj.in ());

      CosNotifyChannelAdmin::AdminID admin_id {};
      CosNotifyChannelAdmin::InterFilterGroupOperator const ifgop = CosNotifyChannelAdmin::AND_OP;
      CosNotifyChannelAdmin::ConsumerAdmin_var const consumer_admin =
        event_channel->new_for_consumers (ifgop, admin_id);

      obj = orb->resolve_initial_references ("RootPOA");
      PortableServer::POA_var const poa = PortableServer::POA::_narrow (obj.in ());
      PortableServer::Servant_var<EventSequenceConsumer_i> const servant =
        new EventSequenceConsumer_i (orb.in ());
      PortableServer::ObjectId_var const object_id = poa->activate_object (servant.in ());
      obj = poa->id_to_reference (object_id.in ());
      CosNotifyComm::SequencePushConsumer_var const consumer =
        CosNotifyComm::SequencePushConsumer::_narrow (obj.in ());

      CosNotifyChannelAdmin::ProxyID consumer_admin_proxy_id {};
      CosNotifyChannelAdmin::ProxySupplier_var const proxy_supplier =
        consumer_admin->obtain_notification_push_supplier (
          CosNotifyChannelAdmin::SEQUENCE_EVENT,
          consumer_admin_proxy_id);
      CosNotifyChannelAdmin::SequenceProxyPushSupplier_var const supplier_proxy =
        CosNotifyChannelAdmin::SequenceProxyPushSupplier::_narrow (proxy_supplier.in ());

      CosNotification::EventTypeSeq added (1);
      CosNotification::EventTypeSeq removed (1);
      added.length (1);
      removed.length (1);
      added[0].domain_name = CORBA::string_dup ("OCI_TAO");
      added[0].type_name = CORBA::string_dup ("examples");
      removed[0].domain_name = CORBA::string_dup ("*");
      removed[0].type_name = CORBA::string_dup ("*");
      supplier_proxy->connect_sequence_push_consumer (consumer.in ());
      supplier_proxy->subscription_change (added, removed);

      PortableServer::POAManager_var const poa_manager = poa->the_POAManager ();
      poa_manager->activate ();
      orb->run ();
      orb->destroy ();

      return 0;
    }
  catch (CORBA::Exception const &ex)
    {
      std::cerr << "Caught exception: " << ex << std::endl;
    }

  return 1;
}
