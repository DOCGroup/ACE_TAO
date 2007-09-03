// $Id$
#include "orbsvcs/Notify/MonitorControlExt/MonitorSupplierAdmin.h"
#include "orbsvcs/Notify/MonitorControlExt/MonitorEventChannel.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

TAO_MonitorSupplierAdmin::TAO_MonitorSupplierAdmin (void)
{
}

TAO_MonitorSupplierAdmin::~TAO_MonitorSupplierAdmin (void)
{
}

CosNotifyChannelAdmin::ProxyConsumer_ptr
TAO_MonitorSupplierAdmin::obtain_named_notification_push_consumer (
        CosNotifyChannelAdmin::ClientType ctype,
        CosNotifyChannelAdmin::ProxyID_out proxy_id,
        const char * name)
{
  // First, make sure we can get down to the real ec type
  TAO_MonitorEventChannel* ec =
    dynamic_cast<TAO_MonitorEventChannel*> (this->ec_.get ());
  if (ec == 0)
    throw CORBA::INTERNAL ();

  // Next, create the push consumer proxy
  CosNotifyChannelAdmin::ProxyConsumer_var proxy =
    this->TAO_Notify_SupplierAdmin::obtain_notification_push_consumer (
      ctype, proxy_id);

  // Next, map the proxy id to the name in the ec
  ec->map_consumer_proxy (proxy_id, name);

  return proxy._retn ();
}

CosNotifyChannelAdmin::ProxyConsumer_ptr
TAO_MonitorSupplierAdmin::obtain_notification_push_consumer (
        CosNotifyChannelAdmin::ClientType ctype,
        CosNotifyChannelAdmin::ProxyID_out proxy_id)
{
  // First, make sure we can get down to the real ec type
  TAO_MonitorEventChannel* ec =
    dynamic_cast<TAO_MonitorEventChannel*> (this->ec_.get ());
  if (ec == 0)
    throw CORBA::INTERNAL ();

  // Next, create the push consumer proxy
  CosNotifyChannelAdmin::ProxyConsumer_var proxy =
    this->TAO_Notify_SupplierAdmin::obtain_notification_push_consumer (
      ctype, proxy_id);

  // Next, map the proxy id to the name in the ec
  char name[64];
  ACE_OS::sprintf(name, "%d", proxy_id);
  ec->map_consumer_proxy (proxy_id, name);

  return proxy._retn ();
}

TAO_END_VERSIONED_NAMESPACE_DECL
