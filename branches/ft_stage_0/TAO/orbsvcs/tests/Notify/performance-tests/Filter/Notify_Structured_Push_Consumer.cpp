// $Id$

#include "Notify_Structured_Push_Consumer.h"
#include "common.h"

Notify_Structured_Push_Consumer::Notify_Structured_Push_Consumer (
                                            const char* name,
                                            unsigned int expected,
                                            CORBA::Boolean& done)
 : name_ (name),
   expected_ (expected),
   count_ (0),
   done_ (done)
{
}


Notify_Structured_Push_Consumer::~Notify_Structured_Push_Consumer ()
{
}


void
Notify_Structured_Push_Consumer::connect (
                CosNotifyChannelAdmin::ConsumerAdmin_ptr consumer_admin
                ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  CosNotifyComm::StructuredPushConsumer_var objref =
    this->_this (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;

  CosNotifyChannelAdmin::ProxySupplier_var proxysupplier =
    consumer_admin->obtain_notification_push_supplier (
      CosNotifyChannelAdmin::STRUCTURED_EVENT,
      proxy_id_
      ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  this->proxy_ =
    CosNotifyChannelAdmin::StructuredProxyPushSupplier::_narrow (
      proxysupplier.in () ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  proxy_->connect_structured_push_consumer (objref.in ()
                                                     ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  this->_remove_ref ();
}


void
Notify_Structured_Push_Consumer::push_structured_event(
                          const CosNotification::StructuredEvent& /*event*/
                          ACE_ENV_ARG_DECL_NOT_USED /*ACE_ENV_SINGLE_ARG_PARAMETER*/)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  if (++this->count_ == this->expected_)
    {
      this->done_ = 1;
    }
}

