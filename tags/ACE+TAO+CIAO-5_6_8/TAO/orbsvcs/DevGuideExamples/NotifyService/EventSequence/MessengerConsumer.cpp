// $Id$

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
    CORBA::ORB_var orb = CORBA::ORB_init(argc, argv);

    CORBA::Object_var obj = orb->resolve_initial_references("NameService");

    CosNaming::NamingContextExt_var naming_context =
      CosNaming::NamingContextExt::_narrow(obj.in());

    obj = naming_context->resolve_str("MyEventChannel");
    CosNotifyChannelAdmin::EventChannel_var ec =
      CosNotifyChannelAdmin::EventChannel::_narrow(obj.in());

    CosNotifyChannelAdmin::AdminID adminid;
    CosNotifyChannelAdmin::InterFilterGroupOperator ifgop =
      CosNotifyChannelAdmin::OR_OP;

    CosNotifyChannelAdmin::ConsumerAdmin_var consumer_admin =
      ec->new_for_consumers(ifgop,
      adminid);

    obj = orb->resolve_initial_references("RootPOA");

    PortableServer::POA_var poa = PortableServer::POA::_narrow (obj.in());

    EventSequenceConsumer_i servant(orb.in());

    PortableServer::ObjectId_var objectId = poa->activate_object(&servant);

    obj = poa->id_to_reference (objectId.in());
    CosNotifyComm::SequencePushConsumer_var consumer =
      CosNotifyComm::SequencePushConsumer::_narrow(obj.in());

    CosNotifyChannelAdmin::ProxyID consumeradmin_proxy_id;

    CosNotifyChannelAdmin::ProxySupplier_var proxy_supplier =
      consumer_admin->obtain_notification_push_supplier(
      CosNotifyChannelAdmin::SEQUENCE_EVENT,
          consumeradmin_proxy_id);

    CosNotifyChannelAdmin::SequenceProxyPushSupplier_var supplier_proxy =
      CosNotifyChannelAdmin::SequenceProxyPushSupplier::_narrow(proxy_supplier.in());

    supplier_proxy->connect_sequence_push_consumer(consumer.in());

    CosNotification::EventTypeSeq added (1);
    CosNotification::EventTypeSeq removed (1);
    added.length (1);
    removed.length (1);

    added[0].domain_name = CORBA::string_dup ("OCI_TAO");
    added[0].type_name = CORBA::string_dup ("examples");

    removed[0].domain_name = CORBA::string_dup ("*");
    removed[0].type_name = CORBA::string_dup ("*");

    supplier_proxy->subscription_change(added, removed);

    PortableServer::POAManager_var poa_manager = poa->the_POAManager();

    poa_manager->activate();

    orb->run();

    orb->destroy();

    return 0;
  }
  catch(const CORBA::Exception& ex)
  {
    std::cerr << "Caught exception: " << ex << std::endl;
  }
  return 1;
}
