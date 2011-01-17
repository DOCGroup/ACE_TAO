// $Id$

#include "orbsvcs/Event/EC_SupplierAdmin.h"
#include "orbsvcs/Event/EC_ProxyConsumer.h"
#include "orbsvcs/Event/EC_ProxySupplier.h"
#include "orbsvcs/Event/EC_Event_Channel_Base.h"
#include "orbsvcs/ESF/ESF_Peer_Workers.h"
#include "orbsvcs/ESF/ESF_Peer_Admin.h"
#include "orbsvcs/ESF/ESF_Shutdown_Proxy.h"



TAO_BEGIN_VERSIONED_NAMESPACE_DECL

TAO_EC_SupplierAdmin::TAO_EC_SupplierAdmin (TAO_EC_Event_Channel_Base *ec)
  : TAO_ESF_Peer_Admin<TAO_EC_Event_Channel_Base,TAO_EC_ProxyPushConsumer,RtecEventChannelAdmin::ProxyPushConsumer,TAO_EC_ProxyPushSupplier> (ec)
{
  this->default_POA_ =
    this->event_channel_->supplier_poa ();
}

TAO_EC_SupplierAdmin::~TAO_EC_SupplierAdmin (void)
{
}

RtecEventChannelAdmin::ProxyPushConsumer_ptr
TAO_EC_SupplierAdmin::obtain_push_consumer (void)
{
  return this->obtain ();
}

PortableServer::POA_ptr
TAO_EC_SupplierAdmin::_default_POA (void)
{
  return PortableServer::POA::_duplicate (this->default_POA_.in ());
}

TAO_END_VERSIONED_NAMESPACE_DECL
