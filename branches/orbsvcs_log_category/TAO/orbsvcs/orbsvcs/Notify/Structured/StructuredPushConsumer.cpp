// $Id$
#include "orbsvcs/Log_Macros.h"
#include "orbsvcs/Notify/Structured/StructuredPushConsumer.h"
#include "ace/Bound_Ptr.h"
#include "tao/Stub.h" // For debug messages printing out ORBid.
#include "tao/ORB_Core.h"
#include "orbsvcs/Notify/Properties.h"
#include "orbsvcs/Notify/Event.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

TAO_Notify_StructuredPushConsumer::TAO_Notify_StructuredPushConsumer (TAO_Notify_ProxySupplier* proxy)
  :TAO_Notify_Consumer (proxy), connection_valid(0)
{
}

TAO_Notify_StructuredPushConsumer::~TAO_Notify_StructuredPushConsumer ()
{
}

void
TAO_Notify_StructuredPushConsumer::init (CosNotifyComm::StructuredPushConsumer_ptr push_consumer)
{
  // Initialize only once
  ACE_ASSERT( CORBA::is_nil (this->push_consumer_.in()) );

  if (CORBA::is_nil (push_consumer))
  {
    throw CORBA::BAD_PARAM();
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

      try
        {
          CosNotifyComm::StructuredPushConsumer_var new_push_consumer =
            CosNotifyComm::StructuredPushConsumer::_unchecked_narrow(obj.in());

          this->push_consumer_ = CosNotifyComm::StructuredPushConsumer::_duplicate (new_push_consumer.in());
          this->publish_ = CosNotifyComm::NotifyPublish::_duplicate (new_push_consumer.in());

          //--cj verify dispatching ORB
          if (TAO_debug_level >= 10)
            {
              ORBSVCS_DEBUG ((LM_DEBUG,
                          "(%P|%t) Structured push init dispatching ORB id is %s.\n",
                          obj->_stubobj()->orb_core()->orbid()));
          }
          //--cj end
        }
      catch (const CORBA::TRANSIENT& ex)
        {
          ex._tao_print_exception (
            "Got a TRANSIENT in NS_StructuredPushConsumer::init");
          ORBSVCS_DEBUG ((LM_DEBUG, "(%P|%t) got it for NS_StructuredPushConsumer %@\n", this));
        }
      catch (const CORBA::Exception&)
        {
          // _narrow failed
        }
    }
}

void
TAO_Notify_StructuredPushConsumer::release (void)
{
  delete this;
  //@@ inform factory
}

void
TAO_Notify_StructuredPushConsumer::push (const CORBA::Any& event)
{
  CosNotification::StructuredEvent notification;

  TAO_Notify_Event::translate (event, notification);

  // Check if we have to validate connection
  if ( !connection_valid ) {
    try
      {
        CORBA::PolicyList_var inconsistent_policies;
        this->push_consumer_->_validate_connection (inconsistent_policies.out());
      }
    catch (const CORBA::COMM_FAILURE&)
      {
        // Expected exception when a bad connection is found
        if (TAO_debug_level >= 1)
          {
            ORBSVCS_DEBUG ((LM_DEBUG, "(%P|%t) Found bad connection.\n"));
          }
      }
    connection_valid = 1;
  }

  last_ping_ = ACE_OS::gettimeofday ();

  this->push_consumer_->push_structured_event (notification);
}

void
TAO_Notify_StructuredPushConsumer::push (const CosNotification::StructuredEvent& event)
{
  //--cj verify dispatching ORB
  if (TAO_debug_level >= 10) {
    ORBSVCS_DEBUG ((LM_DEBUG, "(%P|%t) Structured push dispatching ORB id is %s.\n",
                this->push_consumer_->_stubobj()->orb_core()->orbid()));
  }
  //--cj end

  // Check if we have to validate connection
  if ( !connection_valid ) {
    try
      {
        CORBA::PolicyList_var inconsistent_policies;
        this->push_consumer_->_validate_connection (inconsistent_policies.out());
      }
    catch (const CORBA::COMM_FAILURE&)
      {
        // Expected exception when a bad connection is found
        if (TAO_debug_level >= 1)
          {
            ORBSVCS_DEBUG ((LM_DEBUG, "(%P|%t) Found bad connection.\n"));
          }
      }
    connection_valid = 1;
  }

  last_ping_ = ACE_OS::gettimeofday ();

  this->push_consumer_->push_structured_event (event);
}

/// Push a batch of events to this consumer.
void
TAO_Notify_StructuredPushConsumer::push (const CosNotification::EventBatch& event)
{
  ACE_ASSERT(false);
  ACE_UNUSED_ARG (event);
  // TODO exception?
}

void
TAO_Notify_StructuredPushConsumer::reconnect_from_consumer (TAO_Notify_Consumer* old_consumer)
{
  TAO_Notify_StructuredPushConsumer* tmp = dynamic_cast<TAO_Notify_StructuredPushConsumer *> (old_consumer);
  ACE_ASSERT(tmp != 0);
  this->init(tmp->push_consumer_.in());
  this->schedule_timer(false);
}

ACE_CString
TAO_Notify_StructuredPushConsumer::get_ior (void) const
{
  ACE_CString result;
  CORBA::ORB_var orb = TAO_Notify_PROPERTIES::instance()->orb();
  try
  {
    CORBA::String_var ior = orb->object_to_string(this->push_consumer_.in());
    result = static_cast<const char*> (ior.in ());
  }
  catch (const CORBA::Exception&)
  {
    result.fast_clear();
  }
  return result;
}

CORBA::Object_ptr
TAO_Notify_StructuredPushConsumer::get_consumer (void)
{
  return CosNotifyComm::StructuredPushConsumer::_duplicate (this->push_consumer_.in ());
}

TAO_END_VERSIONED_NAMESPACE_DECL
