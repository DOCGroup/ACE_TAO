// $Id$

#include "Notify_Structured_Push_Consumer.h"
#include "Notify_Test_Client.h"
#include "common.h"

#include "orbsvcs/TimeBaseC.h"

#include "tao/debug.h"

#include "ace/OS_NS_unistd.h"

Notify_Structured_Push_Consumer::Notify_Structured_Push_Consumer (
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
   first_(0),
   client_ (client)
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

  if (use_ordering_)
  {
    CosNotification::QoSProperties properties (1);
    properties.length (1);
    properties[0].name = CORBA::string_dup (CosNotification::OrderPolicy);
    properties[0].value <<= this->order_policy_;

    this->proxy_->set_qos (properties);
  }

  this->proxy_->connect_structured_push_consumer (objref.in ());

  // give ownership to POA
  this->_remove_ref ();
}


void
Notify_Structured_Push_Consumer::push_structured_event (
                          const CosNotification::StructuredEvent& event)
{
  ACE_DEBUG((LM_DEBUG, "-"));
  if (count_ == 0)
  {
    // Sleep long enough to force the channel to back up, otherwise
    // there will be no ordering.
    ACE_OS::sleep(2);
  }

  ++count_;

  if (this->count_ > this->expected_)
  {
    ACE_ERROR ((LM_ERROR, ACE_TEXT ("ERROR: too many events received.\n")));
  }

  if (this->count_ >= this->expected_)
  {
    this->client_.consumer_done (this);
  }

  CORBA::Long id = 0;

  ACE_ASSERT(event.header.variable_header.length() == 3);
  ACE_ASSERT(ACE_OS::strcmp(event.header.variable_header[0].name.in(), "id") == 0);
  event.header.variable_header[0].value >>= id;

  // The first event won't necessarilly be in order, because we hadn't yet forced
  // the channel to queue events.
  if (count_ > 1)
  {
    if (order_policy_ == CosNotification::PriorityOrder
      || order_policy_ == CosNotification::DeadlineOrder)
    {
      int eid = expected_ - count_ + 1;
      if (eid <= first_)
        --eid;

      if (id != eid)
        ACE_ERROR((LM_ERROR, "\nError: "));
      ACE_DEBUG((LM_DEBUG, "Expected id:%d Received id:%d\n", eid, id));
    }
    else
    {
      if (id != count_ - 1)
        ACE_ERROR((LM_ERROR, "\nError: Expected id:%d Received id:%d\n", count_ - 1, id));
    }
  }
  else
  {
    ACE_DEBUG((LM_DEBUG, "Ignoring first event. id=%d\n", id));
    first_ = id;
  }
}
