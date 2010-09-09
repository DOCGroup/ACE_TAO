// $Id$

#include "orbsvcs/Event/EC_ConsumerAdmin.h"
#include "orbsvcs/Event/EC_ProxySupplier.h"
#include "orbsvcs/Event/EC_ProxyConsumer.h"
#include "orbsvcs/Event/EC_Event_Channel_Base.h"
#include "orbsvcs/ESF/ESF_Peer_Workers.h"
#include "orbsvcs/ESF/ESF_Peer_Admin.h"
#include "orbsvcs/ESF/ESF_Shutdown_Proxy.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

TAO_EC_ConsumerAdmin::TAO_EC_ConsumerAdmin (TAO_EC_Event_Channel_Base *ec)
  :  TAO_ESF_Peer_Admin<TAO_EC_Event_Channel_Base,TAO_EC_ProxyPushSupplier,RtecEventChannelAdmin::ProxyPushSupplier,TAO_EC_ProxyPushConsumer> (ec)
{
  this->default_POA_ =
    this->event_channel_->consumer_poa ();
}

TAO_EC_ConsumerAdmin::~TAO_EC_ConsumerAdmin (void)
{
}

RtecEventChannelAdmin::ProxyPushSupplier_ptr
TAO_EC_ConsumerAdmin::obtain_push_supplier (void)
{
  return this->obtain ();
}

PortableServer::POA_ptr
TAO_EC_ConsumerAdmin::_default_POA (void)
{
  return PortableServer::POA::_duplicate (this->default_POA_.in ());
}

TAO_END_VERSIONED_NAMESPACE_DECL
