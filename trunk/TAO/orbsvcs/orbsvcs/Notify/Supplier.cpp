// $Id$

#include "Supplier.h"

#if ! defined (__ACE_INLINE__)
#include "Supplier.inl"
#endif /* __ACE_INLINE__ */

ACE_RCSID(RT_Notify, TAO_NS_Supplier, "$Id$")

#include "ProxyConsumer.h"
#include "Proxy.h"
#include "Admin.h"
#include "EventChannel.h"
#include "EventChannelFactory.h"
#include "Notify_Service.h"

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
TAO_NS_Supplier::dispatch_updates_i (const TAO_NS_EventTypeSeq & added, const TAO_NS_EventTypeSeq & removed
                                     ACE_ENV_ARG_DECL)
{
  CosNotification::EventTypeSeq cos_added;
  CosNotification::EventTypeSeq cos_removed;

  added.populate (cos_added);
  removed.populate (cos_removed);

  if (!CORBA::is_nil (this->subscribe_.in ()))
    this->subscribe_->subscription_change (cos_added, cos_removed ACE_ENV_ARG_PARAMETER);
}
