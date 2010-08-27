// $Id$

#include "orbsvcs/Notify/Any/PushConsumer.h"
#include "ace/Bound_Ptr.h"
#include "tao/Stub.h" // For debug messages printing out ORBid.
#include "tao/ORB_Core.h"
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
TAO_Notify_PushConsumer::init (CosEventComm::PushConsumer_ptr push_consumer)
{
  // Initialize only once
  ACE_ASSERT( CORBA::is_nil (this->push_consumer_.in()) );

  // push_consumer not optional
  if (CORBA::is_nil (push_consumer))
  {
    throw CORBA::BAD_PARAM();
  }

  try
  {
    if (!TAO_Notify_PROPERTIES::instance()->separate_dispatching_orb ())
      {
        this->push_consumer_ = CosEventComm::PushConsumer::_duplicate (push_consumer);

        this->publish_ =
          CosNotifyComm::NotifyPublish::_narrow (push_consumer);
      }
    else
      {
        // "Port" consumer's object reference from receiving ORB to dispatching ORB.
        CORBA::String_var temp =
          TAO_Notify_PROPERTIES::instance()->orb()->object_to_string(push_consumer);

        CORBA::Object_var obj =
          TAO_Notify_PROPERTIES::instance()->dispatching_orb()->string_to_object(temp.in());

        CosEventComm::PushConsumer_var new_cos_comm_pc =
          CosEventComm::PushConsumer::_unchecked_narrow(obj.in());

        this->push_consumer_ =
          CosEventComm::PushConsumer::_duplicate (new_cos_comm_pc.in());

        //
        // Note that here we do an _unchecked_narrow() in order to avoid
        // making a call on the consumer b/c the consumer may not have activated
        // its POA just yet.  That means that before we use this reference the first
        // time, we'll actually need to call _is_a() on it, i.e., the equivalent
        // of an _narrow().  At the time of this writing, the only use of
        // this->publish_ is in TAO_NS_Consumer::dispatch_updates_i (the superclass).
        // If any other use is made of this data member, then the code to validate
        // the actual type of the target object must be refactored.
        this->publish_ =
          CosNotifyComm::NotifyPublish::_unchecked_narrow (obj.in());


        //--cj verify dispatching ORB
        if (TAO_debug_level >= 10)
          {
            ACE_DEBUG ((LM_DEBUG, "(%P|%t) Any push init dispatching ORB id is %s.\n",
                        obj->_stubobj()->orb_core()->orbid()));
          }
        //--cj end
      }
  }
  catch (const CORBA::TRANSIENT& ex)
    {
      ex._tao_print_exception ("Got a TRANSIENT in NS_PushConsumer::init");
      ACE_DEBUG ((LM_DEBUG, "(%P|%t) got it for NS_PushConsumer %@\n", this));
    }
  catch (const CORBA::Exception&)
    {
      // _narrow failed which probably means the interface is CosEventComm type.
    }
}

void
TAO_Notify_PushConsumer::release (void)
{
  delete this;
  //@@ inform factory
}

void
TAO_Notify_PushConsumer::push (const CORBA::Any& payload)
{
  //--cj verify dispatching ORB
  if (TAO_debug_level >= 10) {
    ACE_DEBUG ((LM_DEBUG, "(%P|%t) Any push dispatching ORB id is %s.\n",
                this->push_consumer_->_stubobj()->orb_core()->orbid()));
  }
  //--cj end

  last_ping_ = ACE_OS::gettimeofday ();
  this->push_consumer_->push (payload);
}

void
TAO_Notify_PushConsumer::push (const CosNotification::StructuredEvent& event)
{
  CORBA::Any any;

  TAO_Notify_Event::translate (event, any);

  last_ping_ = ACE_OS::gettimeofday ();
  this->push_consumer_->push (any);
}

/// Push a batch of events to this consumer.
void
TAO_Notify_PushConsumer::push (const CosNotification::EventBatch& event)
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

void
TAO_Notify_PushConsumer::reconnect_from_consumer (TAO_Notify_Consumer* old_consumer)
{
  TAO_Notify_PushConsumer* tmp =
    dynamic_cast<TAO_Notify_PushConsumer*> (old_consumer);
  ACE_ASSERT(tmp != 0);
  this->init(tmp->push_consumer_.in());
  this->schedule_timer(false);
}

CORBA::Object_ptr
TAO_Notify_PushConsumer::get_consumer (void)
{
  return CosEventComm::PushConsumer::_duplicate (this->push_consumer_.in ());
}

TAO_END_VERSIONED_NAMESPACE_DECL
