// $Id$

#include "StructuredPushConsumer.h"

#if ! defined (__ACE_INLINE__)
#include "StructuredPushConsumer.inl"
#endif /* __ACE_INLINE__ */

ACE_RCSID(lib, TAO_StructuredPushConsumer, "$id$")

TAO_Notify_Tests_StructuredPushConsumer_Traits::TAO_Notify_Tests_StructuredPushConsumer_Traits (void)
  :type_ (CosNotifyChannelAdmin::STRUCTURED_EVENT)
{
}

/*******************************************************************/

TAO_Notify_Tests_StructuredPushConsumer::TAO_Notify_Tests_StructuredPushConsumer (void)
{
}

TAO_Notify_Tests_StructuredPushConsumer::~TAO_Notify_Tests_StructuredPushConsumer ()
{
}

void
TAO_Notify_Tests_StructuredPushConsumer::connect_to_peer (Proxy_Traits::PTR proxy_ptr, Peer_Traits::PTR peer_ptr ACE_ENV_ARG_DECL)
{
  proxy_ptr->connect_structured_push_consumer (peer_ptr
                                               ACE_ENV_ARG_PARAMETER);
}

void
TAO_Notify_Tests_StructuredPushConsumer::disconnect_from_proxy (ACE_ENV_SINGLE_ARG_DECL)
{
  this->proxy_->disconnect_structured_push_supplier (ACE_ENV_SINGLE_ARG_PARAMETER);
}

void
TAO_Notify_Tests_StructuredPushConsumer::disconnect_structured_push_consumer (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  this->deactivate (ACE_ENV_SINGLE_ARG_PARAMETER);
}

void
TAO_Notify_Tests_StructuredPushConsumer::push_structured_event
   (const CosNotification::StructuredEvent &/*notification*/
    ACE_ENV_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   CosEventComm::Disconnected))
{
  // NOP
}

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)

template class TAO_Notify_Tests_Peer_T<TAO_Notify_Tests_StructuredPushConsumer_Traits>;
template class TAO_Notify_Tests_Consumer_T<TAO_Notify_Tests_StructuredPushConsumer_Traits>;

#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)

#pragma instantiate TAO_Notify_Tests_Peer_T<TAO_Notify_Tests_StructuredPushConsumer_Traits>
#pragma instantiate TAO_Notify_Tests_Consumer_T<TAO_Notify_Tests_StructuredPushConsumer_Traits>

#endif /*ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */
