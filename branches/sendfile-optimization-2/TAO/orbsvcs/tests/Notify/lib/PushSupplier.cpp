// $Id$

#include "PushSupplier.h"

#if ! defined (__ACE_INLINE__)
#include "PushSupplier.inl"
#endif /* __ACE_INLINE__ */

ACE_RCSID(lib, TAO_PushSupplier, "$Id$")

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
TAO_Notify_Tests_PushSupplier::send_event (const CORBA::Any& any ACE_ENV_ARG_DECL)
{
  ACE_ASSERT (!CORBA::is_nil (this->proxy_.in ()));

  this->proxy_->push (any ACE_ENV_ARG_PARAMETER);
}

void
TAO_Notify_Tests_PushSupplier::connect_to_peer (Proxy_Traits::PTR proxy_ptr, Peer_Traits::PTR peer_ptr ACE_ENV_ARG_DECL)
{
  proxy_ptr->connect_any_push_supplier (peer_ptr
                                        ACE_ENV_ARG_PARAMETER);
}

void
TAO_Notify_Tests_PushSupplier::disconnect_from_proxy (ACE_ENV_SINGLE_ARG_DECL)
{
  ACE_DEBUG((LM_DEBUG, "\nPush Supplier disconnecting from consumer.\n"));
  this->proxy_->disconnect_push_consumer (ACE_ENV_SINGLE_ARG_PARAMETER);
}

void
TAO_Notify_Tests_PushSupplier::disconnect_push_supplier (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  bool this_method_used_only_by_notify_service = false;
  ACE_ASSERT(this_method_used_only_by_notify_service);
  ACE_UNUSED_ARG(this_method_used_only_by_notify_service);
}
