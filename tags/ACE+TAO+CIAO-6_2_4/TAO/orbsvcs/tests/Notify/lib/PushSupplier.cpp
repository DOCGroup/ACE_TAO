// $Id$

#include "PushSupplier.h"

#if ! defined (__ACE_INLINE__)
#include "PushSupplier.inl"
#endif /* __ACE_INLINE__ */



TAO_Notify_Tests_PushSupplier_Traits::TAO_Notify_Tests_PushSupplier_Traits (void)
  :type_ (CosNotifyChannelAdmin::ANY_EVENT)
{
}

/*******************************************************************/

TAO_Notify_Tests_PushSupplier::TAO_Notify_Tests_PushSupplier (void)
{
}

TAO_Notify_Tests_PushSupplier::~TAO_Notify_Tests_PushSupplier ()
{
}

void
TAO_Notify_Tests_PushSupplier::send_event (const CORBA::Any& any)
{
  ACE_ASSERT (!CORBA::is_nil (this->proxy_.in ()));

  this->proxy_->push (any);
}

void
TAO_Notify_Tests_PushSupplier::connect_to_peer (Proxy_Traits::PTR proxy_ptr, Peer_Traits::PTR peer_ptr)
{
  proxy_ptr->connect_any_push_supplier (peer_ptr);
}

void
TAO_Notify_Tests_PushSupplier::disconnect_from_proxy (void)
{
  ACE_DEBUG((LM_DEBUG, "\nPush Supplier disconnecting from consumer.\n"));
  this->proxy_->disconnect_push_consumer ();
}

void
TAO_Notify_Tests_PushSupplier::disconnect_push_supplier (void)
{
  bool this_method_used_only_by_notify_service = false;
  ACE_ASSERT(this_method_used_only_by_notify_service);
  ACE_UNUSED_ARG(this_method_used_only_by_notify_service);
}
