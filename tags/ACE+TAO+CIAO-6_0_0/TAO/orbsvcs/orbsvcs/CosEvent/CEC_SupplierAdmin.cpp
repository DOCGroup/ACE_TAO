// $Id$

#include "orbsvcs/CosEvent/CEC_SupplierAdmin.h"
#include "orbsvcs/CosEvent/CEC_ProxyPushConsumer.h"
#include "orbsvcs/CosEvent/CEC_ProxyPullConsumer.h"
#include "orbsvcs/CosEvent/CEC_EventChannel.h"
#include "orbsvcs/ESF/ESF_Shutdown_Proxy.h"

#if ! defined (__ACE_INLINE__)
#include "orbsvcs/CosEvent/CEC_SupplierAdmin.inl"
#endif /* __ACE_INLINE__ */

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

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
TAO_CEC_SupplierAdmin::_default_POA (void)
{
  return PortableServer::POA::_duplicate (this->default_POA_.in ());
}

void
TAO_CEC_SupplierAdmin::connected (TAO_CEC_ProxyPushConsumer *consumer)
{
  this->push_admin_.connected (consumer);
}

void
TAO_CEC_SupplierAdmin::reconnected (TAO_CEC_ProxyPushConsumer *consumer)
{
  this->push_admin_.reconnected (consumer);
}

void
TAO_CEC_SupplierAdmin::disconnected (TAO_CEC_ProxyPushConsumer *consumer)
{
  this->push_admin_.disconnected (consumer);
}

void
TAO_CEC_SupplierAdmin::connected (TAO_CEC_ProxyPullConsumer *consumer)
{
  this->pull_admin_.connected (consumer);
}

void
TAO_CEC_SupplierAdmin::reconnected (TAO_CEC_ProxyPullConsumer *consumer)
{
  this->pull_admin_.reconnected (consumer);
}

void
TAO_CEC_SupplierAdmin::disconnected (TAO_CEC_ProxyPullConsumer *consumer)
{
  this->pull_admin_.disconnected (consumer);
}

void
TAO_CEC_SupplierAdmin::shutdown (void)
{
  this->push_admin_.shutdown ();

  this->pull_admin_.shutdown ();
}

CosEventChannelAdmin::ProxyPushConsumer_ptr
TAO_CEC_SupplierAdmin::obtain_push_consumer (void)
{
  return this->push_admin_.obtain ();
}

CosEventChannelAdmin::ProxyPullConsumer_ptr
TAO_CEC_SupplierAdmin::obtain_pull_consumer (void)
{
  return this->pull_admin_.obtain ();
}

TAO_END_VERSIONED_NAMESPACE_DECL
