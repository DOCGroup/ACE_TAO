// $Id$
#include "StructuredPushConsumer.h"
#include "ace/Refcounted_Auto_Ptr.h"
#include "../Event.h"

#if ! defined (__ACE_INLINE__)
#include "StructuredPushConsumer.inl"
#endif /* __ACE_INLINE__ */

ACE_RCSID(RT_Notify, TAO_NS_StructuredPushConsumer, "$Id$")

TAO_NS_StructuredPushConsumer::TAO_NS_StructuredPushConsumer (CosNotifyComm::StructuredPushConsumer_ptr push_consumer)
  :push_consumer_ (CosNotifyComm::StructuredPushConsumer::_duplicate (push_consumer))
{
}

TAO_NS_StructuredPushConsumer::~TAO_NS_StructuredPushConsumer ()
{
}

void
TAO_NS_StructuredPushConsumer::release (void)
{
  delete this;
  //@@ inform factory
}

void
TAO_NS_StructuredPushConsumer::push (const CosNotification::StructuredEvent & event ACE_ENV_ARG_DECL)
{
  this->push_consumer_->push_structured_event (event ACE_ENV_ARG_PARAMETER);
}
