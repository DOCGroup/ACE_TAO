// $Id$

#include "PushSupplier.h"

#if ! defined (__ACE_INLINE__)
#include "PushSupplier.inl"
#endif /* __ACE_INLINE__ */

ACE_RCSID(Notify, TAO_NS_PushSupplier, "$id$")

TAO_NS_PushSupplier::TAO_NS_PushSupplier (TAO_NS_ProxyConsumer* proxy)
  :TAO_NS_Supplier (proxy)
{
}

TAO_NS_PushSupplier::~TAO_NS_PushSupplier ()
{
}

void
TAO_NS_PushSupplier::init (CosEventComm::PushSupplier_ptr push_supplier ACE_ENV_ARG_DECL)
{
  this->push_supplier_ = CosEventComm::PushSupplier::_duplicate (push_supplier);

  this->subscribe_ = CosNotifyComm::NotifySubscribe::_narrow (push_supplier ACE_ENV_ARG_PARAMETER);
}

void
TAO_NS_PushSupplier::release (void)
{
  delete this;
  //@@ inform factory
}
