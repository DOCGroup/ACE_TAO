// $Id$

#include "EC_ConsumerAdmin.h"
#include "EC_ProxySupplier.h"
#include "EC_ProxyConsumer.h"
#include "EC_Event_Channel.h"
#include "orbsvcs/ESF/ESF_Peer_Workers.h"
#include "orbsvcs/ESF/ESF_Peer_Admin.h"
#include "orbsvcs/ESF/ESF_Shutdown_Proxy.h"

#if ! defined (__ACE_INLINE__)
#include "EC_ConsumerAdmin.i"
#endif /* __ACE_INLINE__ */

ACE_RCSID(Event, EC_ConsumerAdmin, "$Id$")

TAO_EC_ConsumerAdmin::TAO_EC_ConsumerAdmin (TAO_EC_Event_Channel *ec)
  :  TAO_ESF_Peer_Admin<TAO_EC_Event_Channel,TAO_EC_ProxyPushSupplier,RtecEventChannelAdmin::ProxyPushSupplier,TAO_EC_ProxyPushConsumer> (ec)
{
  this->default_POA_ =
    this->event_channel_->consumer_poa ();
}

TAO_EC_ConsumerAdmin::~TAO_EC_ConsumerAdmin (void)
{
}

RtecEventChannelAdmin::ProxyPushSupplier_ptr
TAO_EC_ConsumerAdmin::obtain_push_supplier (ACE_ENV_SINGLE_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  return this->obtain (ACE_ENV_SINGLE_ARG_PARAMETER);
}

PortableServer::POA_ptr
TAO_EC_ConsumerAdmin::_default_POA (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
{
  return PortableServer::POA::_duplicate (this->default_POA_.in ());
}

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)

template class TAO_ESF_Peer_Admin<TAO_EC_Event_Channel,TAO_EC_ProxyPushSupplier,RtecEventChannelAdmin::ProxyPushSupplier,TAO_EC_ProxyPushConsumer>;
template class TAO_ESF_Proxy_Admin<TAO_EC_Event_Channel,TAO_EC_ProxyPushSupplier,RtecEventChannelAdmin::ProxyPushSupplier>;
template class TAO_ESF_Worker<TAO_EC_ProxyPushSupplier>;
template class TAO_ESF_Shutdown_Proxy<TAO_EC_ProxyPushSupplier>;
template class TAO_ESF_Peer_Connected<TAO_EC_ProxyPushSupplier,TAO_EC_ProxyPushConsumer>;
template class TAO_ESF_Peer_Reconnected<TAO_EC_ProxyPushSupplier,TAO_EC_ProxyPushConsumer>;
template class TAO_ESF_Peer_Disconnected<TAO_EC_ProxyPushSupplier,TAO_EC_ProxyPushConsumer>;

#elif defined(ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)

#pragma instantiate TAO_ESF_Peer_Admin<TAO_EC_Event_Channel,TAO_EC_ProxyPushSupplier,RtecEventChannelAdmin::ProxyPushSupplier,TAO_EC_ProxyPushConsumer>
#pragma instantiate TAO_ESF_Proxy_Admin<TAO_EC_Event_Channel,TAO_EC_ProxyPushSupplier,RtecEventChannelAdmin::ProxyPushSupplier>
#pragma instantiate TAO_ESF_Worker<TAO_EC_ProxyPushSupplier>
#pragma instantiate TAO_ESF_Shutdown_Proxy<TAO_EC_ProxyPushSupplier>
#pragma instantiate TAO_ESF_Peer_Connected<TAO_EC_ProxyPushSupplier,TAO_EC_ProxyPushConsumer>
#pragma instantiate TAO_ESF_Peer_Reconnected<TAO_EC_ProxyPushSupplier,TAO_EC_ProxyPushConsumer>
#pragma instantiate TAO_ESF_Peer_Disconnected<TAO_EC_ProxyPushSupplier,TAO_EC_ProxyPushConsumer>

#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */
