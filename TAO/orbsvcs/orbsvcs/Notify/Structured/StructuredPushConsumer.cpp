// $Id$
#include "StructuredPushConsumer.h"
#include "ace/Refcounted_Auto_Ptr.h"
#include "../Event.h"

#if ! defined (__ACE_INLINE__)
#include "StructuredPushConsumer.inl"
#endif /* __ACE_INLINE__ */

ACE_RCSID(RT_Notify, TAO_NS_StructuredPushConsumer, "$Id$")

TAO_NS_StructuredPushConsumer::TAO_NS_StructuredPushConsumer (TAO_NS_ProxySupplier* proxy)
  :TAO_NS_Consumer (proxy)
{
}

TAO_NS_StructuredPushConsumer::~TAO_NS_StructuredPushConsumer ()
{
}

void
TAO_NS_StructuredPushConsumer::init (CosNotifyComm::StructuredPushConsumer_ptr push_consumer ACE_ENV_ARG_DECL_NOT_USED)
{
  this->push_consumer_ = CosNotifyComm::StructuredPushConsumer::_duplicate (push_consumer);

  this->publish_ = CosNotifyComm::NotifyPublish::_duplicate (push_consumer);

}

void
TAO_NS_StructuredPushConsumer::release (void)
{
  delete this;
  //@@ inform factory
}

void
TAO_NS_StructuredPushConsumer::push_i (const TAO_NS_Event_var& event ACE_ENV_ARG_DECL)
{
  event->push (this ACE_ENV_ARG_PARAMETER);
}

void
TAO_NS_StructuredPushConsumer::push (const CORBA::Any& event ACE_ENV_ARG_DECL)
{
  CosNotification::StructuredEvent notification;

  TAO_NS_Event::translate (event, notification);

  this->push_consumer_->push_structured_event (notification ACE_ENV_ARG_PARAMETER);
}

void
TAO_NS_StructuredPushConsumer::push (const CosNotification::StructuredEvent& event ACE_ENV_ARG_DECL)
{
  this->push_consumer_->push_structured_event (event ACE_ENV_ARG_PARAMETER);
}
