// $Id$

#include "SequencePushConsumer.h"

#if ! defined (__ACE_INLINE__)
#include "SequencePushConsumer.inl"
#endif /* __ACE_INLINE__ */

ACE_RCSID(lib, TAO_SequencePushConsumer, "$Id$")

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
  ACE_DEBUG((LM_DEBUG, "\nPush Consumer disconnecting from supplier.\n"));
  this->proxy_->disconnect_sequence_push_supplier (ACE_ENV_SINGLE_ARG_PARAMETER);
}

void
TAO_Notify_Tests_SequencePushConsumer::disconnect_sequence_push_consumer (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  bool this_method_used_only_by_notify_service = false;
  ACE_ASSERT(this_method_used_only_by_notify_service);
  ACE_UNUSED_ARG(this_method_used_only_by_notify_service);
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
