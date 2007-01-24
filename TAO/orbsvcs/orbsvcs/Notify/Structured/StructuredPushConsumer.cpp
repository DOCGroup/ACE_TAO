// $Id$
#include "orbsvcs/Notify/Structured/StructuredPushConsumer.h"

ACE_RCSID(RT_Notify, TAO_Notify_StructuredPushConsumer, "$Id$")

#include "ace/Bound_Ptr.h"
#include "tao/Stub.h" // For debug messages printing out ORBid.
#include "tao/ORB_Core.h"
#include "orbsvcs/Notify/Properties.h"
#include "orbsvcs/Notify/Event.h"

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

  if (!TAO_Notify_PROPERTIES::instance()->separate_dispatching_orb ())
    {
      this->push_consumer_ = CosNotifyComm::StructuredPushConsumer::_duplicate (push_consumer);
      this->publish_ = CosNotifyComm::NotifyPublish::_duplicate (push_consumer);
    }
  else
    {
      // "Port" consumer's object reference from receiving ORB to dispatching ORB.
      CORBA::String_var temp =
        TAO_Notify_PROPERTIES::instance()->orb()->object_to_string(push_consumer);

      CORBA::Object_var obj =
        TAO_Notify_PROPERTIES::instance()->dispatching_orb()->string_to_object(temp.in());

      ACE_TRY
        {
          CosNotifyComm::StructuredPushConsumer_var new_push_consumer =
            CosNotifyComm::StructuredPushConsumer::_unchecked_narrow(obj.in() ACE_ENV_ARG_PARAMETER);

          this->push_consumer_ = CosNotifyComm::StructuredPushConsumer::_duplicate (new_push_consumer.in());
          this->publish_ = CosNotifyComm::NotifyPublish::_duplicate (new_push_consumer.in());
          //--cj verify dispatching ORB
          if (TAO_debug_level >= 10)
            {
              ACE_DEBUG ((LM_DEBUG,
                          "(%P|%t) Structured push init dispatching ORB id is %s.\n",
                          obj->_stubobj()->orb_core()->orbid()));
          }
          //--cj end
        }
      ACE_CATCH (CORBA::TRANSIENT, ex)
        {
          ACE_PRINT_EXCEPTION (ex, "Got a TRANSIENT in NS_StructuredPushConsumer::init");
          ACE_DEBUG ((LM_DEBUG, "(%P|%t) got it for NS_StructuredPushConsumer %@\n", this));
        }
      ACE_CATCHANY
        {
          // _narrow failed
        }
      ACE_ENDTRY;
    }
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
  //--cj verify dispatching ORB
  if (TAO_debug_level >= 10) {
    ACE_DEBUG ((LM_DEBUG, "(%P|%t) Structured push dispatching ORB id is %s.\n",
                this->push_consumer_->_stubobj()->orb_core()->orbid()));
  }
  //--cj end

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
