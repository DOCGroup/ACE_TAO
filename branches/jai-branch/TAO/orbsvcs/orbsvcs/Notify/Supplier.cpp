// $Id$

#include "Supplier.h"

#if ! defined (__ACE_INLINE__)
#include "Supplier.inl"
#endif /* __ACE_INLINE__ */

ACE_RCSID(Notify, TAO_NS_Supplier, "$Id$")

#include "ProxyConsumer.h"
#include "Proxy.h"

TAO_NS_Supplier::TAO_NS_Supplier (TAO_NS_ProxyConsumer* proxy)
  :proxy_ (proxy)
{
}

TAO_NS_Supplier::~TAO_NS_Supplier ()
{
}

TAO_NS_Proxy*
TAO_NS_Supplier::proxy (void)
{
  return this->proxy_consumer ();
}

void
TAO_NS_Supplier::dispatch_updates_i (const CosNotification::EventTypeSeq& added, const CosNotification::EventTypeSeq& removed
                                     ACE_ENV_ARG_DECL)
{
  if (!CORBA::is_nil (this->subscribe_.in ()))
    this->subscribe_->subscription_change (added, removed ACE_ENV_ARG_PARAMETER);
}
