// $Id$

#include "StructuredProxyPushSupplier_Tie.h"

#if ! defined (__ACE_INLINE__)
#include "StructuredProxyPushSupplier_Tie.inl"
#endif /* __ACE_INLINE__ */

ACE_RCSID(RT_Notify, TAO_NS_StructuredProxyPushSupplier_Tie, "$Id$")

TAO_NS_StructuredProxyPushSupplier_Tie::TAO_NS_StructuredProxyPushSupplier_Tie (void)
  :inherited (tied_)
{
  this->tied_.servant (this);
}

TAO_NS_StructuredProxyPushSupplier_Tie::~TAO_NS_StructuredProxyPushSupplier_Tie ()
{
}

void
TAO_NS_StructuredProxyPushSupplier_Tie::_add_ref (ACE_ENV_SINGLE_ARG_DECL)
{
  this->tied_._incr_refcnt ();
}

void
TAO_NS_StructuredProxyPushSupplier_Tie::_remove_ref (ACE_ENV_SINGLE_ARG_DECL)
{
  this->tied_._decr_refcnt ();
}

void
TAO_NS_StructuredProxyPushSupplier_Tie::release (void)
{
  delete this;
  //@@ inform factory
}




