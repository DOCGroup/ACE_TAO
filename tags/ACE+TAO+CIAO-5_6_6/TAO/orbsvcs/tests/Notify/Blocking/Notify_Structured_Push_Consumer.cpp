// $Id$

#include "ace/OS_NS_unistd.h"
#include "Notify_Structured_Push_Consumer.h"
#include "Notify_Test_Client.h"
#include "orbsvcs/Notify/Notify_Extensions.h"
#include "common.h"
#include "tao/debug.h"

static const int CONSUMER_DELAY = 1; // seconds.

Notify_Structured_Push_Consumer::Notify_Structured_Push_Consumer (
                                            const char* name,
                                            TimeBase::TimeT block,
                                            unsigned int expected,
                                            Notify_Test_Client& client)
 : name_ (name),
   blocking_timeout_ (block),
   expected_ (expected),
   count_ (0),
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

  CosNotification::QoSProperties properties (2);
  properties.length (2);
  // The discard policy and admin properties tests already test using
  // the MaxQueueLength policy, so we'll use MEPC instead. It should
  // make no difference.
  // If the blocking timeout is less than CONSUMER_DELAY seconds, then
  // we want to ensure that exactly one event is discarded, so we set
  // the MEPC to expected_ - 1. If the supplier sends 20, then we expect 19,
  // and we set MEPC to 18. The first event will be dispatched at once, but
  // will block in the consumer. This will allow the queue in the notify svc
  // to fill up to 18. However, the blocking code will timeout before the
  // consumer finishes which will cause an event to be discarded. This will
  // allow the last event to be queued. Eventually the consumer will unblock
  // and receive the remaining events
  properties[0].name = CORBA::string_dup (CosNotification::MaxEventsPerConsumer);
  if (blocking_timeout_ < CONSUMER_DELAY * 1000 * 1000 * 10)
    properties[0].value <<= (CORBA::Long) expected_ - 1;
  else
    properties[0].value <<= (CORBA::Long) 10;


  properties[1].name = CORBA::string_dup (TAO_Notify_Extensions::BlockingPolicy);
  properties[1].value <<= this->blocking_timeout_;

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
  ACE_UNUSED_ARG(event);

  this->count_++;
  if (this->count_ > this->expected_)
  {
    ACE_ERROR ((LM_ERROR,
      ACE_TEXT ("Structured Consumer (%P|%t): ERROR: too "
      "many events received.\n")));
  }

  if (this->count_ >= this->expected_)
  {
    ACE_DEBUG((LM_DEBUG, "\nConsumer received %u events.\n", count_));
    this->client_.consumer_done (this);
  }

  // By pausing here, we force the channel to back up, which will
  // either result in discarding of events, or blocking, depending
  // on whether our BlockingPolicy is greater than the following
  // delay time.
  // A BlockingPolicy > 1 second should allow the first event.
  // A BlockingPolicy < 1 second should discard the first event.
  if (count_ == 1)
    ACE_OS::sleep (CONSUMER_DELAY);
}

