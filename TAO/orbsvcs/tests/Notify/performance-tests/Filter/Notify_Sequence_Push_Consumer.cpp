// $Id$

#include "Notify_Sequence_Push_Consumer.h"
#include "common.h"

Notify_Sequence_Push_Consumer::Notify_Sequence_Push_Consumer (
                                            const char* name,
                                            unsigned int expected,
                                            CORBA::Boolean& done)
 : name_ (name),
   expected_ (expected),
   count_ (0),
   done_ (done)
{
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
  this->_remove_ref ();
}


void
Notify_Sequence_Push_Consumer::push_structured_events(
                          const CosNotification::EventBatch& /*events*/
                          ACE_ENV_ARG_DECL_NOT_USED /*ACE_ENV_SINGLE_ARG_PARAMETER*/)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  if (++this->count_ == this->expected_)
    {
      this->done_ = 1;
    }
}
