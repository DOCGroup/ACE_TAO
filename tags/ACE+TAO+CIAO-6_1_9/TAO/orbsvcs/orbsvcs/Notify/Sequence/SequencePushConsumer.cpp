// $Id$

#include "orbsvcs/Log_Macros.h"
#include "orbsvcs/Notify/Sequence/SequencePushConsumer.h"
#include "ace/Truncate.h"
#include "ace/Reactor.h"
#include "tao/debug.h"
#include "tao/Stub.h" // For debug messages printing out ORBid.
#include "tao/ORB_Core.h"
#include "orbsvcs/Notify/QoSProperties.h"
#include "orbsvcs/Notify/ProxySupplier.h"
#include "orbsvcs/Notify/Worker_Task.h"
#include "orbsvcs/Notify/Consumer.h"
#include "orbsvcs/Notify/Method_Request_Dispatch.h"
#include "orbsvcs/Notify/Method_Request_Event.h"
#include "orbsvcs/Notify/Timer.h"
#include "orbsvcs/Notify/Proxy.h"
#include "orbsvcs/Notify/Properties.h"
//#define DEBUG_LEVEL 10
#ifndef DEBUG_LEVEL
# define DEBUG_LEVEL TAO_debug_level
#endif //DEBUG_LEVEL

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

TAO_Notify_SequencePushConsumer::TAO_Notify_SequencePushConsumer (TAO_Notify_ProxySupplier* proxy)
: TAO_Notify_Consumer (proxy)
{
}

TAO_Notify_SequencePushConsumer::~TAO_Notify_SequencePushConsumer ()
{
}

void
TAO_Notify_SequencePushConsumer::init (CosNotifyComm::SequencePushConsumer_ptr push_consumer)
{
  // Initialize only once
  ACE_ASSERT( CORBA::is_nil (this->push_consumer_.in()) );

  if (CORBA::is_nil (push_consumer))
  {
    throw CORBA::BAD_PARAM();
  }

  if (!TAO_Notify_PROPERTIES::instance()->separate_dispatching_orb ())
    {
      this->push_consumer_ = CosNotifyComm::SequencePushConsumer::_duplicate (push_consumer);
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
          CosNotifyComm::SequencePushConsumer_var new_push_consumer =
            CosNotifyComm::SequencePushConsumer::_unchecked_narrow(obj.in());

          this->push_consumer_ = CosNotifyComm::SequencePushConsumer::_duplicate (new_push_consumer.in());
          this->publish_ = CosNotifyComm::NotifyPublish::_duplicate (new_push_consumer.in());

          //--cj verify dispatching ORB
          if (TAO_debug_level >= 10)
            {
              ORBSVCS_DEBUG ((LM_DEBUG,
                          "(%P|%t) Sequence push init dispatching ORB id is %s.\n",
                          obj->_stubobj()->orb_core()->orbid()));
            }
          //--cj end
        }
      catch (const CORBA::TRANSIENT& ex)
        {
          ex._tao_print_exception (
            "Got a TRANSIENT in NS_SequencePushConsumer::init");
          ORBSVCS_DEBUG ((LM_DEBUG, "(%P|%t) got it for NS_SequencePushConsumer %@\n", this));
        }
      catch (const CORBA::Exception&)
        {
          // _narrow failed
        }
    }
}

void
TAO_Notify_SequencePushConsumer::release (void)
{
  delete this;
  //@@ inform factory
}

// FUZZ: disable check_for_ACE_Guard
bool
TAO_Notify_SequencePushConsumer::dispatch_from_queue (Request_Queue& requests, ACE_Guard <TAO_SYNCH_MUTEX> & ace_mon)
// FUZZ: enable check_for_ACE_Guard
{
  bool result = true;
  if (DEBUG_LEVEL > 0)
  {
    ORBSVCS_DEBUG ( (LM_DEBUG,
      ACE_TEXT ("(%P|%t) SequencePushConsumer dispatch queued requests. queue size:%u\n"),
      requests.size ()));
  }

  CORBA::ULong queue_size = ACE_Utils::truncate_cast<CORBA::ULong> (requests.size ());
  CORBA::Long max_batch_size = queue_size;
  if (this->max_batch_size_.is_valid () )
  {
    max_batch_size = this->max_batch_size_.value ();
  }
  CORBA::Long batch_size = queue_size;
  if (batch_size > max_batch_size)
  {
    batch_size = max_batch_size;
  }
  if (batch_size > 0)
  {
    CosNotification::EventBatch batch (batch_size);
    batch.length (batch_size);

    Request_Queue completed;

    CORBA::Long pos = 0;
    TAO_Notify_Method_Request_Event_Queueable * request = 0;
    while (pos < batch_size && requests.dequeue_head (request) == 0)
    {
      if (DEBUG_LEVEL > 0)
      {
        ORBSVCS_DEBUG ( (LM_DEBUG,
          ACE_TEXT ("(%P|%t) Sequence Dispatch Method_Request_Dispatch @%@\n"),
          request));
      }

      const TAO_Notify_Event * ev = request->event ();
      ev->convert (batch [pos]);
      ++pos;

      // note enqueue at head, use queue as stack.
      completed.enqueue_head (request);
    }
    batch.length (pos);
    ACE_ASSERT (pos > 0);

    ace_mon.release ();
    bool from_timeout = false;
    TAO_Notify_Consumer::DispatchStatus status =
      this->dispatch_batch (batch);
    ace_mon.acquire ();
    switch (status)
    {
    case DISPATCH_SUCCESS:
      {
        TAO_Notify_Method_Request_Event_Queueable * request = 0;
        while (completed.dequeue_head (request) == 0)
        {
          request->complete ();
          request->release ();
        }
        result = true;
        break;
      }
    case DISPATCH_FAIL_TIMEOUT:
      from_timeout = true;
      // Fall through
    case DISPATCH_FAIL:
      {
        TAO_Notify_Method_Request_Event_Queueable * request = 0;
        while (completed.dequeue_head (request) == 0)
        {
          if (request->should_retry ())
          {
            if (DEBUG_LEVEL > 0)
              ORBSVCS_DEBUG ((LM_DEBUG, ACE_TEXT ("(%P|%t) Consumer %d: Will retry %d\n"),
                          static_cast <int> (this->proxy ()->id ()),
                          request->sequence ()));
            requests.enqueue_head (request);
            result = false;
          }
          else
          {
            if (DEBUG_LEVEL > 0)
              ORBSVCS_DEBUG ((LM_DEBUG, ACE_TEXT ("(%P|%t) Consumer %d: Discarding %d\n"),
                          static_cast<int> (this->proxy ()->id ()),
                          request->sequence ()));
            request->complete ();
            request->release ();
          }
        }
        while (requests.dequeue_head (request) == 0)
        {
          if (request->should_retry ())
          {
            if (DEBUG_LEVEL > 0)
              ORBSVCS_DEBUG ((LM_DEBUG, ACE_TEXT ("(%P|%t) Consumer %d: Will retry %d\n"),
                          static_cast<int> (this->proxy ()->id ()),
                          request->sequence ()));
            requests.enqueue_head (request);
            result = false;
          }
          else
          {
            if (DEBUG_LEVEL > 0)
              ORBSVCS_DEBUG ((LM_DEBUG, ACE_TEXT ("(%P|%t) Consumer %d: Discarding %d\n"),
                          static_cast<int> (this->proxy ()->id ()),
                          request->sequence ()));
            request->complete ();
            request->release ();
          }
        }
        ace_mon.release();
        try
        {
          this->proxy_supplier ()->destroy (from_timeout);
        }
        catch (const CORBA::Exception&)
        {
          // todo is there something meaningful we can do here?
          ;
        }
        ace_mon.acquire();
        break;
      }
    case DISPATCH_RETRY:
    case DISPATCH_DISCARD:
      {
        TAO_Notify_Method_Request_Event_Queueable *  request = 0;
        while (completed.dequeue_head (request) == 0)
        {
          if (request->should_retry ())
          {
            if (DEBUG_LEVEL > 0)
              ORBSVCS_DEBUG ((LM_DEBUG, ACE_TEXT ("(%P|%t) Consumer %d: Will retry %d\n"),
                          static_cast<int> (this->proxy ()->id ()),
                          request->sequence ()));
            requests.enqueue_head (request);
            result = false;
          }
          else
          {
            if (DEBUG_LEVEL > 0)
              ORBSVCS_DEBUG ((LM_DEBUG, ACE_TEXT ("(%P|%t) Consumer %d: Discarding %d\n"),
                          static_cast<int> (this->proxy ()->id ()),
                          request->sequence ()));
            request->complete ();
            request->release ();
          }
        }
        break;
      }
    default:
      {
        result = false;
        break;
      }
    }
  }
  return result;
}

bool
TAO_Notify_SequencePushConsumer::enqueue_if_necessary (
  TAO_Notify_Method_Request_Event * request)
{
  if (DEBUG_LEVEL > 0)
    ORBSVCS_DEBUG ( (LM_DEBUG, "SequencePushConsumer enqueing event.\n"));
  this->enqueue_request (request);

  size_t mbs = static_cast<size_t>(this->max_batch_size_.value());

  if (this->pending_events().size() >= mbs || this->pacing_.is_valid () == 0)
  {
    this->dispatch_pending ();
  }
  else
  {
    schedule_timer (false);
  }
  return true;
}


void
TAO_Notify_SequencePushConsumer::push (const CORBA::Any& /*event*/)
{
  //NOP
}

void
TAO_Notify_SequencePushConsumer::push (const CosNotification::StructuredEvent& /*notification*/)
{
  //NOP
}


void
TAO_Notify_SequencePushConsumer::push (const CosNotification::EventBatch& event_batch)
{
  //--cj verify dispatching ORB
  if (TAO_debug_level >= 10) {
    ORBSVCS_DEBUG ((LM_DEBUG, "(%P|%t) Sequence push dispatching ORB id is %s.\n",
                this->push_consumer_->_stubobj()->orb_core()->orbid()));
  }
  //--cj end
  last_ping_ = ACE_OS::gettimeofday ();
  this->push_consumer_->push_structured_events (event_batch);
}

ACE_CString
TAO_Notify_SequencePushConsumer::get_ior (void) const
{
  ACE_CString result;
  CORBA::ORB_var orb = TAO_Notify_PROPERTIES::instance ()->orb ();
  try
  {
    CORBA::String_var ior = orb->object_to_string (this->push_consumer_.in ());
    result = static_cast<const char*> (ior.in ());
  }
  catch (const CORBA::Exception&)
  {
    result.fast_clear();
  }
  return result;
}

void
TAO_Notify_SequencePushConsumer::reconnect_from_consumer (TAO_Notify_Consumer* old_consumer)
{
  TAO_Notify_SequencePushConsumer* tmp = dynamic_cast<TAO_Notify_SequencePushConsumer *> (old_consumer);
  ACE_ASSERT(tmp != 0);
  this->init(tmp->push_consumer_.in());
  this->schedule_timer(false);
}

CORBA::Object_ptr
TAO_Notify_SequencePushConsumer::get_consumer (void)
{
  return CosNotifyComm::SequencePushConsumer::_duplicate (this->push_consumer_.in ());
}

TAO_END_VERSIONED_NAMESPACE_DECL
