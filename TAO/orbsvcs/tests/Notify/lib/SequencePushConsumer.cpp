// $Id$

#include "SequencePushConsumer.h"

#if ! defined (__ACE_INLINE__)
#include "SequencePushConsumer.inl"
#endif /* __ACE_INLINE__ */

ACE_RCSID(lib, TAO_SequencePushConsumer, "$id$")

TAO_Notify_Tests_SequencePushConsumer_Traits::TAO_Notify_Tests_SequencePushConsumer_Traits (void)
  :type_ (CosNotifyChannelAdmin::SEQUENCE_EVENT)
{
}

/*******************************************************************/

TAO_Notify_Tests_SequencePushConsumer::TAO_Notify_Tests_SequencePushConsumer (void)
{
}

TAO_Notify_Tests_SequencePushConsumer::~TAO_Notify_Tests_SequencePushConsumer ()
{
}

void
TAO_Notify_Tests_SequencePushConsumer::connect_to_peer (Proxy_Traits::PTR proxy_ptr, Peer_Traits::PTR peer_ptr ACE_ENV_ARG_DECL)
{
  proxy_ptr->connect_sequence_push_consumer (peer_ptr
                                               ACE_ENV_ARG_PARAMETER);
}

void
TAO_Notify_Tests_SequencePushConsumer::disconnect_from_proxy (ACE_ENV_SINGLE_ARG_DECL)
{
  this->proxy_->disconnect_sequence_push_supplier (ACE_ENV_SINGLE_ARG_PARAMETER);
}

void
TAO_Notify_Tests_SequencePushConsumer::disconnect_sequence_push_consumer (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  this->deactivate (ACE_ENV_SINGLE_ARG_PARAMETER);
}

void
TAO_Notify_Tests_SequencePushConsumer::push_structured_events
   (const CosNotification::EventBatch &/*notifications*/
    ACE_ENV_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   CosEventComm::Disconnected))
{
  // NOP
}

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)

template class TAO_Notify_Tests_Peer_T<TAO_Notify_Tests_SequencePushConsumer_Traits>;
template class TAO_Notify_Tests_Consumer_T<TAO_Notify_Tests_SequencePushConsumer_Traits>;

#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)

#pragma instantiate TAO_Notify_Tests_Peer_T<TAO_Notify_Tests_SequencePushConsumer_Traits>
#pragma instantiate TAO_Notify_Tests_Consumer_T<TAO_Notify_Tests_SequencePushConsumer_Traits>

#endif /*ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */
