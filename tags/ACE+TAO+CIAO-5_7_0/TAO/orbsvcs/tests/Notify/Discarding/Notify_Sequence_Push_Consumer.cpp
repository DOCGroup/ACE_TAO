// $Id$

#include "ace/OS_NS_unistd.h"
#include "Notify_Sequence_Push_Consumer.h"
#include "Notify_Test_Client.h"
#include "orbsvcs/TimeBaseC.h"
#include "common.h"
#include "tao/debug.h"

// The NS will try to send in M size chunks, but may send smaller if
// the pacing interval expires. We set the pacing interval large
// enough so that *should* not happen.
// Note : This batch size must be coordinated with the events sent
// by the supplier. For example, when discard policy is set to
// PriorityOrder, then we want to have exactly BATCH_SIZE events
// with the highest priority so that we know we received the right ones.
// We also need to be sure to send a multiple of the batch size.
static const CORBA::Long BATCH_SIZE = 4;

static const TimeBase::TimeT PACING = 20 * 1000 * 10000;

// This needs to be big enough to ensure that the supplier has sent
// all the events.
static const int FIRST_SLEEP_SECS = 1;

Notify_Sequence_Push_Consumer::Notify_Sequence_Push_Consumer (
                                            const char* name,
                                            CORBA::Short policy,
                                            Notify_Test_Client& client,
                                            int sent)
 : name_ (name)
 , discard_policy_ (policy)
 , count_ (0)
 , client_ (client)
 , sent_(sent)
 , first_(0)
{
  this->client_.consumer_start (this);
}


void
Notify_Sequence_Push_Consumer::_connect (
                CosNotifyChannelAdmin::ConsumerAdmin_ptr consumer_admin)
{
  CosNotifyComm::SequencePushConsumer_var consumer =
    this->_this ();

  CosNotifyChannelAdmin::ProxySupplier_var proxysupplier =
    consumer_admin->obtain_notification_push_supplier (
      CosNotifyChannelAdmin::SEQUENCE_EVENT,
      proxy_id_);

  this->proxy_ =
    CosNotifyChannelAdmin::SequenceProxyPushSupplier::_narrow (
      proxysupplier.in ());

  CosNotification::QoSProperties properties (4);
  properties.length (4);
  CORBA::ULong idx = 0;
  properties[idx].name = CORBA::string_dup (CosNotification::MaximumBatchSize);
  properties[idx].value <<= BATCH_SIZE;
  properties[++idx].name = CORBA::string_dup (CosNotification::PacingInterval);
  properties[idx].value <<= PACING;
  properties[++idx].name = CORBA::string_dup (CosNotification::DiscardPolicy);
  properties[idx].value <<= this->discard_policy_;
  properties[++idx].name = CORBA::string_dup (CosNotification::MaxEventsPerConsumer);
  // We set this equal to the batch size so that we conveniently always receive
  // one more batch after the first.
  properties[idx].value <<= BATCH_SIZE;

  this->proxy_->set_qos (properties);
  this->proxy_->connect_sequence_push_consumer (consumer.in ());

  // give ownership to POA
  this->_remove_ref ();
}


void
Notify_Sequence_Push_Consumer::push_structured_events (
                          const CosNotification::EventBatch& events)
{
  ++count_;

  // The pacing interval is set large enough that it should never come into
  // play unless something goes wrong.
  if (events.length() != static_cast<size_t>(BATCH_SIZE))
  {
    ACE_ERROR((LM_ERROR,
      "Error : Unexpected batch size %u/%u\n", events.length(),
      BATCH_SIZE));
    this->client_.consumer_done (this);
  }
  ACE_ASSERT(ACE_OS::strcmp(events[0].header.variable_header[0].name.in(), "Id") == 0);

  CORBA::Long id1 = 0;
  CORBA::Long id2 = 0;
  events[0].header.variable_header[0].value >>= id1;
  events[events.length() - 1].header.variable_header[0].value >>= id2;

  ACE_DEBUG((LM_DEBUG, "{%d-%d}\n", id1, id2));

  if (count_ == 1)
  {
    // We sleep long enough after the first batch to ensure that
    // the supplier has time to send all the events. This will allow
    // the notify service to discard all but one batch.
    ACE_OS::sleep(FIRST_SLEEP_SECS);

    first_ = id1;
  }

  // Validate the batch is ordered
  for (CORBA::Long i = 1; i < BATCH_SIZE; ++i)
  {
    CORBA::Long id = 0;
    events[i].header.variable_header[0].value >>= id;
    if (id != id1 + i)
    {
      ACE_ERROR((LM_ERROR, "Error: Invalid batch. Expected %d, Was %d\n", id1 + i, id));
      this->client_.consumer_done (this);
      return;
    }
  }

  CORBA::Long expected = 0;

  if (discard_policy_ == CosNotification::PriorityOrder)
  {
    expected = sent_ - BATCH_SIZE + 1; // e.g. 37, 38, 39, 40
  }
  else if (discard_policy_ == CosNotification::FifoOrder)
  {
    expected = sent_ - BATCH_SIZE + 1; // e.g. 37, 38, 39, 40
  }
  else
  {
    ACE_ASSERT(discard_policy_ == CosNotification::LifoOrder);
    expected = BATCH_SIZE + 1; // e.g. 5, 6, 7, 8
  }

  // On some slower platforms, the discard policy may be applied before the first
  // batch is sent. In that case we may only get a single batch.
  // On other platforms we may get two batches, but the first batch
  // may or may not conform to the discard policy.

  if (count_ == 1 && id1 != expected && discard_policy_ != CosNotification::LifoOrder)
  {
    // We expect to get another batch with the correct one.
  }
  else
  {
    this->client_.consumer_done (this);
  }

  if (count_ > 2)
  {
    ACE_ERROR((LM_ERROR, "Error : Too many batches received.\n"));
  }
}
