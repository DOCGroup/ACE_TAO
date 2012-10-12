// $Id$

#include "orbsvcs/CosEvent/CEC_ConsumerAdmin.h"
#include "orbsvcs/CosEvent/CEC_ProxyPushSupplier.h"
#include "orbsvcs/CosEvent/CEC_ProxyPullSupplier.h"
#include "orbsvcs/CosEvent/CEC_ProxyPushConsumer.h"
#include "orbsvcs/CosEvent/CEC_EventChannel.h"

#include "orbsvcs/ESF/ESF_Shutdown_Proxy.h"

#if ! defined (__ACE_INLINE__)
#include "orbsvcs/CosEvent/CEC_ConsumerAdmin.inl"
#endif /* __ACE_INLINE__ */

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

TAO_CEC_ConsumerAdmin::TAO_CEC_ConsumerAdmin (TAO_CEC_EventChannel *ec)
  :  event_channel_ (ec),
     push_admin_ (ec),
     pull_admin_ (ec)
{
  this->default_POA_ =
    this->event_channel_->consumer_poa ();
}

TAO_CEC_ConsumerAdmin::~TAO_CEC_ConsumerAdmin (void)
{
}

void
TAO_CEC_ConsumerAdmin::push (const CORBA::Any &event)
{
  TAO_CEC_Propagate_Event_Push push_worker (event);

  this->push_admin_.for_each (&push_worker);

  TAO_CEC_Propagate_Event_Pull pull_worker (event);

  this->pull_admin_.for_each (&pull_worker);
}

void
TAO_CEC_ConsumerAdmin::connected (TAO_CEC_ProxyPushSupplier *supplier)
{
  this->push_admin_.connected (supplier);
}

void
TAO_CEC_ConsumerAdmin::reconnected (TAO_CEC_ProxyPushSupplier *supplier)
{
  this->push_admin_.reconnected (supplier);
}

void
TAO_CEC_ConsumerAdmin::disconnected (TAO_CEC_ProxyPushSupplier *supplier)
{
  this->push_admin_.disconnected (supplier);
}

void
TAO_CEC_ConsumerAdmin::connected (TAO_CEC_ProxyPullSupplier *supplier)
{
  this->pull_admin_.connected (supplier);
}

void
TAO_CEC_ConsumerAdmin::reconnected (TAO_CEC_ProxyPullSupplier *supplier)
{
  this->pull_admin_.reconnected (supplier);
}

void
TAO_CEC_ConsumerAdmin::disconnected (TAO_CEC_ProxyPullSupplier *supplier)
{
  this->pull_admin_.disconnected (supplier);
}

void
TAO_CEC_ConsumerAdmin::shutdown (void)
{
  this->push_admin_.shutdown ();

  this->pull_admin_.shutdown ();
}

CosEventChannelAdmin::ProxyPushSupplier_ptr
TAO_CEC_ConsumerAdmin::obtain_push_supplier (void)
{
  return this->push_admin_.obtain ();
}

CosEventChannelAdmin::ProxyPullSupplier_ptr
TAO_CEC_ConsumerAdmin::obtain_pull_supplier (void)
{
  return this->pull_admin_.obtain ();
}

PortableServer::POA_ptr
TAO_CEC_ConsumerAdmin::_default_POA (void)
{
  return PortableServer::POA::_duplicate (this->default_POA_.in ());
}

// ****************************************************************

void
TAO_CEC_Propagate_Event_Push::work (TAO_CEC_ProxyPushSupplier *supplier)
{
  supplier->push (this->event_);
}

// ****************************************************************

void
TAO_CEC_Propagate_Event_Pull::work (TAO_CEC_ProxyPullSupplier *supplier)
{
  supplier->push (this->event_);
}

TAO_END_VERSIONED_NAMESPACE_DECL
