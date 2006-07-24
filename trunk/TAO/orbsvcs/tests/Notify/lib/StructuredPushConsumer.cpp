// $Id$

#include "StructuredPushConsumer.h"

#if ! defined (__ACE_INLINE__)
#include "StructuredPushConsumer.inl"
#endif /* __ACE_INLINE__ */

ACE_RCSID(lib, TAO_StructuredPushConsumer, "$Id$")

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
  ACE_DEBUG((LM_DEBUG, "\nPush Consumer disconnecting from supplier.\n"));
  this->proxy_->disconnect_structured_push_supplier (ACE_ENV_SINGLE_ARG_PARAMETER);
}

void
TAO_Notify_Tests_StructuredPushConsumer::disconnect_structured_push_consumer (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  bool this_method_used_only_by_notify_service = false;
  ACE_ASSERT(this_method_used_only_by_notify_service);
  ACE_UNUSED_ARG(this_method_used_only_by_notify_service);
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
