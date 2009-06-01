// $Id$

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
    CORBA::ORB_var orb = CORBA::ORB_init(argc, argv);

    CORBA::Object_var poa_object =
      orb->resolve_initial_references("RootPOA");

    if (CORBA::is_nil (poa_object.in())) {
      std::cerr << "Unable to initialize the POA." << std::endl;
      return 1;
    }

    PortableServer::POA_var poa =
    PortableServer::POA::_narrow(poa_object.in());

    // Activate POA manager
    PortableServer::POAManager_var mgr = poa->the_POAManager();
    mgr->activate();

    CORBA::Object_var naming_obj =
      orb->resolve_initial_references ("NameService");

    if (CORBA::is_nil(naming_obj.in())) {
      std::cerr << "Unable to find naming service" << std::endl;
      return 1;
    }

    CosNaming::NamingContext_var naming_context =
      CosNaming::NamingContext::_narrow(naming_obj.in());

    CosNaming::Name name(1);

    name.length (1);
    name[0].id = CORBA::string_dup("MyEventChannel");
    CORBA::Object_var ecObj = naming_context->resolve(name);

    CosNotifyChannelAdmin::EventChannel_var ec =
      CosNotifyChannelAdmin::EventChannel::_narrow(ecObj.in());

    if (CORBA::is_nil (ec.in())) {
      std::cerr << "Unable to find event channel" << std::endl;
      return 1;
    }

    CosNotifyChannelAdmin::AdminID adminid;
    CosNotifyChannelAdmin::InterFilterGroupOperator ifgop =
      CosNotifyChannelAdmin::OR_OP;

    CosNotifyChannelAdmin::ConsumerAdmin_var consumer_admin =
      ec->new_for_consumers(ifgop,
      adminid);

    if (CORBA::is_nil (consumer_admin.in())) {
      std::cerr << "Unable to find consumer admin" << std::endl;
      return 1;
    }

    StructuredEventConsumer_i  servant(orb.in());

    CosNotifyComm::StructuredPushConsumer_var consumer =
      servant._this();

    CosNotifyChannelAdmin::ProxyID consumeradmin_proxy_id;

    CosNotifyChannelAdmin::ProxySupplier_var proxy_supplier =
      consumer_admin->obtain_notification_push_supplier(
          CosNotifyChannelAdmin::STRUCTURED_EVENT,
          consumeradmin_proxy_id);

    // The proxy that we are connected to.
    CosNotifyChannelAdmin::StructuredProxyPushSupplier_var supplier_proxy;
    supplier_proxy = CosNotifyChannelAdmin::StructuredProxyPushSupplier::
      _narrow(proxy_supplier.in());

    if (CORBA::is_nil (supplier_proxy.in())) {
      std::cerr << "Unable to create structured push supplier proxy" << std::endl;
      return 1;
    }

    supplier_proxy->connect_structured_push_consumer(consumer.in());
    CosNotification::EventTypeSeq added (1);
    CosNotification::EventTypeSeq removed (1);
    added.length (1);
    removed.length (1);

    added[0].domain_name = CORBA::string_dup ("OCI_TAO");
    added[0].type_name = CORBA::string_dup ("examples");

    removed[0].domain_name = CORBA::string_dup ("*");
    removed[0].type_name = CORBA::string_dup ("*");

    supplier_proxy->subscription_change(added, removed);

    orb->run();
   }
   catch(const CORBA::Exception& ex)
    {
      std::cerr << "MessengerConsumer:: Caught exception: " << ex << std::endl;
      return 1;
    }
   std::cerr << "MessengerConsumer: success" << std::endl;
   return 0;
}
