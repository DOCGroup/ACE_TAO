// $Id$

#include "PushConsumer.h"

#if ! defined (__ACE_INLINE__)
#include "PushConsumer.inl"
#endif /* __ACE_INLINE__ */



TAO_Notify_Tests_PushConsumer_Traits::TAO_Notify_Tests_PushConsumer_Traits (void)
  :type_ (CosNotifyChannelAdmin::ANY_EVENT)
{
}

/*******************************************************************/

TAO_Notify_Tests_PushConsumer::TAO_Notify_Tests_PushConsumer (void)
{
}

TAO_Notify_Tests_PushConsumer::~TAO_Notify_Tests_PushConsumer ()
{
}

void
TAO_Notify_Tests_PushConsumer::connect_to_peer (Proxy_Traits::PTR proxy_ptr, Peer_Traits::PTR peer_ptr)
{
  proxy_ptr->connect_any_push_consumer (peer_ptr);
}

void
TAO_Notify_Tests_PushConsumer::disconnect_from_proxy (void)
{
  ACE_DEBUG((LM_DEBUG, "\nPush Consumer disconnecting from supplier.\n"));
  this->proxy_->disconnect_push_supplier ();
}

void
TAO_Notify_Tests_PushConsumer::disconnect_push_consumer (void)
{
  bool this_method_used_only_by_notify_service = false;
  ACE_ASSERT(this_method_used_only_by_notify_service);
  ACE_UNUSED_ARG(this_method_used_only_by_notify_service);
}

void
TAO_Notify_Tests_PushConsumer::push (const CORBA::Any & /*data*/)
{
  // NOP
}
