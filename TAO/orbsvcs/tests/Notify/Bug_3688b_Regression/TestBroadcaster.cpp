#include "TestBroadcaster.h"

TestBroadcaster::TestBroadcaster () = default;

TestBroadcaster::~TestBroadcaster () noexcept
{
  try
    {
      this->disconnect ();
    }
  catch (CORBA::Exception const& ex)
    {
      ex._tao_print_exception ("TestBroadcaster::~TestBroadcaster: ");
    }
  catch (...)
    {
    }
}

bool
TestBroadcaster::connect (CORBA::ORB_ptr orb, std::string const& channel_ior)
{
  try
    {
      CORBA::Object_var const object = orb->string_to_object (channel_ior.c_str ());
      CosNotifyChannelAdmin::EventChannel_var const event_channel =
        CosNotifyChannelAdmin::EventChannel::_narrow (object.in ());
      CosNotifyChannelAdmin::AdminID supplier_admin_id {};
      CosNotifyChannelAdmin::SupplierAdmin_var const supplier_admin =
        event_channel->new_for_suppliers (CosNotifyChannelAdmin::OR_OP, supplier_admin_id);
      CosNotifyChannelAdmin::ProxyID proxy_consumer_id {};
      CosNotifyChannelAdmin::ProxyConsumer_var const proxy_consumer =
        supplier_admin->obtain_notification_push_consumer (
          CosNotifyChannelAdmin::SEQUENCE_EVENT,
          proxy_consumer_id);

      this->sequence_proxy_push_consumer =
        CosNotifyChannelAdmin::SequenceProxyPushConsumer::_narrow (proxy_consumer.in ());
      this->sequence_proxy_push_consumer->connect_sequence_push_supplier (
        CosNotifyComm::SequencePushSupplier::_nil ());
    }
  catch (CORBA::Exception const& ex)
    {
      ex._tao_print_exception ("TestBroadcaster::connect: ");
      return false;
    }
  catch (...)
    {
      return false;
    }

  return true;
}

bool
TestBroadcaster::disconnect ()
{
  try
    {
      if (this->is_connected ())
        {
          this->sequence_proxy_push_consumer->disconnect_sequence_push_consumer ();
          this->sequence_proxy_push_consumer =
            CosNotifyChannelAdmin::SequenceProxyPushConsumer_var ();
        }
    }
  catch (CORBA::Exception const& ex)
    {
      ex._tao_print_exception ("TestBroadcaster::disconnect: ");
      return false;
    }
  catch (...)
    {
      return false;
    }

  return true;
}

bool
TestBroadcaster::send_data ()
{
  try
    {
      CosNotification::EventBatch events;
      events.length (5);

      for (CORBA::ULong index {}, count = events.length (); index < count; ++index)
        {
          events[index].header.fixed_header.event_type.domain_name = "Test";
          events[index].header.fixed_header.event_type.type_name = "Data";
          events[index].header.fixed_header.event_name = "Info";
          events[index].header.variable_header.length (0);
          events[index].filterable_data.length (1);
          events[index].filterable_data[0].name = "data";
          events[index].filterable_data[0].value <<= index;
          events[index].remainder_of_body <<= index;
        }

      this->sequence_proxy_push_consumer->push_structured_events (events);
    }
  catch (CORBA::Exception const& ex)
    {
      ex._tao_print_exception ("TestBroadcaster::send_data: ");
      return false;
    }
  catch (...)
    {
      return false;
    }

  return true;
}

bool
TestBroadcaster::is_connected () const
{
  return !CORBA::is_nil (this->sequence_proxy_push_consumer.in ());
}
