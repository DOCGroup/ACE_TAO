// $Id$

#include "StructuredPushSupplier.h"

#if ! defined (__ACE_INLINE__)
#include "StructuredPushSupplier.inl"
#endif /* __ACE_INLINE__ */

ACE_RCSID(lib, TAO_StructuredPushSupplier, "$Id$")

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
    ACE_ENV_ARG_DECL
  )
{
  ACE_ASSERT (!CORBA::is_nil (this->proxy_.in ()));

  this->proxy_->push_structured_event (event ACE_ENV_ARG_PARAMETER);
}

void
TAO_Notify_Tests_StructuredPushSupplier::connect_to_peer (Proxy_Traits::PTR proxy_ptr, Peer_Traits::PTR peer_ptr ACE_ENV_ARG_DECL)
{
  proxy_ptr->connect_structured_push_supplier (peer_ptr
                                               ACE_ENV_ARG_PARAMETER);
}

void
TAO_Notify_Tests_StructuredPushSupplier::disconnect_from_proxy (ACE_ENV_SINGLE_ARG_DECL)
{
  ACE_DEBUG((LM_DEBUG, "\nPush Supplier disconnecting from consumer.\n"));
  this->proxy_->disconnect_structured_push_consumer (ACE_ENV_SINGLE_ARG_PARAMETER);
}

void
TAO_Notify_Tests_StructuredPushSupplier::disconnect_structured_push_supplier (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  bool this_method_used_only_by_notify_service = false;
  ACE_ASSERT(this_method_used_only_by_notify_service);
  ACE_UNUSED_ARG(this_method_used_only_by_notify_service);
}
