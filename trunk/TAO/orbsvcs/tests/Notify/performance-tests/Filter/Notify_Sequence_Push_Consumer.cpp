// $Id$

#include "Notify_Sequence_Push_Consumer.h"
#include "Notify_Test_Client.h"
#include "common.h"

#include "ace/OS_NS_sys_time.h"

Notify_Sequence_Push_Consumer::Notify_Sequence_Push_Consumer (
  const char* name,
  unsigned int expected,
  Notify_Test_Client& client)
  : name_ (name),
  expected_ (expected),
  count_ (0),
  client_ (client)
{
  this->client_.consumer_start (this);
}


Notify_Sequence_Push_Consumer::~Notify_Sequence_Push_Consumer ()
{
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

  CosNotification::QoSProperties properties(1);
  properties.length(1);
  properties[0].name = CORBA::string_dup(CosNotification::MaximumBatchSize);
  properties[0].value <<= (CORBA::Long)10;

  proxy_->set_qos(properties);
  proxy_->connect_sequence_push_consumer (objref.in ()
    ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  // give ownership to POA
  this->_remove_ref (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;
}


void
Notify_Sequence_Push_Consumer::push_structured_events(
  const CosNotification::EventBatch& events
  ACE_ENV_ARG_DECL_NOT_USED /*ACE_ENV_SINGLE_ARG_PARAMETER*/)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  if ( this->count_ == 0 )
  {
    this->first_ = ACE_OS::gettimeofday();
  }

  this->count_ += events.length();

  if (this->count_ > this->expected_)
  {
    ACE_ERROR ((LM_ERROR, ACE_TEXT ("\nError: Expected %d events. Received %d\n"), expected_, count_));
    this->client_.consumer_done (this);
    return;
  }

  if (this->count_ >= this->expected_)
  {
    this->client_.consumer_done (this);
    ACE_Time_Value totaltime = ( ACE_OS::gettimeofday() - this->first_ );
    double events_per_ms = static_cast<double>(this->count_) /
      static_cast<double>(totaltime.msec());
    ACE_DEBUG((LM_DEBUG, "\nConsumer %s received %d at %.2f events/ms\n", name_.c_str(), this->count_, events_per_ms ));
  }
}
