// $Id$

#include "PushConsumer.h"

#if ! defined (__ACE_INLINE__)
#include "PushConsumer.inl"
#endif /* __ACE_INLINE__ */

ACE_RCSID(lib, TAO_PushConsumer, "$id$")

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
TAO_Notify_Tests_PushConsumer::connect_to_peer (Proxy_Traits::PTR proxy_ptr, Peer_Traits::PTR peer_ptr ACE_ENV_ARG_DECL)
{
  proxy_ptr->connect_any_push_consumer (peer_ptr ACE_ENV_ARG_PARAMETER);
}

void
TAO_Notify_Tests_PushConsumer::disconnect_from_proxy (ACE_ENV_SINGLE_ARG_DECL)
{
  this->proxy_->disconnect_push_supplier (ACE_ENV_SINGLE_ARG_PARAMETER);
}

void
TAO_Notify_Tests_PushConsumer::disconnect_push_consumer (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  this->deactivate (ACE_ENV_SINGLE_ARG_PARAMETER);
}

void
TAO_Notify_Tests_PushConsumer::push (const CORBA::Any & /*data*/ ACE_ENV_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((
                   CORBA::SystemException,
                   CosEventComm::Disconnected
                   ))
{
  // NOP
}

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)

template class TAO_Notify_Tests_Peer_T<TAO_Notify_Tests_PushConsumer_Traits>;
template class TAO_Notify_Tests_Consumer_T<TAO_Notify_Tests_PushConsumer_Traits>;

#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)

#pragma instantiate TAO_Notify_Tests_Peer_T<TAO_Notify_Tests_PushConsumer_Traits>
#pragma instantiate TAO_Notify_Tests_Consumer_T<TAO_Notify_Tests_PushConsumer_Traits>

#endif /*ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */
