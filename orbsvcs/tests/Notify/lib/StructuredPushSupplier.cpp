// $Id$

#include "StructuredPushSupplier.h"

#if ! defined (__ACE_INLINE__)
#include "StructuredPushSupplier.inl"
#endif /* __ACE_INLINE__ */



TAO_Notify_Tests_StructuredPushSupplier_Traits::TAO_Notify_Tests_StructuredPushSupplier_Traits (void)
  :type_ (CosNotifyChannelAdmin::STRUCTURED_EVENT)
{
}

/*******************************************************************/

TAO_Notify_Tests_StructuredPushSupplier::TAO_Notify_Tests_StructuredPushSupplier (void)
{
}

TAO_Notify_Tests_StructuredPushSupplier::~TAO_Notify_Tests_StructuredPushSupplier ()
{
}

void
TAO_Notify_Tests_StructuredPushSupplier::send_event (
    const CosNotification::StructuredEvent& event
  )
{
  ACE_ASSERT (!CORBA::is_nil (this->proxy_.in ()));

  this->proxy_->push_structured_event (event);
}

void
TAO_Notify_Tests_StructuredPushSupplier::connect_to_peer (Proxy_Traits::PTR proxy_ptr, Peer_Traits::PTR peer_ptr)
{
  proxy_ptr->connect_structured_push_supplier (peer_ptr);
}

void
TAO_Notify_Tests_StructuredPushSupplier::disconnect_from_proxy (void)
{
  ACE_DEBUG((LM_DEBUG, "\nPush Supplier disconnecting from consumer.\n"));
  this->proxy_->disconnect_structured_push_consumer ();
}

void
TAO_Notify_Tests_StructuredPushSupplier::disconnect_structured_push_supplier (void)
{
  bool this_method_used_only_by_notify_service = false;
  ACE_ASSERT(this_method_used_only_by_notify_service);
  ACE_UNUSED_ARG(this_method_used_only_by_notify_service);
}
