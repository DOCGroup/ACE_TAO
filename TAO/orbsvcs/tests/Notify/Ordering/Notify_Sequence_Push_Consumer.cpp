// $Id$

#include "ace/OS_NS_unistd.h"
#include "Notify_Sequence_Push_Consumer.h"
#include "Notify_Test_Client.h"
#include "orbsvcs/TimeBaseC.h"
#include "common.h"
#include "tao/debug.h"

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
  first_ (0),
  client_ (client)
{
  this->client_.consumer_start (this);
}


void
Notify_Sequence_Push_Consumer::_connect (
  CosNotifyChannelAdmin::ConsumerAdmin_ptr consumer_admin
  ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  CosNotifyComm::SequencePushConsumer_var objref =
    this->_this (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;

  CosNotifyChannelAdmin::ProxySupplier_var proxysupplier =
    consumer_admin->obtain_notification_push_supplier (
    CosNotifyChannelAdmin::SEQUENCE_EVENT,
    proxy_id_
    ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  this->proxy_ =
    CosNotifyChannelAdmin::SequenceProxyPushSupplier::_narrow (
    proxysupplier.in () ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  CosNotification::QoSProperties properties (3);
  properties.length (3);
  properties[0].name = CORBA::string_dup (CosNotification::MaximumBatchSize);
  properties[0].value <<= (CORBA::Long)5;
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
  this->proxy_->connect_sequence_push_consumer (objref.in () ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  // give ownership to POA
  this->_remove_ref (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;
}


void
Notify_Sequence_Push_Consumer::push_structured_events (
  const CosNotification::EventBatch& events
  ACE_ENV_ARG_DECL_NOT_USED /*ACE_ENV_SINGLE_ARG_PARAMETER*/)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  if (count_ == 0)
  {
    // Sleep long enough to force the channel to back up, otherwise
    // there will be no ordering.
    ACE_OS::sleep(2);
  }

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
  CORBA::Long id1 = 0;
  CORBA::Long id2 = 0;
  events[0].header.variable_header[0].value >>= id1;
  events[events.length() - 1].header.variable_header[0].value >>= id2;

  ACE_DEBUG((LM_DEBUG, "\n Received id %d-%d\n", id1, id2));

  for (CORBA::ULong idx = 0; idx < events.length(); ++idx)
    {
      CORBA::Long id = 0;

      events[idx].header.variable_header[0].value >>= id;

      // The first batch may be out of order on faster platforms
      if (count_ > static_cast<int>(events.length()))
        {
          int eid = idx + count_ - static_cast<int>(events.length());
          if (order_policy_ == CosNotification::PriorityOrder
              || order_policy_ == CosNotification::DeadlineOrder)
            {
              eid = expected_ - 1;
              eid -= (count_ - static_cast<int>(events.length()));
              eid -= idx;
              if (first_ < static_cast<int>(events.length()))
                eid += static_cast<int>(events.length());
            }
          if (id != eid)
            ACE_ERROR((LM_ERROR, "\nError: Expected id:%d Received id:%d\n", eid, id));
        }
      else
        {
          first_ = id1;
        }
    }
}
