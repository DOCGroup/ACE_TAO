// $Id$

#include "Notify_Sequence_Push_Consumer.h"
#include "Notify_Test_Client.h"
#include "common.h"
#include "orbsvcs/TimeBaseC.h"
#include "tao/debug.h"
#include "ace/OS_NS_unistd.h"

const int BATCH_SIZE = 5;

Notify_Sequence_Push_Consumer::Notify_Sequence_Push_Consumer (
  const char* name,
  CORBA::Short policy,
  bool use_ordering,
  int expected,
  Notify_Test_Client& client)
  : name_ (name),
  order_policy_ (policy),
  use_ordering_ (use_ordering),
  expected_ (expected),
  count_ (0),
  previous_first_ (0),
  client_ (client)
{
  this->client_.consumer_start (this);
}


void
Notify_Sequence_Push_Consumer::_connect (
  CosNotifyChannelAdmin::ConsumerAdmin_ptr consumer_admin)
{
  CosNotifyComm::SequencePushConsumer_var objref =
    this->_this ();

  CosNotifyChannelAdmin::ProxySupplier_var proxysupplier =
    consumer_admin->obtain_notification_push_supplier (
    CosNotifyChannelAdmin::SEQUENCE_EVENT,
    proxy_id_);

  this->proxy_ =
    CosNotifyChannelAdmin::SequenceProxyPushSupplier::_narrow (
    proxysupplier.in ());

  CosNotification::QoSProperties properties (3);
  properties.length (3);
  properties[0].name = CORBA::string_dup (CosNotification::MaximumBatchSize);
  properties[0].value <<= (CORBA::Long) BATCH_SIZE;
  properties[1].name = CORBA::string_dup (CosNotification::PacingInterval);
  properties[1].value <<= (TimeBase::TimeT) (1000 * 10000); // 1 secs
  if (use_ordering_)
  {
    properties[2].name = CORBA::string_dup (CosNotification::OrderPolicy);
    properties[2].value <<= this->order_policy_;
  }
  else
  {
    properties.length(2);
  }

  this->proxy_->set_qos (properties);
  this->proxy_->connect_sequence_push_consumer (objref.in ());

  // give ownership to POA
  this->_remove_ref ();
}


void
Notify_Sequence_Push_Consumer::push_structured_events (
  const CosNotification::EventBatch& events)
{
  if (count_ == 0)
  {
    // Sleep long enough to force the channel to back up, otherwise
    // there will be no ordering.
    ACE_OS::sleep(2);
  }

  ACE_ASSERT(events.length() == static_cast<CORBA::ULong>(BATCH_SIZE));

  count_ += events.length();

  if (this->count_ > this->expected_)
  {
    ACE_ERROR ((LM_ERROR, ACE_TEXT ("ERROR: too many events received.\n")));
  }

  if (this->count_ >= this->expected_)
  {
    this->client_.consumer_done (this);
  }

  ACE_ASSERT(events[0].header.variable_header.length() == 3);
  ACE_ASSERT(ACE_OS::strcmp(events[0].header.variable_header[0].name.in(), "id") == 0);
  CORBA::Long first_id = 0;
  events[0].header.variable_header[0].value >>= first_id;
  CORBA::Long last_id = 0;
  events[events.length() - 1].header.variable_header[0].value >>= last_id;

  ACE_DEBUG((LM_DEBUG, "\n Received id %d-%d\n", first_id, last_id));

  int events_length = static_cast<int>(events.length());

  CORBA::Long previous_id = first_id;

  if (count_ > events_length) // Ignore the very first batch
  {
    // First check that the sequences are ordered correctly
    for (CORBA::ULong idx = 1; idx < events.length(); ++idx)
    {
      CORBA::Long id = 0;

      events[idx].header.variable_header[0].value >>= id;
      CORBA::Long expected_id = previous_id + 1;
      if (order_policy_ == CosNotification::PriorityOrder
        || order_policy_ == CosNotification::DeadlineOrder)
      {
        expected_id = previous_id - 1;
      }
      if (id != expected_id)
      {
        ACE_ERROR((LM_ERROR, "Error: Expected:%d Received:%d\n", expected_id, id));
        return;
      }
      previous_id = id;
    }

    // Next check that the first id in the sequence is ordered
    // relative to the previously retrieved sequence.
    if (previous_first_ != 0)
    {
      CORBA::Long expected_id = previous_first_ + BATCH_SIZE;
      if (order_policy_ == CosNotification::PriorityOrder
        || order_policy_ == CosNotification::DeadlineOrder)
      {
        expected_id = previous_first_ - BATCH_SIZE;
      }
      if (first_id != expected_id)
      {
        ACE_ERROR((LM_ERROR, "Error: Expected:%d Received:%d\n", expected_id, first_id));
        return;
      }
    }
    previous_first_ = first_id;
  }
}
