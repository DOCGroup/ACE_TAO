// $Id$

#include "ace/OS_NS_unistd.h"
#include "Notify_Structured_Push_Consumer.h"
#include "Notify_Test_Client.h"
#include "common.h"
#include "tao/debug.h"

Notify_Structured_Push_Consumer::Notify_Structured_Push_Consumer (
  const char* name,
  CORBA::Short policy,
  CORBA::Long max_events_per_consumer,
  Notify_Test_Client& client)
  : name_ (name),
  discard_policy_ (policy),
  max_events_per_consumer_ (max_events_per_consumer),
  count_ (0),
  first_ (0),
  client_ (client),
  sent_(40)
{
  this->client_.consumer_start (this);
}


void
Notify_Structured_Push_Consumer::_connect (
  CosNotifyChannelAdmin::ConsumerAdmin_ptr consumer_admin)
{
  CosNotifyComm::StructuredPushConsumer_var objref =
    this->_this ();

  CosNotifyChannelAdmin::ProxySupplier_var proxysupplier =
    consumer_admin->obtain_notification_push_supplier (
    CosNotifyChannelAdmin::STRUCTURED_EVENT,
    proxy_id_);

  this->proxy_ =
    CosNotifyChannelAdmin::StructuredProxyPushSupplier::_narrow (
    proxysupplier.in ());

  CosNotification::QoSProperties properties (2);
  properties.length (2);
  properties[0].name = CORBA::string_dup (CosNotification::DiscardPolicy);
  properties[0].value <<= this->discard_policy_;
  properties[1].name = CORBA::string_dup (CosNotification::MaxEventsPerConsumer);
  properties[1].value <<= this->max_events_per_consumer_;

  this->proxy_->set_qos (properties);
  this->proxy_->connect_structured_push_consumer (objref.in ());

  // give ownership to POA
  this->_remove_ref ();
}


void
Notify_Structured_Push_Consumer::push_structured_event (
  const CosNotification::StructuredEvent& event)
{
  ACE_DEBUG((LM_DEBUG, "-"));

  this->count_++;
  if (this->count_ > max_events_per_consumer_ + 1)
  {
    this->client_.consumer_done (this);
    ACE_ERROR ((LM_ERROR,
      ACE_TEXT ("Structured Consumer (%P|%t): ERROR: too ")
      ACE_TEXT ("many events received (%d).\n"), this->count_));
  }

  ACE_ASSERT(ACE_OS::strcmp(event.header.variable_header[0].name.in(), "Id") == 0);
  CORBA::Any v = event.header.variable_header[0].value;
  CORBA::Long id = 0;
  v >>= id;

  // Force the notify service to queue events
  if (this->count_ == 1)
  {
    ACE_OS::sleep(2);
    first_ = id;
  }

  // @@ The priority header isn't making it through the notify service.
  //n = event.header.variable_header[1].name;
  //if (ACE_OS::strcmp(n.in(), CosNotification::Priority) != 0)
  //{
  //  ACE_ERROR((LM_ERROR, "Error: Couldn't find Priority header in event.\n"));
  //  break;
  //}
  //v = event.header.variable_header[1].value;
  //CORBA::Long priority = 0;
  //v >>= priority;
  CORBA::Long expected = 0;
  if (discard_policy_ == CosNotification::PriorityOrder)
  {
    expected = sent_ - max_events_per_consumer_ + count_;
    if (first_ != sent_ - max_events_per_consumer_ + 1)
      --expected;
  }
  else if (discard_policy_ == CosNotification::FifoOrder)
  {
    expected = sent_ - max_events_per_consumer_ + count_;
    if (first_ != sent_ - max_events_per_consumer_ + 1)
      --expected;
  }
  else if (discard_policy_ == CosNotification::LifoOrder)
  {
    expected = count_;
  }
  else
  {
    ACE_ERROR((LM_ERROR, "Error: Unexpected discard policy.\n"));
    return;
  }

  if (id != expected && count_ != 1)
  {
    ACE_DEBUG((LM_DEBUG, "Error: Expected %d, ", expected));
    this->client_.consumer_done (this);
  }

  ACE_DEBUG((LM_DEBUG, "received %d\n", id));

  // We should receive mepc + 1, because the first event will be in-transit
  // before our sleep causes the notify to queue events.
  // However, on some platforms, we'll only receive mepc, because filtering
  // happened before the first event.
  if (this->count_ >= this->max_events_per_consumer_)
  {
    this->client_.consumer_done (this);
  }
}
