// $Id$
#include "orbsvcs/Notify/Structured/StructuredPushConsumer.h"

ACE_RCSID(RT_Notify, TAO_Notify_StructuredPushConsumer, "$Id$")

#include "orbsvcs/Notify/Properties.h"
#include "orbsvcs/Notify/Event.h"
#include "ace/Bound_Ptr.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

TAO_Notify_StructuredPushConsumer::TAO_Notify_StructuredPushConsumer (TAO_Notify_ProxySupplier* proxy)
  :TAO_Notify_Consumer (proxy)
{
}

TAO_Notify_StructuredPushConsumer::~TAO_Notify_StructuredPushConsumer ()
{
}

void
TAO_Notify_StructuredPushConsumer::init (CosNotifyComm::StructuredPushConsumer_ptr push_consumer ACE_ENV_ARG_DECL)
{
  // Initialize only once
  ACE_ASSERT( CORBA::is_nil (this->push_consumer_.in()) );

  if (CORBA::is_nil (push_consumer))
  {
    ACE_THROW (CORBA::BAD_PARAM());
  }

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

/// Push a batch of events to this consumer.
void
TAO_Notify_StructuredPushConsumer::push (const CosNotification::EventBatch& event ACE_ENV_ARG_DECL_NOT_USED)
{
  ACE_ASSERT(false);
  ACE_UNUSED_ARG (event);
  // TODO exception?
}

void
TAO_Notify_StructuredPushConsumer::reconnect_from_consumer (TAO_Notify_Consumer* old_consumer
    ACE_ENV_ARG_DECL)
{
  TAO_Notify_StructuredPushConsumer* tmp = dynamic_cast<TAO_Notify_StructuredPushConsumer *> (old_consumer);
  ACE_ASSERT(tmp != 0);
  this->init(tmp->push_consumer_.in() ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;
  this->schedule_timer(false);
}

ACE_CString
TAO_Notify_StructuredPushConsumer::get_ior (void) const
{
  ACE_CString result;
  CORBA::ORB_var orb = TAO_Notify_PROPERTIES::instance()->orb();
  ACE_DECLARE_NEW_CORBA_ENV;
  ACE_TRY
  {
    CORBA::String_var ior = orb->object_to_string(this->push_consumer_.in() ACE_ENV_ARG_PARAMETER);
    ACE_TRY_CHECK;
    result = static_cast<const char*> (ior.in ());
  }
  ACE_CATCHANY
  {
    result.fast_clear();
  }
  ACE_ENDTRY;
  return result;
}

TAO_END_VERSIONED_NAMESPACE_DECL
