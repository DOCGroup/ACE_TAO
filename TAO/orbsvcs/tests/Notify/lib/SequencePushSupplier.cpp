// $Id$

#include "SequencePushSupplier.h"

#if ! defined (__ACE_INLINE__)
#include "SequencePushSupplier.inl"
#endif /* __ACE_INLINE__ */



TAO_Notify_Tests_SequencePushSupplier_Traits::TAO_Notify_Tests_SequencePushSupplier_Traits (void)
  :type_ (CosNotifyChannelAdmin::SEQUENCE_EVENT)
{
}

/*******************************************************************/

TAO_Notify_Tests_SequencePushSupplier::TAO_Notify_Tests_SequencePushSupplier (void)
{
}

TAO_Notify_Tests_SequencePushSupplier::~TAO_Notify_Tests_SequencePushSupplier ()
{
}

void
TAO_Notify_Tests_SequencePushSupplier::send_events (const CosNotification::EventBatch& events
                                          )
{
  ACE_ASSERT (!CORBA::is_nil (this->proxy_.in ()));

  this->proxy_->push_structured_events (events);
}

void
TAO_Notify_Tests_SequencePushSupplier::connect_to_peer (Proxy_Traits::PTR proxy_ptr, Peer_Traits::PTR peer_ptr)
{
  proxy_ptr->connect_sequence_push_supplier (peer_ptr);
}

void
TAO_Notify_Tests_SequencePushSupplier::disconnect_from_proxy (void)
{
  ACE_DEBUG((LM_DEBUG, "\nPush Supplier disconnecting from consumer.\n"));
  this->proxy_->disconnect_sequence_push_consumer ();
}

void
TAO_Notify_Tests_SequencePushSupplier::disconnect_sequence_push_supplier (void)
{
  bool this_method_used_only_by_notify_service = false;
  ACE_ASSERT(this_method_used_only_by_notify_service);
  ACE_UNUSED_ARG(this_method_used_only_by_notify_service);
}
