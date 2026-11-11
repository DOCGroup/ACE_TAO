#include "orbsvcs/CosNotifyChannelAdminC.h"
#include "orbsvcs/CosNotifyCommC.h"
#include "orbsvcs/CosNamingC.h"

#include "Messenger_i.h"
#include "StructuredEventSupplier_i.h"

#include <iostream>

Messenger_i::Messenger_i (CORBA::ORB_ptr orb)
  : orb_ (CORBA::ORB::_duplicate (orb))
{
  CORBA::Object_var const poa_obj = orb_->resolve_initial_references ("RootPOA");
  PortableServer::POA_var const poa = PortableServer::POA::_narrow (poa_obj.in ());
  CORBA::Object_var const naming_obj = orb_->resolve_initial_references ("NameService");
  CosNaming::NamingContext_var const naming_context =
    CosNaming::NamingContext::_narrow (naming_obj.in ());

  CosNaming::Name name (1);
  name.length (1);
  name[0].id = CORBA::string_dup ("NotifyEventChannelFactory");
  CORBA::Object_var const obj = naming_context->resolve (name);
  CosNotifyChannelAdmin::EventChannelFactory_var const notify_factory =
    CosNotifyChannelAdmin::EventChannelFactory::_narrow (obj.in ());

  CosNotifyChannelAdmin::ChannelID id {};
  CosNotification::QoSProperties initial_qos {};
  CosNotification::AdminProperties initial_admin {};
  CosNotifyChannelAdmin::EventChannel_var const event_channel =
    notify_factory->create_channel (initial_qos, initial_admin, id);
  name[0].id = CORBA::string_dup ("MyEventChannel");
  naming_context->rebind (name, event_channel.in ());

  CosNotifyChannelAdmin::AdminID admin_id {};
  CosNotifyChannelAdmin::InterFilterGroupOperator const ifgop = CosNotifyChannelAdmin::AND_OP;
  CosNotifyChannelAdmin::SupplierAdmin_var const supplier_admin =
    event_channel->new_for_suppliers (ifgop, admin_id);
  CosNotifyChannelAdmin::ProxyID supplier_admin_proxy_id {};
  CosNotifyChannelAdmin::ProxyConsumer_var const proxy_consumer =
    supplier_admin->obtain_notification_push_consumer (
      CosNotifyChannelAdmin::STRUCTURED_EVENT,
      supplier_admin_proxy_id);

  PortableServer::Servant_var<StructuredEventSupplier_i> const supplier_servant =
    new StructuredEventSupplier_i (orb_.in ());
  PortableServer::ObjectId_var const oid = poa->activate_object (supplier_servant.in ());
  CORBA::Object_var const supplier_obj = poa->id_to_reference (oid.in ());
  CosNotifyComm::StructuredPushSupplier_var const supplier =
    CosNotifyComm::StructuredPushSupplier::_narrow (supplier_obj.in ());
  consumer_proxy_ = CosNotifyChannelAdmin::StructuredProxyPushConsumer::_narrow (proxy_consumer.in ());
  consumer_proxy_->connect_structured_push_supplier (supplier.in ());
}

CORBA::Boolean
Messenger_i::send_message (char const *user_name,
                           char const *subject,
                           char *&message)
{
  std::cout << "Message from: " << user_name << std::endl;
  std::cout << "Subject:      " << subject << std::endl;
  std::cout << "Message:      " << message << std::endl;

  try
    {
      CosNotification::StructuredEvent event {};
      event.header.fixed_header.event_type.domain_name = CORBA::string_dup ("OCI_TAO");
      event.header.fixed_header.event_type.type_name = CORBA::string_dup ("examples");
      event.header.fixed_header.event_name = CORBA::string_dup ("myevent");

      event.filterable_data.length (3);
      event.filterable_data[0].name = CORBA::string_dup ("From");
      event.filterable_data[0].value <<= user_name;
      event.filterable_data[1].name = CORBA::string_dup ("Subject");
      event.filterable_data[1].value <<= subject;
      event.filterable_data[2].name = CORBA::string_dup ("Message");
      event.filterable_data[2].value <<= message;

      consumer_proxy_->push_structured_event (event);
    }
  catch (CosNotifyComm::InvalidEventType const &)
    {
      std::cerr << "Invalid Event Type Exception" << std::endl;
      return false;
    }
  catch (CORBA::Exception const &ex)
    {
      std::cerr << ex << std::endl;
      return false;
    }

  return true;
}
