// $Id$

#include "CEC_ConsumerAdmin.h"
#include "CEC_ProxyPushSupplier.h"
#include "CEC_ProxyPullSupplier.h"
#include "CEC_ProxyPushConsumer.h"
#include "CEC_EventChannel.h"
#include "orbsvcs/ESF/ESF_Shutdown_Proxy.h"

#if ! defined (__ACE_INLINE__)
#include "CEC_ConsumerAdmin.i"
#endif /* __ACE_INLINE__ */

ACE_RCSID(CosEvent, CEC_ConsumerAdmin, "$Id$")

TAO_CEC_ConsumerAdmin::TAO_CEC_ConsumerAdmin (TAO_CEC_EventChannel *ec)
  :  event_channel_ (ec),
     push_admin_ (ec),
     pull_admin_ (ec)
{
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
}

void
TAO_CEC_ConsumerAdmin::push (const CORBA::Any &event
                             TAO_ENV_ARG_DECL)
{
  TAO_CEC_Propagate_Event_Push push_worker (event);

  this->push_admin_.for_each (&push_worker
                              TAO_ENV_ARG_PARAMETER);
  ACE_CHECK;

  TAO_CEC_Propagate_Event_Pull pull_worker (event);

  this->pull_admin_.for_each (&pull_worker
                              TAO_ENV_ARG_PARAMETER);
  ACE_CHECK;
}

void
TAO_CEC_ConsumerAdmin::connected (TAO_CEC_ProxyPushSupplier *supplier
                                  TAO_ENV_ARG_DECL)
{
  this->push_admin_.connected (supplier TAO_ENV_ARG_PARAMETER);
}

void
TAO_CEC_ConsumerAdmin::reconnected (TAO_CEC_ProxyPushSupplier *supplier
                                    TAO_ENV_ARG_DECL)
{
  this->push_admin_.reconnected (supplier TAO_ENV_ARG_PARAMETER);
}

void
TAO_CEC_ConsumerAdmin::disconnected (TAO_CEC_ProxyPushSupplier *supplier
                                     TAO_ENV_ARG_DECL)
{
  this->push_admin_.disconnected (supplier TAO_ENV_ARG_PARAMETER);
}

void
TAO_CEC_ConsumerAdmin::connected (TAO_CEC_ProxyPullSupplier *supplier
                                  TAO_ENV_ARG_DECL)
{
  this->pull_admin_.connected (supplier TAO_ENV_ARG_PARAMETER);
}

void
TAO_CEC_ConsumerAdmin::reconnected (TAO_CEC_ProxyPullSupplier *supplier
                                   TAO_ENV_ARG_DECL)
{
  this->pull_admin_.reconnected (supplier TAO_ENV_ARG_PARAMETER);
}

void
TAO_CEC_ConsumerAdmin::disconnected (TAO_CEC_ProxyPullSupplier *supplier
                                    TAO_ENV_ARG_DECL)
{
  this->pull_admin_.disconnected (supplier TAO_ENV_ARG_PARAMETER);
}

void
TAO_CEC_ConsumerAdmin::shutdown (TAO_ENV_SINGLE_ARG_DECL)
{
  this->push_admin_.shutdown (TAO_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;

  this->pull_admin_.shutdown (TAO_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;
}

CosEventChannelAdmin::ProxyPushSupplier_ptr
TAO_CEC_ConsumerAdmin::obtain_push_supplier (TAO_ENV_SINGLE_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  return this->push_admin_.obtain (TAO_ENV_SINGLE_ARG_PARAMETER);
}

CosEventChannelAdmin::ProxyPullSupplier_ptr
TAO_CEC_ConsumerAdmin::obtain_pull_supplier (TAO_ENV_SINGLE_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  return this->pull_admin_.obtain (TAO_ENV_SINGLE_ARG_PARAMETER);
}

PortableServer::POA_ptr
TAO_CEC_ConsumerAdmin::_default_POA (TAO_ENV_SINGLE_ARG_DECL_NOT_USED)
{
  return PortableServer::POA::_duplicate (this->default_POA_.in ());
}

// ****************************************************************

void
TAO_CEC_Propagate_Event_Push::work (TAO_CEC_ProxyPushSupplier *supplier
                                    TAO_ENV_ARG_DECL)
{
  supplier->push (this->event_ TAO_ENV_ARG_PARAMETER);
}

// ****************************************************************

void
TAO_CEC_Propagate_Event_Pull::work (TAO_CEC_ProxyPullSupplier *supplier
                                    TAO_ENV_ARG_DECL)
{
  supplier->push (this->event_ TAO_ENV_ARG_PARAMETER);
}

// ****************************************************************

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)

template class TAO_ESF_Proxy_Admin<TAO_CEC_EventChannel,TAO_CEC_ProxyPushSupplier,CosEventChannelAdmin::ProxyPushSupplier>;
template class TAO_ESF_Shutdown_Proxy<TAO_CEC_ProxyPushSupplier>;
template class TAO_ESF_Worker<TAO_CEC_ProxyPushSupplier>;

template class TAO_ESF_Proxy_Admin<TAO_CEC_EventChannel,TAO_CEC_ProxyPullSupplier,CosEventChannelAdmin::ProxyPullSupplier>;
template class TAO_ESF_Shutdown_Proxy<TAO_CEC_ProxyPullSupplier>;
template class TAO_ESF_Worker<TAO_CEC_ProxyPullSupplier>;

#elif defined(ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)

#pragma instantiate TAO_ESF_Proxy_Admin<TAO_CEC_EventChannel,TAO_CEC_ProxyPushSupplier,CosEventChannelAdmin::ProxyPushSupplier>
#pragma instantiate TAO_ESF_Shutdown_Proxy<TAO_CEC_ProxyPushSupplier>
#pragma instantiate TAO_ESF_Worker<TAO_CEC_ProxyPushSupplier>

#pragma instantiate TAO_ESF_Proxy_Admin<TAO_CEC_EventChannel,TAO_CEC_ProxyPullSupplier,CosEventChannelAdmin::ProxyPullSupplier>
#pragma instantiate TAO_ESF_Shutdown_Proxy<TAO_CEC_ProxyPullSupplier>
#pragma instantiate TAO_ESF_Worker<TAO_CEC_ProxyPullSupplier>

#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */
