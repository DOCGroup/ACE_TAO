// $Id$

#include "CEC_ConsumerAdmin.h"
#include "CEC_ProxyPushSupplier.h"
#include "CEC_ProxyPushConsumer.h"
#include "CEC_EventChannel.h"

#if ! defined (__ACE_INLINE__)
#include "CEC_ConsumerAdmin.i"
#endif /* __ACE_INLINE__ */

ACE_RCSID(CosEvent, CEC_ConsumerAdmin, "$Id$")

TAO_CEC_ConsumerAdmin::TAO_CEC_ConsumerAdmin (TAO_CEC_EventChannel *ec)
  :  event_channel_ (ec)
{
  this->collection_ =
    this->event_channel_->create_proxy_push_supplier_collection ();

  // @@
  // @@ this->collection_->busy_hwm (this->event_channel_->busy_hwm ());
  // @@ this->collection_->max_write_delay (
  // @@           this->event_channel_->max_write_delay ()
  // @@ );
  this->default_POA_ =
    this->event_channel_->consumer_poa ();
}

TAO_CEC_ConsumerAdmin::~TAO_CEC_ConsumerAdmin (void)
{
  this->event_channel_->destroy_proxy_push_supplier_collection (this->collection_);
  this->collection_ = 0;
}

void
TAO_CEC_ConsumerAdmin::connected (TAO_CEC_ProxyPushConsumer *consumer,
                                 CORBA::Environment &ACE_TRY_ENV)
{
  TAO_CEC_Connect_Consumer worker (consumer);

  this->collection_->for_each (&worker, ACE_TRY_ENV);
}

void
TAO_CEC_ConsumerAdmin::reconnected (TAO_CEC_ProxyPushConsumer *consumer,
                                   CORBA::Environment &ACE_TRY_ENV)
{
  TAO_CEC_Reconnect_Consumer worker (consumer);

  this->collection_->for_each (&worker, ACE_TRY_ENV);
}

void
TAO_CEC_ConsumerAdmin::disconnected (TAO_CEC_ProxyPushConsumer *consumer,
                                    CORBA::Environment &ACE_TRY_ENV)
{
  TAO_CEC_Disconnect_Consumer worker (consumer);

  this->collection_->for_each (&worker, ACE_TRY_ENV);
}

void
TAO_CEC_ConsumerAdmin::connected (TAO_CEC_ProxyPushSupplier *supplier,
                                 CORBA::Environment &ACE_TRY_ENV)
{
  this->collection_->connected (supplier, ACE_TRY_ENV);
}

void
TAO_CEC_ConsumerAdmin::reconnected (TAO_CEC_ProxyPushSupplier *supplier,
                                   CORBA::Environment &ACE_TRY_ENV)
{
  this->collection_->reconnected (supplier, ACE_TRY_ENV);
}

void
TAO_CEC_ConsumerAdmin::disconnected (TAO_CEC_ProxyPushSupplier *supplier,
                                    CORBA::Environment &ACE_TRY_ENV)
{
  this->collection_->disconnected (supplier, ACE_TRY_ENV);
}

void
TAO_CEC_ConsumerAdmin::shutdown (CORBA::Environment &ACE_TRY_ENV)
{
  TAO_CEC_Shutdown_Supplier worker;

  this->collection_->for_each (&worker, ACE_TRY_ENV);
  this->collection_->shutdown (ACE_TRY_ENV);
}

CosEventChannelAdmin::ProxyPushSupplier_ptr
TAO_CEC_ConsumerAdmin::obtain_push_supplier (CORBA::Environment &ACE_TRY_ENV)
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  TAO_CEC_ProxyPushSupplier* supplier =
    this->event_channel_->create_proxy_push_supplier ();

  return supplier->_this (ACE_TRY_ENV);
}

CosEventChannelAdmin::ProxyPullSupplier_ptr
TAO_CEC_ConsumerAdmin::obtain_pull_supplier (CORBA::Environment &)
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  return CosEventChannelAdmin::ProxyPullSupplier::_nil ();
}

PortableServer::POA_ptr
TAO_CEC_ConsumerAdmin::_default_POA (CORBA::Environment&)
{
  return PortableServer::POA::_duplicate (this->default_POA_.in ());
}

// ****************************************************************

void
TAO_CEC_Connect_Consumer::work (TAO_CEC_ProxyPushSupplier *supplier,
                               CORBA::Environment &ACE_TRY_ENV)
{
  supplier->connected (this->consumer_, ACE_TRY_ENV);
  ACE_CHECK;
  this->consumer_->connected (supplier, ACE_TRY_ENV);
  ACE_CHECK;
}

// ****************************************************************

void
TAO_CEC_Reconnect_Consumer::work (TAO_CEC_ProxyPushSupplier *supplier,
                                 CORBA::Environment &ACE_TRY_ENV)
{
  supplier->reconnected (this->consumer_, ACE_TRY_ENV);
  ACE_CHECK;
  this->consumer_->reconnected (supplier, ACE_TRY_ENV);
  ACE_CHECK;
}

// ****************************************************************

void
TAO_CEC_Disconnect_Consumer::work (TAO_CEC_ProxyPushSupplier *supplier,
                                  CORBA::Environment &ACE_TRY_ENV)
{
  supplier->disconnected (this->consumer_, ACE_TRY_ENV);
  ACE_CHECK;
  this->consumer_->disconnected (supplier, ACE_TRY_ENV);
  ACE_CHECK;
}

// ****************************************************************

void
TAO_CEC_Shutdown_Supplier::work (TAO_CEC_ProxyPushSupplier *supplier,
                                 CORBA::Environment &ACE_TRY_ENV)
{
  ACE_TRY
    {
      supplier->shutdown (ACE_TRY_ENV);
      ACE_TRY_CHECK;
    }
  ACE_CATCHANY
    {
      // Ignore exceptions
    }
  ACE_ENDTRY;
}


#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)

template class TAO_EC_Worker<TAO_CEC_ProxyPushSupplier>;

#elif defined(ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)

#pragma instantiate TAO_EC_Worker<TAO_CEC_ProxyPushSupplier>

#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */
