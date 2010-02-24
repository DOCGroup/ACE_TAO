// $Id$

#include "ace/Get_Opt.h"

#include "orbsvcs/CosNotifyChannelAdminC.h"
#include "orbsvcs/CosNotifyCommC.h"
#include "orbsvcs/CosNamingC.h"

#include "StructuredEventConsumer_i.h"
#include <iostream>

#define CA_FILTER "Subject == 'OCI_TAO'"
#define TCL_GRAMMAR "TCL"

int ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
  try
  {
    CORBA::ORB_var orb = CORBA::ORB_init(argc, argv);

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
      CosNotifyChannelAdmin::AND_OP;

    CosNotifyChannelAdmin::ConsumerAdmin_var consumer_admin =
      ec->new_for_consumers(ifgop,
      adminid);

    if (CORBA::is_nil (consumer_admin.in())) {
      std::cerr << "Unable to find consumer admin" << std::endl;
      return 1;
    }

    CosNotifyFilter::FilterFactory_var ffact =
      ec->default_filter_factory ();

    // setup a filter at the consumer admin
    CosNotifyFilter::Filter_var ca_filter =
      ffact->create_filter (TCL_GRAMMAR);

    if (CORBA::is_nil (ca_filter.in())) {
      std::cerr << "Unable to create filetr object" << std::endl;
      return 1;
    }

    CosNotifyFilter::ConstraintExpSeq constraint_list (1);
    constraint_list.length (1);
    constraint_list[0].event_types.length (0);
    constraint_list[0].constraint_expr = CORBA::string_dup (CA_FILTER);

    ca_filter->add_constraints (constraint_list);

    consumer_admin ->add_filter (ca_filter.in());

    CosNotification::EventTypeSeq added(1);
    CosNotification::EventTypeSeq removed (0);
    added.length (1);
    removed.length (0);

    added[0].domain_name =  CORBA::string_dup ("*");
    added[0].type_name = CORBA::string_dup ("*");

    consumer_admin->subscription_change (added, removed);

    CORBA::Object_var poa_object =
      orb->resolve_initial_references("RootPOA");

    if (CORBA::is_nil (poa_object.in())) {
      std::cerr << "Unable to initialize the POA." << std::endl;
      return 1;
    }

    PortableServer::POA_var poa =
      PortableServer::POA::_narrow(poa_object.in());

    PortableServer::Servant_var<StructuredEventConsumer_i> servant =
      new StructuredEventConsumer_i(orb.in());

    PortableServer::ObjectId_var oid = poa->activate_object(servant.in());
    CORBA::Object_var consumer_obj = poa->id_to_reference(oid.in());
    CosNotifyComm::StructuredPushConsumer_var consumer =
      CosNotifyComm::StructuredPushConsumer::_narrow(consumer_obj.in());


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

    PortableServer::POAManager_var poa_manager = poa->the_POAManager();

    poa_manager->activate();

    orb->run();
    orb->destroy ();

   }
   catch(const CORBA::Exception& ex) {
     std::cerr << ex << std::endl;
     return 1;
   }
   return 0;
}
