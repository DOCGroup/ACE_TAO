// $Id$

#include "SequencePushSupplier.h"

#if ! defined (__ACE_INLINE__)
#include "SequencePushSupplier.inl"
#endif /* __ACE_INLINE__ */

ACE_RCSID(Notify, TAO_Notify_SequencePushSupplier, "$id$")

TAO_Notify_SequencePushSupplier::TAO_Notify_SequencePushSupplier (TAO_Notify_ProxyConsumer* proxy)
  :TAO_Notify_Supplier (proxy)
{
}

TAO_Notify_SequencePushSupplier::~TAO_Notify_SequencePushSupplier ()
{
}

void
TAO_Notify_SequencePushSupplier::init (CosNotifyComm::SequencePushSupplier_ptr push_supplier ACE_ENV_ARG_DECL_NOT_USED)
{
  this->push_supplier_ = CosNotifyComm::SequencePushSupplier::_duplicate (push_supplier);

  this->subscribe_ = CosNotifyComm::NotifySubscribe::_duplicate (push_supplier);
}

void
TAO_Notify_SequencePushSupplier::release (void)
{
  delete this;
  //@@ inform factory
}
