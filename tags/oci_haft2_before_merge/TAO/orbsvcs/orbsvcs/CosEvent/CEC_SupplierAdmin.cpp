// $Id$

#include "CEC_SupplierAdmin.h"
#include "CEC_ProxyPushConsumer.h"
#include "CEC_ProxyPullConsumer.h"
#include "CEC_EventChannel.h"
#include "orbsvcs/ESF/ESF_Shutdown_Proxy.h"

#if ! defined (__ACE_INLINE__)
#include "CEC_SupplierAdmin.i"
#endif /* __ACE_INLINE__ */

ACE_RCSID(CosEvent, CEC_SupplierAdmin, "$Id$")

TAO_CEC_SupplierAdmin::TAO_CEC_SupplierAdmin (TAO_CEC_EventChannel *ec)
  :  event_channel_ (ec),
     push_admin_ (ec),
     pull_admin_ (ec)
{
  this->default_POA_ =
    this->event_channel_->supplier_poa ();
}

TAO_CEC_SupplierAdmin::~TAO_CEC_SupplierAdmin (void)
{
}

PortableServer::POA_ptr
TAO_CEC_SupplierAdmin::_default_POA (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
{
  return PortableServer::POA::_duplicate (this->default_POA_.in ());
}

void
TAO_CEC_SupplierAdmin::connected (TAO_CEC_ProxyPushConsumer *consumer
                                  ACE_ENV_ARG_DECL)
{
  this->push_admin_.connected (consumer ACE_ENV_ARG_PARAMETER);
}

void
TAO_CEC_SupplierAdmin::reconnected (TAO_CEC_ProxyPushConsumer *consumer
                                    ACE_ENV_ARG_DECL)
{
  this->push_admin_.reconnected (consumer ACE_ENV_ARG_PARAMETER);
}

void
TAO_CEC_SupplierAdmin::disconnected (TAO_CEC_ProxyPushConsumer *consumer
                                     ACE_ENV_ARG_DECL)
{
  this->push_admin_.disconnected (consumer ACE_ENV_ARG_PARAMETER);
}

void
TAO_CEC_SupplierAdmin::connected (TAO_CEC_ProxyPullConsumer *consumer
                                  ACE_ENV_ARG_DECL)
{
  this->pull_admin_.connected (consumer ACE_ENV_ARG_PARAMETER);
}

void
TAO_CEC_SupplierAdmin::reconnected (TAO_CEC_ProxyPullConsumer *consumer
                                    ACE_ENV_ARG_DECL)
{
  this->pull_admin_.reconnected (consumer ACE_ENV_ARG_PARAMETER);
}

void
TAO_CEC_SupplierAdmin::disconnected (TAO_CEC_ProxyPullConsumer *consumer
                                     ACE_ENV_ARG_DECL)
{
  this->pull_admin_.disconnected (consumer ACE_ENV_ARG_PARAMETER);
}

void
TAO_CEC_SupplierAdmin::shutdown (ACE_ENV_SINGLE_ARG_DECL)
{
  this->push_admin_.shutdown (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;

  this->pull_admin_.shutdown (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;
}

CosEventChannelAdmin::ProxyPushConsumer_ptr
TAO_CEC_SupplierAdmin::obtain_push_consumer (ACE_ENV_SINGLE_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  return this->push_admin_.obtain (ACE_ENV_SINGLE_ARG_PARAMETER);
}

CosEventChannelAdmin::ProxyPullConsumer_ptr
TAO_CEC_SupplierAdmin::obtain_pull_consumer (ACE_ENV_SINGLE_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  return this->pull_admin_.obtain (ACE_ENV_SINGLE_ARG_PARAMETER);
}

// ****************************************************************

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)

template class TAO_ESF_Proxy_Admin<TAO_CEC_EventChannel,TAO_CEC_ProxyPushConsumer,CosEventChannelAdmin::ProxyPushConsumer>;
template class TAO_ESF_Shutdown_Proxy<TAO_CEC_ProxyPushConsumer>;
template class TAO_ESF_Worker<TAO_CEC_ProxyPushConsumer>;

template class TAO_ESF_Proxy_Admin<TAO_CEC_EventChannel,TAO_CEC_ProxyPullConsumer,CosEventChannelAdmin::ProxyPullConsumer>;
template class TAO_ESF_Shutdown_Proxy<TAO_CEC_ProxyPullConsumer>;
template class TAO_ESF_Worker<TAO_CEC_ProxyPullConsumer>;

#elif defined(ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)

#pragma instantiate TAO_ESF_Proxy_Admin<TAO_CEC_EventChannel,TAO_CEC_ProxyPushConsumer,CosEventChannelAdmin::ProxyPushConsumer>
#pragma instantiate TAO_ESF_Shutdown_Proxy<TAO_CEC_ProxyPushConsumer>
#pragma instantiate TAO_ESF_Worker<TAO_CEC_ProxyPushConsumer>

#pragma instantiate TAO_ESF_Proxy_Admin<TAO_CEC_EventChannel,TAO_CEC_ProxyPullConsumer,CosEventChannelAdmin::ProxyPullConsumer>
#pragma instantiate TAO_ESF_Shutdown_Proxy<TAO_CEC_ProxyPullConsumer>
#pragma instantiate TAO_ESF_Worker<TAO_CEC_ProxyPullConsumer>

#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */
