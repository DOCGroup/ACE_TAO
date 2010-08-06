// $Id$

#include "orbsvcs/Notify/Notify_EventChannelFactory_i.h"
#include "orbsvcs/CosNamingC.h"
#include "ace/Profile_Timer.h"
#include "StructuredEventSupplier_i.h"
#include <iostream>

int ACE_TMAIN(int argc, ACE_TCHAR * argv[])
{
  try
  {
     CORBA::ORB_var orb = CORBA::ORB_init(argc, argv);
     CORBA::Object_var rootObj = orb->resolve_initial_references("NameService");
     CosNaming::NamingContext_var rootNC =
       CosNaming::NamingContext::_narrow(rootObj.in());

     // Get reference to Root POA.
     CORBA::Object_var obj = orb->resolve_initial_references("RootPOA");
     PortableServer::POA_var poa = PortableServer::POA::_narrow(obj.in());

     // Activate POA manager
     PortableServer::POAManager_var mgr = poa->the_POAManager();
     mgr->activate();

     // Create an Event Channel factory.
     CosNotifyChannelAdmin::EventChannelFactory_var notify_factory =
       TAO_Notify_EventChannelFactory_i::create(poa.in());
     ACE_ASSERT (!CORBA::is_nil (notify_factory.in ()));

     // Create an Event Channel.
     CosNotifyChannelAdmin::ChannelID id;
     CosNotification::QoSProperties initial_qos;
     CosNotification::AdminProperties initial_admin;
     CosNotifyChannelAdmin::EventChannel_var ec =
       notify_factory->create_channel (initial_qos, initial_admin, id);

     // Bind it in the Naming Service.
     CosNaming::Name name(1);
     name.length(1);
     name[0].id = CORBA::string_dup("MyEventChannel");
     rootNC->rebind(name, ec.in());

     // Become a structured push supplier.
     CosNotifyChannelAdmin::AdminID adminid;
     CosNotifyChannelAdmin::InterFilterGroupOperator ifgop =
       CosNotifyChannelAdmin::OR_OP;
     CosNotifyChannelAdmin::SupplierAdmin_var supplier_admin =
       ec->new_for_suppliers (ifgop, adminid);

     CosNotifyChannelAdmin::ProxyID supplieradmin_proxy_id;
     CosNotifyChannelAdmin::ProxyConsumer_var proxy_consumer =
     supplier_admin->obtain_notification_push_consumer(
       CosNotifyChannelAdmin::STRUCTURED_EVENT,
       supplieradmin_proxy_id);

     StructuredEventSupplier_i *servant =
       new StructuredEventSupplier_i(orb.in());
     CosNotifyComm::StructuredPushSupplier_var supplier = servant->_this();

     CosNotifyChannelAdmin::StructuredProxyPushConsumer_var consumer_proxy =
       CosNotifyChannelAdmin::StructuredProxyPushConsumer::_narrow(
         proxy_consumer.in());
     consumer_proxy->connect_structured_push_supplier(supplier.in());

     // Set up events to push.
     CosNotification::StructuredEvent event;
     event.header.fixed_header.event_type.domain_name =
     CORBA::string_dup("OCI_TAO");

    event.header.fixed_header.event_type.type_name =
      CORBA::string_dup("examples");
    event.header.fixed_header.event_name = CORBA::string_dup("myevent");
    event.filterable_data.length (1);
    event.filterable_data[0].name = CORBA::string_dup("Message from:");
    event.filterable_data[0].value <<= (const char *)user_name;
    event.filterable_data.length (2);
    event.filterable_data[1].name = CORBA::string_dup("Subject:");
    event.filterable_data[1].value <<= (const char *)subject;
    event.filterable_data.length (3);
    event.filterable_data[2].name = CORBA::string_dup("Message:");
    event.filterable_data[2].value <<= (const char *)message;

    // Push events.
    while (1) {
      std::cout << "pushing " << std::endl;
      consumer_proxy->push_structured_event (event);
      ACE_OS::sleep (1);
    }
  }
  catch(const CORBA::Exception& ex) {
    return 1;
  }

  return 0;
}

