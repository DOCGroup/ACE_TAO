// $Id$

#include "StructuredPushSupplier.h"

#if ! defined (__ACE_INLINE__)
#include "StructuredPushSupplier.inl"
#endif /* __ACE_INLINE__ */

ACE_RCSID(RT_Notify, TAO_NS_StructuredPushSupplier, "$Id$")

TAO_NS_StructuredPushSupplier::TAO_NS_StructuredPushSupplier (TAO_NS_ProxyConsumer* proxy)
  :TAO_NS_Supplier (proxy)
{
}

TAO_NS_StructuredPushSupplier::~TAO_NS_StructuredPushSupplier ()
{
}

void
TAO_NS_StructuredPushSupplier::init (CosNotifyComm::StructuredPushSupplier_ptr push_supplier ACE_ENV_ARG_DECL_NOT_USED)
{
  this->push_supplier_ = CosNotifyComm::StructuredPushSupplier::_duplicate (push_supplier);

  this->subscribe_ = CosNotifyComm::NotifySubscribe::_duplicate (push_supplier);
}

void
TAO_NS_StructuredPushSupplier::release (void)
{
  delete this;
  //@@ inform factory
}
