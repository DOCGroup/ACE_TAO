// $Id$

#include "CEC_SupplierAdmin.h"
#include "CEC_ProxyPushConsumer.h"
#include "CEC_ProxyPullConsumer.h"
#include "CEC_EventChannel.h"

#if ! defined (__ACE_INLINE__)
#include "CEC_SupplierAdmin.i"
#endif /* __ACE_INLINE__ */

ACE_RCSID(CosEvent, CEC_SupplierAdmin, "$Id$")

TAO_CEC_SupplierAdmin::TAO_CEC_SupplierAdmin (TAO_CEC_EventChannel *ec)
  :  event_channel_ (ec)
{
  this->push_collection_ =
    this->event_channel_->create_proxy_push_consumer_collection ();

  this->pull_collection_ =
    this->event_channel_->create_proxy_pull_consumer_collection ();

  // @@
  // @@ this->collection_->busy_hwm (this->event_channel_->busy_hwm ());
  // @@ this->collection_->max_write_delay (
  // @@           this->event_channel_->max_write_delay ()
  // @@ );

  this->default_POA_ =
    this->event_channel_->supplier_poa ();
}

TAO_CEC_SupplierAdmin::~TAO_CEC_SupplierAdmin (void)
{
  this->event_channel_->destroy_proxy_push_consumer_collection (this->push_collection_);
  this->push_collection_ = 0;

  this->event_channel_->destroy_proxy_pull_consumer_collection (this->pull_collection_);
  this->pull_collection_ = 0;
}

PortableServer::POA_ptr
TAO_CEC_SupplierAdmin::_default_POA (CORBA::Environment&)
{
  return PortableServer::POA::_duplicate (this->default_POA_.in ());
}

void
TAO_CEC_SupplierAdmin::connected (TAO_CEC_ProxyPushConsumer * /*consumer*/,
                                  CORBA::Environment & /*ACE_TRY_ENV*/)
{
  // this->push_collection_->connected (consumer, ACE_TRY_ENV);
}

void
TAO_CEC_SupplierAdmin::reconnected (TAO_CEC_ProxyPushConsumer *consumer,
                                    CORBA::Environment &ACE_TRY_ENV)
{
  this->push_collection_->reconnected (consumer, ACE_TRY_ENV);
}

void
TAO_CEC_SupplierAdmin::disconnected (TAO_CEC_ProxyPushConsumer *consumer,
                                     CORBA::Environment &ACE_TRY_ENV)
{
  this->push_collection_->disconnected (consumer, ACE_TRY_ENV);
}

void
TAO_CEC_SupplierAdmin::connected (TAO_CEC_ProxyPullConsumer * /*consumer*/,
                                  CORBA::Environment & /*ACE_TRY_ENV*/)
{
  // this->pull_collection_->connected (consumer, ACE_TRY_ENV);
}

void
TAO_CEC_SupplierAdmin::reconnected (TAO_CEC_ProxyPullConsumer *consumer,
                                    CORBA::Environment &ACE_TRY_ENV)
{
  this->pull_collection_->reconnected (consumer, ACE_TRY_ENV);
}

void
TAO_CEC_SupplierAdmin::disconnected (TAO_CEC_ProxyPullConsumer *consumer,
                                     CORBA::Environment &ACE_TRY_ENV)
{
  this->pull_collection_->disconnected (consumer, ACE_TRY_ENV);
}

void
TAO_CEC_SupplierAdmin::shutdown (CORBA::Environment &ACE_TRY_ENV)
{
  TAO_CEC_Shutdown_Push_Consumer push_worker;

  this->push_collection_->for_each (&push_worker, ACE_TRY_ENV);
  ACE_CHECK;
  this->push_collection_->shutdown (ACE_TRY_ENV);
  ACE_CHECK;

  TAO_CEC_Shutdown_Pull_Consumer pull_worker;

  this->pull_collection_->for_each (&pull_worker, ACE_TRY_ENV);
  ACE_CHECK;
  this->pull_collection_->shutdown (ACE_TRY_ENV);
  ACE_CHECK;
}

CosEventChannelAdmin::ProxyPushConsumer_ptr
TAO_CEC_SupplierAdmin::obtain_push_consumer (CORBA::Environment &ACE_TRY_ENV)
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  TAO_CEC_ProxyPushConsumer* consumer =
    this->event_channel_->create_proxy_push_consumer ();

  PortableServer::ServantBase_var holder = consumer;

  CosEventChannelAdmin::ProxyPushConsumer_var result =
    consumer->_this (ACE_TRY_ENV);
  ACE_CHECK_RETURN (CosEventChannelAdmin::ProxyPushConsumer::_nil ());

  this->push_collection_->connected (consumer, ACE_TRY_ENV);
  ACE_CHECK_RETURN (CosEventChannelAdmin::ProxyPushConsumer::_nil ());
  
  return result._retn ();
}

CosEventChannelAdmin::ProxyPullConsumer_ptr
TAO_CEC_SupplierAdmin::obtain_pull_consumer (CORBA::Environment &ACE_TRY_ENV)
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  TAO_CEC_ProxyPullConsumer* consumer =
    this->event_channel_->create_proxy_pull_consumer ();

  PortableServer::ServantBase_var holder = consumer;

  CosEventChannelAdmin::ProxyPullConsumer_var result =
    consumer->_this (ACE_TRY_ENV);
  ACE_CHECK_RETURN (CosEventChannelAdmin::ProxyPullConsumer::_nil ());

  this->pull_collection_->connected (consumer, ACE_TRY_ENV);
  ACE_CHECK_RETURN (CosEventChannelAdmin::ProxyPullConsumer::_nil ());
  
  return result._retn ();
}

// ****************************************************************

void
TAO_CEC_Shutdown_Push_Consumer::work (TAO_CEC_ProxyPushConsumer *consumer,
                                      CORBA::Environment &ACE_TRY_ENV)
{
  ACE_TRY
    {
      consumer->shutdown (ACE_TRY_ENV);
      ACE_TRY_CHECK;
    }
  ACE_CATCHANY
    {
      // Ignore exceptions
    }
  ACE_ENDTRY;
}

// ****************************************************************

void
TAO_CEC_Shutdown_Pull_Consumer::work (TAO_CEC_ProxyPullConsumer *consumer,
                                      CORBA::Environment &ACE_TRY_ENV)
{
  ACE_TRY
    {
      consumer->shutdown (ACE_TRY_ENV);
      ACE_TRY_CHECK;
    }
  ACE_CATCHANY
    {
      // Ignore exceptions
    }
  ACE_ENDTRY;
}

// ****************************************************************

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)

template class TAO_EC_Worker<TAO_CEC_ProxyPushConsumer>;
template class TAO_EC_Worker<TAO_CEC_ProxyPullConsumer>;

#elif defined(ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)

#pragma instantiate TAO_EC_Worker<TAO_CEC_ProxyPushConsumer>
#pragma instantiate TAO_EC_Worker<TAO_CEC_ProxyPullConsumer>

#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */
