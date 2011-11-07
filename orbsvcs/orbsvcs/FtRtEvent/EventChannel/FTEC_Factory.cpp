// $Id$

#include "orbsvcs/Event/EC_Event_Channel.h"
#include "orbsvcs/FtRtEvent/EventChannel/FTEC_Event_Channel.h"
#include "orbsvcs/FtRtEvent/EventChannel/FTEC_Factory.h"
#include "orbsvcs/FtRtEvent/EventChannel/FTEC_ProxySupplier.h"
#include "orbsvcs/FtRtEvent/EventChannel/FTEC_ProxyConsumer.h"
#include "orbsvcs/FtRtEvent/EventChannel/FTEC_SupplierAdmin.h"
#include "orbsvcs/FtRtEvent/EventChannel/FTEC_ConsumerAdmin.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

TAO_FTEC_Basic_Factory::TAO_FTEC_Basic_Factory (void)
{
}

TAO_EC_ConsumerAdmin*
TAO_FTEC_Basic_Factory::create_consumer_admin (TAO_EC_Event_Channel_Base* ec)
{
  return new TAO_FTEC_ConsumerAdmin(ec);
}

TAO_EC_SupplierAdmin*
TAO_FTEC_Basic_Factory::create_supplier_admin (TAO_EC_Event_Channel_Base* ec)
{
  return new TAO_FTEC_SupplierAdmin(ec);
}

TAO_EC_ProxyPushSupplier*
TAO_FTEC_Basic_Factory::create_proxy_push_supplier (TAO_EC_Event_Channel_Base* ec)
{
  return new TAO_FTEC_ProxyPushSupplier(ec, consumer_validate_connection_);
}

TAO_EC_ProxyPushConsumer*
TAO_FTEC_Basic_Factory::create_proxy_push_consumer (TAO_EC_Event_Channel_Base* ec)
{
  return new TAO_FTEC_ProxyPushConsumer(ec);
}

void
TAO_FTEC_Basic_Factory::destroy_proxy_push_supplier (TAO_EC_ProxyPushSupplier* proxy)
{
  delete proxy;
}

void
TAO_FTEC_Basic_Factory::destroy_proxy_push_consumer (TAO_EC_ProxyPushConsumer* proxy)
{
  delete proxy;
}

TAO_END_VERSIONED_NAMESPACE_DECL
