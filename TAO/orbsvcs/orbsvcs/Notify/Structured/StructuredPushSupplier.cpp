// $Id$

#include "StructuredPushSupplier.h"

#if ! defined (__ACE_INLINE__)
#include "StructuredPushSupplier.inl"
#endif /* __ACE_INLINE__ */

ACE_RCSID(RT_Notify, TAO_NS_StructuredPushSupplier, "$Id$")

TAO_NS_StructuredPushSupplier::TAO_NS_StructuredPushSupplier (CosNotifyComm::StructuredPushSupplier_ptr push_supplier)
  :push_supplier_ (CosNotifyComm::StructuredPushSupplier::_duplicate (push_supplier))
{
}

TAO_NS_StructuredPushSupplier::~TAO_NS_StructuredPushSupplier ()
{
}

void
TAO_NS_StructuredPushSupplier::release (void)
{
  delete this;
  //@@ inform factory
}

