// $Id$

#include "orbsvcs/CosNotifyChannelAdminC.h"
#include "orbsvcs/CosNotifyCommC.h"
#include "orbsvcs/CosNamingC.h"
#include "ace/OS_NS_stdio.h"

#include "Messenger_i.h"
#include "StructuredEventSupplier_i.h"
#include <iostream>

Messenger_i::Messenger_i (CORBA::ORB_ptr orb)
  : orb_ (CORBA::ORB::_duplicate (orb))
{

  CORBA::Object_var naming_obj =
    orb_->resolve_initial_references ("NameService");

  CosNaming::NamingContext_var naming_context =
    CosNaming::NamingContext::_narrow (naming_obj.in());

  CosNaming::Name name;
  name.length (1);
  name[0].id = CORBA::string_dup ("NotifyEventChannelFactory");

  CORBA::Object_var obj = naming_context->resolve (name);

  CosNotifyChannelAdmin::EventChannelFactory_var notify_factory =
    CosNotifyChannelAdmin::EventChannelFactory::_narrow (obj.in ());

  CosNotifyChannelAdmin::ChannelID id;
  CosNotification::QoSProperties initial_qos;
  CosNotification::AdminProperties initial_admin;

  CosNotifyChannelAdmin::EventChannel_var ec =
    notify_factory->create_channel (initial_qos,
                                    initial_admin,
                                    id);

  name[0].id = CORBA::string_dup ("MyEventChannel");

  naming_context->rebind (name, ec.in());

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
    new StructuredEventSupplier_i(orb_.in());

  CORBA::Object_var poa_obj = orb_->resolve_initial_references ("RootPOA");
  PortableServer::POA_var poa = PortableServer::POA::_narrow (poa_obj.in ());
  PortableServer::POAManager_var mgr = poa->the_POAManager ();

  mgr->activate ();
  PortableServer::ObjectId_var objectId = poa->activate_object (servant);

  CORBA::Object_var supplier_obj = poa->id_to_reference (objectId.in ());

  CosNotifyComm::StructuredPushSupplier_var supplier =
    CosNotifyComm::StructuredPushSupplier::_narrow (supplier_obj.in ());

  consumer_proxy_ =
    CosNotifyChannelAdmin::StructuredProxyPushConsumer::_narrow (proxy_consumer.in());

  consumer_proxy_->
    connect_structured_push_supplier (supplier.in());
}


Messenger_i::~Messenger_i (void)
{
}

CORBA::Boolean Messenger_i::send_message (
    const char * user_name,
    const char * subject,
    char *& message)
{

  ACE_OS::printf("Message from: %s\nSubject:      %s\nMessage:      %s\n",
                 user_name, subject, message);
  //cout << "Message from: " << user_name << endl;
  //cout << "Subject:      " << subject << endl;
  //cout << "Message:      " << message << endl;


  // Event Definition
  CosNotification::StructuredEvent event;

  event.header.fixed_header.event_type.domain_name =
    CORBA::string_dup("OCI_TAO");
  // string
  event.header.fixed_header.event_type.type_name =
    CORBA::string_dup("examples");
  // string
  event.header.fixed_header.event_name =
    CORBA::string_dup("myevent");

  // OptionalHeaderFields
  // PropertySeq

  // sequence<Property>: string name, any value
  event.filterable_data.length (3);
  event.filterable_data[0].name = CORBA::string_dup("Message from:");
  event.filterable_data[0].value <<= (const char *)user_name;
  event.filterable_data[1].name = CORBA::string_dup("Subject:");
  event.filterable_data[1].value <<= (const char *)subject;
  event.filterable_data[2].name = CORBA::string_dup("Message:");
  event.filterable_data[2].value <<= (const char *)message;

  std::cout << "pushing " << std::endl;
  consumer_proxy_->push_structured_event(event);

  return 1;

}

