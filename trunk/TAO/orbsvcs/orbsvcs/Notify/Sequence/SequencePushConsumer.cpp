// $Id$

#include "SequencePushConsumer.h"

#if ! defined (__ACE_INLINE__)
#include "SequencePushConsumer.inl"
#endif /* __ACE_INLINE__ */

ACE_RCSID (Notify, TAO_Notify_SequencePushConsumer, "$Id$")

#include "ace/Reactor.h"
#include "tao/debug.h"
#include "../QoSProperties.h"
#include "../ProxySupplier.h"
#include "../Worker_Task.h"
#include "../Consumer.h"
#include "../Method_Request_Dispatch.h"
#include "../Method_Request_Event.h"
#include "../Timer.h"
#include "../Proxy.h"
#include "../Properties.h"
//#define DEBUG_LEVEL 10
#ifndef DEBUG_LEVEL
# define DEBUG_LEVEL TAO_debug_level
#endif //DEBUG_LEVEL

TAO_Notify_SequencePushConsumer::TAO_Notify_SequencePushConsumer (TAO_Notify_ProxySupplier* proxy)
  : TAO_Notify_Consumer (proxy)
{
}

TAO_Notify_SequencePushConsumer::~TAO_Notify_SequencePushConsumer ()
{
}

void
TAO_Notify_SequencePushConsumer::init (
  CosNotifyComm::SequencePushConsumer_ptr push_consumer, TAO_Notify_AdminProperties_var& admin_properties
#if 1
  ACE_ENV_ARG_DECL_NOT_USED)
#else //1
  ACE_ENV_ARG_DECL)
#endif
{
  set_consumer (push_consumer);

#if 1 //// @@ TODO: use buffering strategy in TAO_Notify_Consumer???
  ACE_UNUSED_ARG ( admin_properties);
#else //1

  ACE_NEW_THROW_EX (this->buffering_strategy_,
                    TAO_Notify_Batch_Buffering_Strategy (this->msg_queue_, admin_properties,
                                                     this->max_batch_size_.value ()),
                    CORBA::NO_MEMORY ());
#endif // 1
}

void
TAO_Notify_SequencePushConsumer::set_consumer (
  CosNotifyComm::SequencePushConsumer_ptr push_consumer)
{
  this->push_consumer_ = CosNotifyComm::SequencePushConsumer::_duplicate (push_consumer);
  this->publish_ = CosNotifyComm::NotifyPublish::_duplicate (push_consumer);

}


void
TAO_Notify_SequencePushConsumer::release (void)
{
  delete this;
  //@@ inform factory
}

bool
TAO_Notify_SequencePushConsumer::dispatch_from_queue (Request_Queue& requests, ACE_Guard <TAO_SYNCH_MUTEX> & ace_mon)
{
  bool result = true;
  if (DEBUG_LEVEL > 0) ACE_DEBUG ( (LM_DEBUG,
      ACE_TEXT ("(%P|%t) SequencePushConsumer dispatch queued requests. queue size:%u\n"),
      requests.size ()));

  long queue_size = requests.size ();
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
  if (DEBUG_LEVEL > 0) ACE_DEBUG ( (LM_DEBUG,
        ACE_TEXT ("(%P|%t) Sequence Dispatch Method_Request_Dispatch @%@\n"),
        request));

      const TAO_Notify_Event * ev = request->event ();
      ev->convert (batch [pos]);
      ++pos;

      // note enqueue at head, use queue as stack.
      completed.enqueue_head (request);
    }
    batch.length (pos);
    ACE_ASSERT (pos > 0);

    ace_mon.release ();
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
      // TODO: we should distinguish between these (someday)
      case DISPATCH_FAIL:
      case DISPATCH_DISCARD:
      {
        TAO_Notify_Method_Request_Event_Queueable *  request = 0;
        while (completed.dequeue_head (request) == 0)
        {
          if (request->should_retry ())
          {
            if (DEBUG_LEVEL > 0) ACE_DEBUG ( (LM_DEBUG,
              ACE_TEXT ("(%P|%t) SequencePushConsumer: Failed to dispatch :%d. Will retry\n"),
              request->sequence ()
              ));
            requests.enqueue_head (request);
            result = false;
          }
          else
          {
            if (DEBUG_LEVEL > 0) ACE_DEBUG ( (LM_DEBUG,
              ACE_TEXT ("(%P|%t) SequencePushConsumer: Failed to dispatch :%d. Discarding event.\n"),
              request->sequence ()
              ));
            request->complete ();
            request->release ();
          }
        }
        break;
      }
      case DISPATCH_RETRY:
      {
        if (DEBUG_LEVEL > 0) ACE_DEBUG ( (LM_DEBUG,
          ACE_TEXT ("(%P|%t) Consumer %d: Will retry %d\n"),
          ACE_static_cast (int, this->proxy ()->id ()),
          request->sequence ()
          ));
        ace_mon.acquire ();
        requests.enqueue_head (request); // put the failed event back where it was
        result = false;
        break;
      }
    }
  }
  return result;
}

bool
TAO_Notify_SequencePushConsumer::enqueue_if_necessary (
    TAO_Notify_Method_Request_Event * request
    ACE_ENV_ARG_DECL)
{
  if (DEBUG_LEVEL > 0)
    ACE_DEBUG ( (LM_DEBUG, "SequencePushConsumer enqueing event.\n"));
  this->enqueue_request (request ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (false);

  if (this->pacing_.is_valid ())
  {
    schedule_timer (false);
  }
  else
  {
    this->dispatch_pending (ACE_ENV_SINGLE_ARG_PARAMETER);
  }
  return true;
}


void
TAO_Notify_SequencePushConsumer::push (const CORBA::Any& /*event*/ ACE_ENV_ARG_DECL_NOT_USED)
{
  //NOP
}

void
TAO_Notify_SequencePushConsumer::push (const CosNotification::StructuredEvent& /*notification*/ ACE_ENV_ARG_DECL_NOT_USED)
{
  //NOP
}


void
TAO_Notify_SequencePushConsumer::push (const CosNotification::EventBatch& event_batch ACE_ENV_ARG_DECL)
{
  this->push_consumer_->push_structured_events (event_batch ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;
}

bool
TAO_Notify_SequencePushConsumer::get_ior (ACE_CString & iorstr) const
{
  bool result = false;
  CORBA::ORB_var orb = TAO_Notify_PROPERTIES::instance ()->orb ();
  ACE_DECLARE_NEW_CORBA_ENV;
  ACE_TRY
  {
    CORBA::String_var ior = orb->object_to_string (this->push_consumer_.in () ACE_ENV_ARG_PARAMETER);
    ACE_TRY_CHECK;
    iorstr = ACE_static_cast (const char *, ior.in ());
    result = true;
  }
  ACE_CATCHANY
  {
    ACE_ASSERT (0);
  }
  ACE_ENDTRY;
  return result;
}

void
TAO_Notify_SequencePushConsumer::reconnect_from_consumer (TAO_Notify_Consumer* old_consumer
    ACE_ENV_ARG_DECL_NOT_USED)
{
  TAO_Notify_SequencePushConsumer* tmp = dynamic_cast<TAO_Notify_SequencePushConsumer *> (old_consumer);
  ACE_ASSERT(tmp != 0);
  this->set_consumer(tmp->push_consumer_.in());
  this->schedule_timer(false);
}
