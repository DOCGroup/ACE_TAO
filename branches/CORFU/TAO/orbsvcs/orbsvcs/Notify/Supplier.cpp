// $Id$

#include "orbsvcs/Notify/Supplier.h"

#if ! defined (__ACE_INLINE__)
#include "orbsvcs/Notify/Supplier.inl"
#endif /* __ACE_INLINE__ */

ACE_RCSID(Notify, TAO_Notify_Supplier, "$Id$")

#include "orbsvcs/Notify/ProxyConsumer.h"
#include "orbsvcs/Notify/Proxy.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

TAO_Notify_Supplier::TAO_Notify_Supplier (TAO_Notify_ProxyConsumer* proxy)
  : proxy_ (proxy)
{
}

TAO_Notify_Supplier::~TAO_Notify_Supplier ()
{
}

TAO_Notify_Proxy*
TAO_Notify_Supplier::proxy (void)
{
  return this->proxy_consumer ();
}

void
TAO_Notify_Supplier::dispatch_updates_i (
  const CosNotification::EventTypeSeq& added,
  const CosNotification::EventTypeSeq& removed)
{
  if (!CORBA::is_nil (this->subscribe_.in ()))
    this->subscribe_->subscription_change (added, removed);
}

TAO_END_VERSIONED_NAMESPACE_DECL
