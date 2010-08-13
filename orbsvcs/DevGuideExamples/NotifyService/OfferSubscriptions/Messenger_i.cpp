// $Id$

#include "orbsvcs/CosNotifyChannelAdminC.h"
#include "orbsvcs/CosNotifyCommC.h"
#include "orbsvcs/CosNamingC.h"

#include "Messenger_i.h"
#include "StructuredEventSupplier_i.h"
#include <iostream>

Messenger_i::Messenger_i (CORBA::ORB_ptr orb)
              : orb_(CORBA::ORB::_duplicate(orb))
{
  CORBA::Object_var poa_object =
    orb_->resolve_initial_references("RootPOA");

  if (CORBA::is_nil (poa_object.in())) {
    std::cerr << "Unable to initialize the POA." << std::endl;
  }

  CORBA::Object_var naming_obj =
    orb_->resolve_initial_references ("NameService");

  if (CORBA::is_nil(naming_obj.in())) {
  std::cerr << "Unable to find naming service" << std::endl;
  }

  CosNaming::NamingContext_var naming_context =
    CosNaming::NamingContext::_narrow(naming_obj.in());

  CosNaming::Name name;
  name.length (1);
  name[0].id = CORBA::string_dup("NotifyEventChannelFactory");

  CORBA::Object_var obj = naming_context->resolve(name);

  CosNotifyChannelAdmin::EventChannelFactory_var notify_factory =
    CosNotifyChannelAdmin::EventChannelFactory::_narrow(obj.in());

  if (CORBA::is_nil(notify_factory.in())) {
    std::cerr << "Unable to find notify factory" << std::endl;
  }

  CosNotifyChannelAdmin::ChannelID id;
  CosNotification::QoSProperties initial_qos;
  CosNotification::AdminProperties initial_admin;

  CosNotifyChannelAdmin::EventChannel_var ec =
    notify_factory->create_channel (initial_qos,
            initial_admin,
            id);

  if (CORBA::is_nil (ec.in())) {
    std::cerr << "Unable to crete event channel" << std::endl;
  }

  // name.length(1);
  name[0].id = CORBA::string_dup("MyEventChannel");

  naming_context->rebind(name, ec.in());

  CosNotifyChannelAdmin::AdminID adminid;
  CosNotifyChannelAdmin::InterFilterGroupOperator ifgop =
    CosNotifyChannelAdmin::AND_OP;

  CosNotifyChannelAdmin::SupplierAdmin_var supplier_admin =
    ec->new_for_suppliers (ifgop, adminid);

  if (CORBA::is_nil (supplier_admin.in())) {
    std::cerr << "Unable to find supplier admin" << std::endl;
  }

  CosNotifyChannelAdmin::ProxyID supplieradmin_proxy_id;

  CosNotifyChannelAdmin::ProxyConsumer_var proxy_consumer =
    supplier_admin->obtain_notification_push_consumer(
         CosNotifyChannelAdmin::STRUCTURED_EVENT,
         supplieradmin_proxy_id);

  StructuredEventSupplier_i *servant =
    new StructuredEventSupplier_i(orb_.in());

  CosNotifyComm::StructuredPushSupplier_var supplier =
    servant->_this();

  s_proxy_consumer_ =
  CosNotifyChannelAdmin::StructuredProxyPushConsumer::
    _narrow(proxy_consumer.in());

  if (CORBA::is_nil (s_proxy_consumer_.in())) {
    std::cerr << "Unable to find structured proxy push consumer" << std::endl;
  }

  s_proxy_consumer_->
    connect_structured_push_supplier(supplier.in());


  CosNotification::EventTypeSeq added (1);
  CosNotification::EventTypeSeq removed (1);
  added.length (1);
  removed.length (1);
  added[0].domain_name = CORBA::string_dup ("OCI_TAO");
  added[0].type_name = CORBA::string_dup ("examples");

  removed[0].domain_name = CORBA::string_dup ("*");
  removed[0].type_name = CORBA::string_dup ("*");

  s_proxy_consumer_->offer_change(added, removed);
}

Messenger_i::~Messenger_i (void)
{
}

CORBA::Boolean Messenger_i::send_message (
    const char * user_name,
    const char * subject,
    char *& message)
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

  s_proxy_consumer_->push_structured_event(event);

  return 1;
}
