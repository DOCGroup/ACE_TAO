// $Id$

#include "PushSupplier.h"

#if ! defined (__ACE_INLINE__)
#include "PushSupplier.inl"
#endif /* __ACE_INLINE__ */

ACE_RCSID(lib, TAO_PushSupplier, "$id$")

TAO_NS_PushSupplier_Traits::TAO_NS_PushSupplier_Traits (void)
  :type_ (CosNotifyChannelAdmin::ANY_EVENT)
{
}

/*******************************************************************/

TAO_NS_PushSupplier::TAO_NS_PushSupplier (void)
{
}

TAO_NS_PushSupplier::~TAO_NS_PushSupplier ()
{
}

void
TAO_NS_PushSupplier::send_event (const CORBA::Any& any ACE_ENV_ARG_DECL)
{
  ACE_ASSERT (!CORBA::is_nil (this->proxy_.in ()));

  this->proxy_->push (any ACE_ENV_ARG_PARAMETER);
}

void
TAO_NS_PushSupplier::connect_to_peer (Proxy_Traits::PTR proxy_ptr, Peer_Traits::PTR peer_ptr ACE_ENV_ARG_DECL)
{
  proxy_ptr->connect_any_push_supplier (peer_ptr
                                        ACE_ENV_ARG_PARAMETER);
}

void
TAO_NS_PushSupplier::disconnect_from_proxy (ACE_ENV_SINGLE_ARG_DECL)
{
  this->proxy_->disconnect_push_consumer (ACE_ENV_SINGLE_ARG_PARAMETER);
}

void
TAO_NS_PushSupplier::disconnect_push_supplier (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  this->deactivate (ACE_ENV_SINGLE_ARG_PARAMETER);
}

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)

template class TAO_NS_Peer_T<TAO_NS_PushSupplier_Traits>;
template class TAO_NS_Supplier_T<TAO_NS_PushSupplier_Traits>;

#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)

#pragma instantiate TAO_NS_Peer_T<TAO_NS_PushSupplier_Traits>
#pragma instantiate TAO_NS_Supplier_T<TAO_NS_PushSupplier_Traits>

#endif /*ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */
