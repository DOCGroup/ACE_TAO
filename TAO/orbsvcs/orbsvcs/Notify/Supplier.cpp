// $Id$

#include "Supplier.h"

#if ! defined (__ACE_INLINE__)
#include "Supplier.inl"
#endif /* __ACE_INLINE__ */

ACE_RCSID(RT_Notify, TAO_NS_Supplier, "$Id$")

#include "ProxyConsumer.h"

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
