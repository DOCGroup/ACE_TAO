// $Id$

#include "CEC_ConsumerAdmin.h"
#include "CEC_ProxyPushSupplier.h"
#include "CEC_ProxyPullSupplier.h"
#include "CEC_ProxyPushConsumer.h"
#include "CEC_EventChannel.h"

#if ! defined (__ACE_INLINE__)
#include "CEC_ConsumerAdmin.i"
#endif /* __ACE_INLINE__ */

ACE_RCSID(CosEvent, CEC_ConsumerAdmin, "$Id$")

TAO_CEC_ConsumerAdmin::TAO_CEC_ConsumerAdmin (TAO_CEC_EventChannel *ec)
  :  event_channel_ (ec)
{
  this->push_collection_ =
    this->event_channel_->create_proxy_push_supplier_collection ();
  this->pull_collection_ =
    this->event_channel_->create_proxy_pull_supplier_collection ();

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
  this->event_channel_->destroy_proxy_push_supplier_collection (this->push_collection_);
  this->push_collection_ = 0;
  this->event_channel_->destroy_proxy_pull_supplier_collection (this->pull_collection_);
  this->pull_collection_ = 0;
}

void
TAO_CEC_ConsumerAdmin::push (const CORBA::Any &event,
                             CORBA::Environment &ACE_TRY_ENV)
{
  TAO_CEC_Propagate_Event_Push push_worker (event);

  this->push_collection_->for_each (&push_worker,
                                    ACE_TRY_ENV);
  ACE_CHECK;

  TAO_CEC_Propagate_Event_Pull pull_worker (event);

  this->pull_collection_->for_each (&pull_worker,
                                    ACE_TRY_ENV);
  ACE_CHECK;
}

void
TAO_CEC_ConsumerAdmin::connected (TAO_CEC_ProxyPushSupplier * /*supplier*/,
                                  CORBA::Environment & /*ACE_TRY_ENV*/)
{
  // this->push_collection_->connected (supplier, ACE_TRY_ENV);
}

void
TAO_CEC_ConsumerAdmin::reconnected (TAO_CEC_ProxyPushSupplier *supplier,
                                    CORBA::Environment &ACE_TRY_ENV)
{
  this->push_collection_->reconnected (supplier, ACE_TRY_ENV);
}

void
TAO_CEC_ConsumerAdmin::disconnected (TAO_CEC_ProxyPushSupplier *supplier,
                                     CORBA::Environment &ACE_TRY_ENV)
{
  this->push_collection_->disconnected (supplier, ACE_TRY_ENV);
}

void
TAO_CEC_ConsumerAdmin::connected (TAO_CEC_ProxyPullSupplier * /*supplier*/,
                                  CORBA::Environment & /*ACE_TRY_ENV*/)
{
  // this->pull_collection_->connected (supplier, ACE_TRY_ENV);
}

void
TAO_CEC_ConsumerAdmin::reconnected (TAO_CEC_ProxyPullSupplier *supplier,
                                   CORBA::Environment &ACE_TRY_ENV)
{
  this->pull_collection_->reconnected (supplier, ACE_TRY_ENV);
}

void
TAO_CEC_ConsumerAdmin::disconnected (TAO_CEC_ProxyPullSupplier *supplier,
                                    CORBA::Environment &ACE_TRY_ENV)
{
  this->pull_collection_->disconnected (supplier, ACE_TRY_ENV);
}

void
TAO_CEC_ConsumerAdmin::shutdown (CORBA::Environment &ACE_TRY_ENV)
{
  TAO_CEC_Shutdown_Push_Supplier push_worker;

  this->push_collection_->for_each (&push_worker, ACE_TRY_ENV);
  ACE_CHECK;
  this->push_collection_->shutdown (ACE_TRY_ENV);
  ACE_CHECK;

  TAO_CEC_Shutdown_Pull_Supplier pull_worker;

  this->pull_collection_->for_each (&pull_worker, ACE_TRY_ENV);
  ACE_CHECK;
  this->pull_collection_->shutdown (ACE_TRY_ENV);
  ACE_CHECK;
}

CosEventChannelAdmin::ProxyPushSupplier_ptr
TAO_CEC_ConsumerAdmin::obtain_push_supplier (CORBA::Environment &ACE_TRY_ENV)
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  TAO_CEC_ProxyPushSupplier* supplier =
    this->event_channel_->create_proxy_push_supplier ();

  PortableServer::ServantBase_var holder = supplier;

  CosEventChannelAdmin::ProxyPushSupplier_var result =
    supplier->_this (ACE_TRY_ENV);
  ACE_CHECK_RETURN (CosEventChannelAdmin::ProxyPushSupplier::_nil ());

  this->push_collection_->connected (supplier, ACE_TRY_ENV);
  ACE_CHECK_RETURN (CosEventChannelAdmin::ProxyPushSupplier::_nil ());

  return result._retn ();
}

CosEventChannelAdmin::ProxyPullSupplier_ptr
TAO_CEC_ConsumerAdmin::obtain_pull_supplier (CORBA::Environment &ACE_TRY_ENV)
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  TAO_CEC_ProxyPullSupplier* supplier =
    this->event_channel_->create_proxy_pull_supplier ();

  PortableServer::ServantBase_var holder = supplier;

  CosEventChannelAdmin::ProxyPullSupplier_var result =
    supplier->_this (ACE_TRY_ENV);
  ACE_CHECK_RETURN (CosEventChannelAdmin::ProxyPullSupplier::_nil ());

  this->pull_collection_->connected (supplier, ACE_TRY_ENV);
  ACE_CHECK_RETURN (CosEventChannelAdmin::ProxyPullSupplier::_nil ());

  return result._retn ();
}

PortableServer::POA_ptr
TAO_CEC_ConsumerAdmin::_default_POA (CORBA::Environment&)
{
  return PortableServer::POA::_duplicate (this->default_POA_.in ());
}

// ****************************************************************

void
TAO_CEC_Shutdown_Push_Supplier::work (TAO_CEC_ProxyPushSupplier *supplier,
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

// ****************************************************************

void
TAO_CEC_Shutdown_Pull_Supplier::work (TAO_CEC_ProxyPullSupplier *supplier,
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

// ****************************************************************

void
TAO_CEC_Propagate_Event_Push::work (TAO_CEC_ProxyPushSupplier *supplier,
                                    CORBA::Environment &ACE_TRY_ENV)
{
  supplier->push (this->event_, ACE_TRY_ENV);
}

// ****************************************************************

void
TAO_CEC_Propagate_Event_Pull::work (TAO_CEC_ProxyPullSupplier *supplier,
                                    CORBA::Environment &ACE_TRY_ENV)
{
  supplier->push (this->event_, ACE_TRY_ENV);
}

// ****************************************************************

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)

template class TAO_EC_Worker<TAO_CEC_ProxyPushSupplier>;
template class TAO_EC_Worker<TAO_CEC_ProxyPullSupplier>;

#elif defined(ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)

#pragma instantiate TAO_EC_Worker<TAO_CEC_ProxyPushSupplier>
#pragma instantiate TAO_EC_Worker<TAO_CEC_ProxyPullSupplier>

#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */
