// $Id$

#include "orbsvcs/Notify/Any/PushConsumer.h"

ACE_RCSID (Notify,
           TAO_Notify_PushConsumer,
           "$Id$")

#include "ace/Bound_Ptr.h"
#include "orbsvcs/CosEventCommC.h"
#include "orbsvcs/Notify/Event.h"
#include "orbsvcs/Notify/Properties.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

TAO_Notify_PushConsumer::TAO_Notify_PushConsumer (TAO_Notify_ProxySupplier* proxy)
  :TAO_Notify_Consumer (proxy)
{
}

TAO_Notify_PushConsumer::~TAO_Notify_PushConsumer ()
{
}

void
TAO_Notify_PushConsumer::init (CosEventComm::PushConsumer_ptr push_consumer
                               ACE_ENV_ARG_DECL)
{
  // Initialize only once
  ACE_ASSERT( CORBA::is_nil (this->push_consumer_.in()) );

  // push_consumer not optional
  if (CORBA::is_nil (push_consumer))
  {
    ACE_THROW (CORBA::BAD_PARAM());
  }

  this->push_consumer_ = CosEventComm::PushConsumer::_duplicate (push_consumer);

  ACE_TRY
    {
      this->publish_ =
        CosNotifyComm::NotifyPublish::_narrow (push_consumer
                                               ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;
    }
  ACE_CATCHANY
    {
      // _narrow failed which probably means the interface is CosEventComm type.
    }
  ACE_ENDTRY;
}

void
TAO_Notify_PushConsumer::release (void)
{
  delete this;
  //@@ inform factory
}

void
TAO_Notify_PushConsumer::push (const CORBA::Any& payload ACE_ENV_ARG_DECL)
{
  this->push_consumer_->push (payload ACE_ENV_ARG_PARAMETER);
}

void
TAO_Notify_PushConsumer::push (const CosNotification::StructuredEvent& event ACE_ENV_ARG_DECL)
{
  CORBA::Any any;

  TAO_Notify_Event::translate (event, any);

  this->push_consumer_->push (any ACE_ENV_ARG_PARAMETER);
}

/// Push a batch of events to this consumer.
void
TAO_Notify_PushConsumer::push (const CosNotification::EventBatch& event ACE_ENV_ARG_DECL_NOT_USED)
{
  ACE_ASSERT(false);
  ACE_UNUSED_ARG (event);
  // TODO exception?
}

ACE_CString
TAO_Notify_PushConsumer::get_ior (void) const
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

void
TAO_Notify_PushConsumer::reconnect_from_consumer (TAO_Notify_Consumer* old_consumer
    ACE_ENV_ARG_DECL)
{
  TAO_Notify_PushConsumer* tmp =
    dynamic_cast<TAO_Notify_PushConsumer*> (old_consumer);
  ACE_ASSERT(tmp != 0);
  this->init(tmp->push_consumer_.in() ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;
  this->schedule_timer(false);
}

TAO_END_VERSIONED_NAMESPACE_DECL
