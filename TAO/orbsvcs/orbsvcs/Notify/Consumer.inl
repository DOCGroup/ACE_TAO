// $Id$

#include "Consumer.h"

ACE_INLINE
TAO_NS_ProxySupplier*
TAO_NS_Consumer::proxy_supplier (void)
{
  return this->proxy_;
}

ACE_INLINE void
TAO_NS_Consumer::event_dispatch_observer (TAO_NS_Event_Dispatch_Observer* event_dispatch_observer)
{
  this->event_dispatch_observer_ = event_dispatch_observer;
}

