// $Id$
#include "StructuredPushConsumer.h"

#if ! defined (__ACE_INLINE__)
#include "StructuredPushConsumer.inl"
#endif /* __ACE_INLINE__ */

ACE_RCSID(RT_Notify, TAO_Notify_StructuredPushConsumer, "$Id$")

#include "../Properties.h"
#include "../Event.h"
#include "ace/Refcounted_Auto_Ptr.h"

TAO_Notify_StructuredPushConsumer::TAO_Notify_StructuredPushConsumer (TAO_Notify_ProxySupplier* proxy)
  :TAO_Notify_Consumer (proxy)
{
}

TAO_Notify_StructuredPushConsumer::~TAO_Notify_StructuredPushConsumer ()
{
}

void
TAO_Notify_StructuredPushConsumer::init (CosNotifyComm::StructuredPushConsumer_ptr push_consumer ACE_ENV_ARG_DECL_NOT_USED)
{
  this->push_consumer_ = CosNotifyComm::StructuredPushConsumer::_duplicate (push_consumer);

  this->publish_ = CosNotifyComm::NotifyPublish::_duplicate (push_consumer);

}

void
TAO_Notify_StructuredPushConsumer::release (void)
{
  delete this;
  //@@ inform factory
}

void
TAO_Notify_StructuredPushConsumer::push_i (const TAO_Notify_Event* event ACE_ENV_ARG_DECL)
{
  event->push (this ACE_ENV_ARG_PARAMETER);
}

void
TAO_Notify_StructuredPushConsumer::push_i (const TAO_Notify_Event_var& event ACE_ENV_ARG_DECL)
{
  event->push (this ACE_ENV_ARG_PARAMETER);
}

void
TAO_Notify_StructuredPushConsumer::push (const CORBA::Any& event ACE_ENV_ARG_DECL)
{
  CosNotification::StructuredEvent notification;

  TAO_Notify_Event::translate (event, notification);

  this->push_consumer_->push_structured_event (notification ACE_ENV_ARG_PARAMETER);
}

void
TAO_Notify_StructuredPushConsumer::push (const CosNotification::StructuredEvent& event ACE_ENV_ARG_DECL)
{
  this->push_consumer_->push_structured_event (event ACE_ENV_ARG_PARAMETER);
}
bool
TAO_Notify_StructuredPushConsumer::get_ior (ACE_CString & iorstr) const
{
  bool result = false;
  CORBA::ORB_var orb = TAO_Notify_PROPERTIES::instance()->orb();
  ACE_DECLARE_NEW_CORBA_ENV;
  ACE_TRY
  {
    CORBA::String_var ior = orb->object_to_string(this->push_consumer_.in() ACE_ENV_ARG_PARAMETER);
    ACE_TRY_CHECK;
    iorstr = ACE_static_cast (const char *, ior.in ());
    result = true;
  }
  ACE_CATCHANY
  {
    ACE_ASSERT(0);
  }
  ACE_ENDTRY;
  return result;
}
