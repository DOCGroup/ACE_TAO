// $Id$

#include "EC_SupplierAdmin.h"
#include "EC_ProxyConsumer.h"
#include "EC_ProxySupplier.h"
#include "EC_Event_Channel.h"

#if ! defined (__ACE_INLINE__)
#include "EC_SupplierAdmin.i"
#endif /* __ACE_INLINE__ */

ACE_RCSID(Event, EC_SupplierAdmin, "$Id$")

TAO_EC_SupplierAdmin::TAO_EC_SupplierAdmin (TAO_EC_Event_Channel *ec)
  : TAO_ESF_Peer_Admin<TAO_EC_Event_Channel,TAO_EC_ProxyPushConsumer,TAO_EC_ProxyPushSupplier> (ec)
{
  this->default_POA_ =
    this->event_channel_->supplier_poa ();
}

TAO_EC_SupplierAdmin::~TAO_EC_SupplierAdmin (void)
{
}

RtecEventChannelAdmin::ProxyPushConsumer_ptr
TAO_EC_SupplierAdmin::obtain_push_consumer (CORBA::Environment &ACE_TRY_ENV)
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  return this->obtain (ACE_TRY_ENV);
}

PortableServer::POA_ptr
TAO_EC_SupplierAdmin::_default_POA (CORBA::Environment&)
{
  return PortableServer::POA::_duplicate (this->default_POA_.in ());
}

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)

template class TAO_ESF_Peer_Admin<TAO_EC_Event_Channel,TAO_EC_ProxyPushConsumer,TAO_EC_ProxyPushSupplier>;
template class TAO_ESF_Proxy_Admin<TAO_EC_Event_Channel,TAO_EC_ProxyPushConsumer>;
template class TAO_ESF_Worker<TAO_EC_ProxyPushConsumer>;
template class TAO_ESF_Shutdown_Proxy<TAO_EC_ProxyPushConsumer>;
template class TAO_ESF_Peer_Connected<TAO_EC_ProxyPushConsumer,TAO_EC_ProxyPushSupplier>;
template class TAO_ESF_Peer_Reconnected<TAO_EC_ProxyPushConsumer,TAO_EC_ProxyPushSupplier>;
template class TAO_ESF_Peer_Disconnected<TAO_EC_ProxyPushConsumer,TAO_EC_ProxyPushSupplier>;

#elif defined(ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)

#pragma instantiate TAO_ESF_Peer_Admin<TAO_EC_Event_Channel,TAO_EC_ProxyPushConsumer,TAO_EC_ProxyPushSupplier>
#pragma instantiate TAO_ESF_Proxy_Admin<TAO_EC_Event_Channel,TAO_EC_ProxyPushConsumer>
#pragma instantiate TAO_ESF_Worker<TAO_EC_ProxyPushConsumer>
#pragma instantiate TAO_ESF_Shutdown_Proxy<TAO_EC_ProxyPushConsumer>
#pragma instantiate TAO_ESF_Peer_Connected<TAO_EC_ProxyPushConsumer,TAO_EC_ProxyPushSupplier>
#pragma instantiate TAO_ESF_Peer_Reconnected<TAO_EC_ProxyPushConsumer,TAO_EC_ProxyPushSupplier>
#pragma instantiate TAO_ESF_Peer_Disconnected<TAO_EC_ProxyPushConsumer,TAO_EC_ProxyPushSupplier>

#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */
