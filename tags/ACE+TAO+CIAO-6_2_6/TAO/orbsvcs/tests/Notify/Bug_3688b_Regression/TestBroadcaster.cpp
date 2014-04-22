// $Id$
#include "TestBroadcaster.h"


TestBroadcaster::TestBroadcaster()
{
} /* end of TestBroadcaster::TestBroadcaster */


TestBroadcaster::~TestBroadcaster()
  throw()
{
  // ensure nothrow guarantee
  try
  {
    disconnect();
  } /* end of try */
  catch(...)
  {
  }
} /* end of TestBroadcaster::~TestBroadcaster */


bool TestBroadcaster::connect(
  CORBA::ORB_ptr p_orb,
  std::string const & rc_channelIor
)
{
  try
  {
    CosNotifyChannelAdmin::EventChannel_var v_eventChannel;
    CosNotifyChannelAdmin::SupplierAdmin_var v_supplierAdmin;
    CosNotifyChannelAdmin::ProxyConsumer_var v_proxyConsumer;
    CosNotifyChannelAdmin::AdminID supplierAdminId;
    CosNotifyChannelAdmin::ProxyID proxyConsumerId;

    CORBA::Object_var v_obj = p_orb->string_to_object(rc_channelIor.c_str());

    v_eventChannel = CosNotifyChannelAdmin::EventChannel::_narrow(v_obj.in());

    v_supplierAdmin =
      v_eventChannel->new_for_suppliers (
      CosNotifyChannelAdmin::OR_OP,
      supplierAdminId
    );

    v_proxyConsumer = v_supplierAdmin->obtain_notification_push_consumer(
      CosNotifyChannelAdmin::SEQUENCE_EVENT,
      proxyConsumerId
    );

    mv_sequenceProxyPushConsumer =
      CosNotifyChannelAdmin::SequenceProxyPushConsumer::_narrow(
        v_proxyConsumer.in()
      );

    mv_sequenceProxyPushConsumer->connect_sequence_push_supplier(
      CosNotifyComm::SequencePushSupplier::_nil()
    );
  }
  catch(...)
  {
    return false;
  }

  return true;
} /* end of TestBroadcaster::connect */


bool TestBroadcaster::disconnect()
{
  try
  {
    if(isConnected())
    {
      mv_sequenceProxyPushConsumer->disconnect_sequence_push_consumer();
      mv_sequenceProxyPushConsumer = CosNotifyChannelAdmin::SequenceProxyPushConsumer_var();
    }
  } /* end of try */
  catch(...)
  {
    return false;
  }

  return true;
} /* end of TestBroadcaster::disconnect */


bool TestBroadcaster::sendData( )
{
  try
  {
    CosNotification::EventBatch events;
    events.length(5);

    for (CORBA::ULong idx = 0, max = events.length(); idx < max; ++idx)
    {
      events[idx].header.fixed_header.event_type.domain_name = "Test";
      events[idx].header.fixed_header.event_type.type_name = "Data";
      events[idx].header.fixed_header.event_name = "Info";
      events[idx].header.variable_header.length(0);

      // provide filter criteria
      events[idx].filterable_data.length(1);
      events[idx].filterable_data[0].name = "data";
      events[idx].filterable_data[0].value <<= idx;

      events[idx].remainder_of_body <<= idx;
    } /* end of for */

    mv_sequenceProxyPushConsumer->push_structured_events(events);
  }
  catch(...)
  {
    return false;
  }

  return true;
} /* end of TestBroadcaster::sendData */


bool TestBroadcaster::isConnected() const
{
  return !CORBA::is_nil(mv_sequenceProxyPushConsumer.in());
} /* end of TestBroadcaster::isConnected */
