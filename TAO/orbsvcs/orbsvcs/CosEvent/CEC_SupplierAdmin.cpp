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
    this->event_channel_->supplier_poa ();
}

TAO_CEC_SupplierAdmin::~TAO_CEC_SupplierAdmin (void)
{
}

PortableServer::POA_ptr
TAO_CEC_SupplierAdmin::_default_POA (CORBA::Environment&)
{
  return PortableServer::POA::_duplicate (this->default_POA_.in ());
}

void
TAO_CEC_SupplierAdmin::connected (TAO_CEC_ProxyPushConsumer *consumer,
                                  CORBA::Environment & ACE_TRY_ENV)
{
  this->push_admin_.connected (consumer, ACE_TRY_ENV);
}

void
TAO_CEC_SupplierAdmin::reconnected (TAO_CEC_ProxyPushConsumer *consumer,
                                    CORBA::Environment &ACE_TRY_ENV)
{
  this->push_admin_.reconnected (consumer, ACE_TRY_ENV);
}

void
TAO_CEC_SupplierAdmin::disconnected (TAO_CEC_ProxyPushConsumer *consumer,
                                     CORBA::Environment &ACE_TRY_ENV)
{
  this->push_admin_.disconnected (consumer, ACE_TRY_ENV);
}

void
TAO_CEC_SupplierAdmin::connected (TAO_CEC_ProxyPullConsumer *consumer,
                                  CORBA::Environment &ACE_TRY_ENV)
{
  this->pull_admin_.connected (consumer, ACE_TRY_ENV);
}

void
TAO_CEC_SupplierAdmin::reconnected (TAO_CEC_ProxyPullConsumer *consumer,
                                    CORBA::Environment &ACE_TRY_ENV)
{
  this->pull_admin_.reconnected (consumer, ACE_TRY_ENV);
}

void
TAO_CEC_SupplierAdmin::disconnected (TAO_CEC_ProxyPullConsumer *consumer,
                                     CORBA::Environment &ACE_TRY_ENV)
{
  this->pull_admin_.disconnected (consumer, ACE_TRY_ENV);
}

void
TAO_CEC_SupplierAdmin::shutdown (CORBA::Environment &ACE_TRY_ENV)
{
  this->push_admin_.shutdown (ACE_TRY_ENV);
  ACE_CHECK;

  this->pull_admin_.shutdown (ACE_TRY_ENV);
  ACE_CHECK;
}

CosEventChannelAdmin::ProxyPushConsumer_ptr
TAO_CEC_SupplierAdmin::obtain_push_consumer (CORBA::Environment &ACE_TRY_ENV)
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  return this->push_admin_.obtain (ACE_TRY_ENV);
}

CosEventChannelAdmin::ProxyPullConsumer_ptr
TAO_CEC_SupplierAdmin::obtain_pull_consumer (CORBA::Environment &ACE_TRY_ENV)
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  return this->pull_admin_.obtain (ACE_TRY_ENV);
}

// ****************************************************************

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)

template class TAO_ESF_Proxy_Admin<TAO_CEC_EventChannel,TAO_CEC_ProxyPushConsumer>;
template class TAO_ESF_Shutdown_Proxy<TAO_CEC_ProxyPushConsumer>;
template class TAO_ESF_Worker<TAO_CEC_ProxyPushConsumer>;

template class TAO_ESF_Proxy_Admin<TAO_CEC_EventChannel,TAO_CEC_ProxyPullConsumer>;
template class TAO_ESF_Shutdown_Proxy<TAO_CEC_ProxyPullConsumer>;
template class TAO_ESF_Worker<TAO_CEC_ProxyPullConsumer>;

#elif defined(ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)

#pragma instantiate TAO_ESF_Proxy_Admin<TAO_CEC_EventChannel,TAO_CEC_ProxyPushConsumer>
#pragma instantiate TAO_ESF_Shutdown_Proxy<TAO_CEC_ProxyPushConsumer>
#pragma instantiate TAO_ESF_Worker<TAO_CEC_ProxyPushConsumer>

#pragma instantiate TAO_ESF_Proxy_Admin<TAO_CEC_EventChannel,TAO_CEC_ProxyPullConsumer>
#pragma instantiate TAO_ESF_Shutdown_Proxy<TAO_CEC_ProxyPullConsumer>
#pragma instantiate TAO_ESF_Worker<TAO_CEC_ProxyPullConsumer>

#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */
