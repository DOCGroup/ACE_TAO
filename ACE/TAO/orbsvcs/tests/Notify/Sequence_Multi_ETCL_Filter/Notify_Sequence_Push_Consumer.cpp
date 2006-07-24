// $Id$

#include "ace/OS_NS_unistd.h"
#include "Notify_Sequence_Push_Consumer.h"
#include "Notify_Test_Client.h"
#include "orbsvcs/TimeBaseC.h"
#include "common.h"
#include "tao/debug.h"

Notify_Sequence_Push_Consumer::Notify_Sequence_Push_Consumer (
  const char* name,
  unsigned int low,
  unsigned int high,
  Notify_Test_Client& client)
  : name_ (name),
  low_ (low),
  high_ (high),
  count_ (0),
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
  properties[0].value <<= (CORBA::Long) 3;
  properties[1].name = CORBA::string_dup (CosNotification::PacingInterval);
  properties[1].value <<= (TimeBase::TimeT) 1 * 1000 * 10000;

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
  ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  ACE_DEBUG((LM_DEBUG, "{%u}", events.length()));

  this->count_ += events.length();

  if (this->count_ > this->high_)
  {
    this->client_.consumer_done (this);
    ACE_ERROR ((LM_ERROR,
      ACE_TEXT ("Sequence Consumer (%P|%t): ERROR: too "
      "many events received.\n")));
    ACE_THROW (CORBA::INTERNAL ());
  }
  else if (this->count_ >= this->low_)
  {
    this->client_.consumer_done (this);
  }
}
