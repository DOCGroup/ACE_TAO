#include "TestListener.h"

TestListener::TestListener (
  CORBA::ORB* orb,
  std::string const& channel_ior,
  std::string const& filter_expression)
{
  CORBA::Object_var const poa_object = orb->resolve_initial_references ("RootPOA");
  this->root_poa = PortableServer::POA::_narrow (poa_object.in ());
  this->object_id = this->root_poa->activate_object (this);
  this->object_reference = this->root_poa->id_to_reference (this->object_id.in ());

  CORBA::Object_var channel_object = orb->string_to_object (channel_ior.c_str ());
  this->event_channel = CosNotifyChannelAdmin::EventChannel::_narrow (channel_object.in ());
  this->consumer_admin =
    this->event_channel->new_for_consumers (CosNotifyChannelAdmin::AND_OP,
                                            this->consumer_admin_id);

  CosNotifyFilter::FilterFactory_var const filter_factory =
    this->event_channel->default_filter_factory ();
  CosNotifyFilter::Filter_var const filter = filter_factory->create_filter ("EXTENDED_TCL");

  CosNotifyFilter::ConstraintExpSeq constraints (1);
  constraints.length (1);
  constraints[0].event_types.length (1);
  constraints[0].event_types[0].domain_name = "Test";
  constraints[0].event_types[0].type_name = "Data";
  constraints[0].constraint_expr = filter_expression.c_str ();
  filter->add_constraints (constraints);
  this->consumer_admin->add_filter (filter.in ());

  channel_object = this->root_poa->id_to_reference (this->object_id.in ());
  this->structured_push_consumer =
    CosNotifyComm::StructuredPushConsumer::_narrow (channel_object.in ());

  CosNotifyChannelAdmin::ProxySupplier_var const proxy_supplier =
    this->consumer_admin->obtain_notification_push_supplier (
      CosNotifyChannelAdmin::STRUCTURED_EVENT,
      this->proxy_supplier_id);
  this->proxy_supplier =
    CosNotifyChannelAdmin::StructuredProxyPushSupplier::_narrow (proxy_supplier.in ());
  this->proxy_supplier->connect_structured_push_consumer (this->structured_push_consumer.in ());
}

TestListener::~TestListener ()
{
  if (!CORBA::is_nil (this->proxy_supplier.in ()))
    {
      // Disconnecting the supplier implicitly destroys the proxy.
      this->proxy_supplier->disconnect_structured_push_supplier ();
      this->proxy_supplier = CosNotifyChannelAdmin::StructuredProxyPushSupplier_var ();
    }

  if (!CORBA::is_nil (this->structured_push_consumer.in ()) && !CORBA::is_nil (this->root_poa.in ()))
    this->root_poa->deactivate_object (this->object_id.in ());
}

void
TestListener::push_structured_event (CosNotification::StructuredEvent const& notification)
{
  CORBA::Long data {};
  notification.remainder_of_body >>= data;
  this->received_data.push_back (data);
}

void
TestListener::disconnect_structured_push_consumer ()
{
}

void
TestListener::offer_change (
  CosNotification::EventTypeSeq const&,
  CosNotification::EventTypeSeq const&)
{
}
