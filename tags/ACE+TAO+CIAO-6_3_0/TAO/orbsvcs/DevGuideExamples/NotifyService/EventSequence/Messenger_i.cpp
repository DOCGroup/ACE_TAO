// $Id$

#include "orbsvcs/CosNotifyChannelAdminC.h"
#include "orbsvcs/CosNotifyCommC.h"
#include "orbsvcs/CosNamingC.h"

#include "Messenger_i.h"
#include "EventSequenceSupplier_i.h"

#include <iostream>

Messenger_i::Messenger_i (CORBA::ORB_ptr orb)
: orb_ (CORBA::ORB::_duplicate (orb))
, supplier_(new EventSequenceSupplier_i(orb))
{

  CORBA::Object_var naming_obj =
    orb_->resolve_initial_references ("NameService");

  CosNaming::NamingContextExt_var naming_context =
    CosNaming::NamingContextExt::_narrow (naming_obj.in());

  CORBA::Object_var obj = naming_context->resolve_str("NotifyEventChannelFactory");

  CosNotifyChannelAdmin::EventChannelFactory_var notify_factory =
    CosNotifyChannelAdmin::EventChannelFactory::_narrow (obj.in ());

  CosNotifyChannelAdmin::ChannelID id;
  CosNotification::QoSProperties initial_qos;
  CosNotification::AdminProperties initial_admin;

  CosNotifyChannelAdmin::EventChannel_var ec =
    notify_factory->create_channel (initial_qos,
    initial_admin,
    id);

  CosNaming::Name_var name = naming_context->to_name("MyEventChannel");
  naming_context->rebind(name.in(), ec.in());

  CosNotifyChannelAdmin::AdminID adminid;
  CosNotifyChannelAdmin::InterFilterGroupOperator ifgop =
    CosNotifyChannelAdmin::AND_OP;

  CosNotifyChannelAdmin::SupplierAdmin_var supplier_admin =
    ec->new_for_suppliers (ifgop, adminid);

  CosNotifyChannelAdmin::ProxyID supplieradmin_proxy_id;

  CosNotifyChannelAdmin::ProxyConsumer_var proxy_consumer =
    supplier_admin->obtain_notification_push_consumer(
    CosNotifyChannelAdmin::SEQUENCE_EVENT,
    supplieradmin_proxy_id);

  CORBA::Object_var poa_obj = orb_->resolve_initial_references ("RootPOA");
  PortableServer::POA_var poa = PortableServer::POA::_narrow (poa_obj.in());
  PortableServer::POAManager_var mgr = poa->the_POAManager();
  mgr->activate ();

  PortableServer::ObjectId_var objectId = poa->activate_object(supplier_.get());
  CORBA::Object_var supplier_obj = poa->id_to_reference(objectId.in());

  CosNotifyComm::SequencePushSupplier_var supplier =
    CosNotifyComm::SequencePushSupplier::_narrow(supplier_obj.in());

  consumer_proxy_ =
    CosNotifyChannelAdmin::SequenceProxyPushConsumer::_narrow(proxy_consumer.in());

  consumer_proxy_->connect_sequence_push_supplier(supplier.in());
}


Messenger_i::~Messenger_i (void)
{
}

CORBA::Boolean Messenger_i::send_message (
                                          const char * user_name,
                                          const char * subject,
                                          char *& message
                                          )
{

  std::cout << "Message from: " << user_name << std::endl;
  std::cout << "Subject:      " << subject << std::endl;
  std::cout << "Message:      " << message << std::endl;

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
  event.filterable_data.length (1);
  event.filterable_data[0].name = CORBA::string_dup("Message from:");
  event.filterable_data[0].value <<= (const char *)user_name;
  event.filterable_data.length (2);
  event.filterable_data[1].name = CORBA::string_dup("Subject:");
  event.filterable_data[1].value <<= (const char *)subject;
  event.filterable_data.length (3);
  event.filterable_data[2].name = CORBA::string_dup("Message:");
  event.filterable_data[2].value <<= (const char *)message;

  std::cout << "pushing " << std::endl;
  CosNotification::EventBatch events;
  events.length(4);
  events[0] = event;
  events[1] = event;
  events[2] = event;
  events[3] = event;

  consumer_proxy_->push_structured_events(events);

  return 1;

}

