// $Id$

#include "Consumer.h"

#if ! defined (__ACE_INLINE__)
#include "Consumer.inl"
#endif /* __ACE_INLINE__ */

ACE_RCSID (RT_Notify, TAO_Notify_Consumer, "$Id$")

#include "Timer.h"
#include "orbsvcs/orbsvcs/Time_Utilities.h"
#include "ace/Refcounted_Auto_Ptr.h"
#include "ace/Unbounded_Queue.h"
#include "tao/debug.h"
#include "Method_Request_Event.h"

//#define DEBUG_LEVEL 10
#ifndef DEBUG_LEVEL
# define DEBUG_LEVEL TAO_debug_level
#endif //DEBUG_LEVEL

static const int DEFAULT_RETRY_TIMEOUT = 10;//120; // Note : This should be a config param or qos setting

TAO_Notify_Consumer::TAO_Notify_Consumer (TAO_Notify_ProxySupplier* proxy)
  : proxy_ (proxy)
  , pending_events_ (0)
  , is_suspended_ (0)
  , pacing_ (proxy->qos_properties_.pacing_interval ())
  , max_batch_size_ (CosNotification::MaximumBatchSize, 0)
  , timer_id_ (-1)
//  , buffering_strategy_ (0)
  , timer_ (0)
{
  ACE_NEW (
    this->pending_events_ ,
    TAO_Notify_Consumer::Request_Queue ()
    );

  this->timer_ = this->proxy ()->timer ();
}

TAO_Notify_Consumer::~TAO_Notify_Consumer ()
{
  delete this->pending_events_;
//  delete this->buffering_strategy_;
  if (this->timer_ == 0)
    {
      this->cancel_timer ();
      this->timer_->_decr_refcnt ();
      this->timer_ = 0;
    }
}

TAO_Notify_Proxy*
TAO_Notify_Consumer::proxy (void)
{
  return this->proxy_supplier ();
}

void
TAO_Notify_Consumer::qos_changed (const TAO_Notify_QoSProperties& qos_properties)
{
  this->max_batch_size_ = qos_properties.maximum_batch_size ();

/*
//@@ todo: consider buffering strategy
  if (this->max_batch_size_.is_valid ())
    {// set the max batch size.
      this->buffering_strategy_->batch_size (this->max_batch_size_.value ());
    }
*/

  // Inform the buffering strategy of qos change.
/*
  this->buffering_strategy_->update_qos_properties (qos_properties);
*/
}

void
TAO_Notify_Consumer::resume (ACE_ENV_SINGLE_ARG_DECL)
{
  this->is_suspended_ = 0;

  this->dispatch_pending (ACE_ENV_SINGLE_ARG_PARAMETER);
}

void
TAO_Notify_Consumer::enqueue_request (
  TAO_Notify_Method_Request_Event * request
  ACE_ENV_ARG_DECL)
{
  TAO_Notify_Event_var event_var;
  request->event ()->queueable_copy (event_var ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;
  TAO_Notify_Method_Request_Event_Queueable * queue_entry;
  ACE_NEW_THROW_EX (queue_entry,
    TAO_Notify_Method_Request_Event_Queueable (*request, event_var),
    CORBA::NO_MEMORY ());
  ACE_CHECK;

  if (DEBUG_LEVEL  > 3) ACE_DEBUG ( (LM_DEBUG,
    ACE_TEXT ("Consumer %d: enqueue_request (%d) @%@.\n"),
    ACE_static_cast (int, this->proxy ()->id ()),
    request->sequence (),
    request
    ));
  ACE_GUARD (TAO_SYNCH_MUTEX, ace_mon, *this->proxy_lock ());
  this->pending_events_->enqueue_tail (queue_entry);
}

bool
TAO_Notify_Consumer::enqueue_if_necessary (TAO_Notify_Method_Request_Event * request ACE_ENV_ARG_DECL)
{
  ACE_GUARD_RETURN (TAO_SYNCH_MUTEX, ace_mon, *this->proxy_lock (), false);
  if (! this->pending_events_->is_empty ())
    {
      if (DEBUG_LEVEL > 3)ACE_DEBUG ( (LM_DEBUG,
        ACE_TEXT ("Consumer %d: enqueuing another event. %d\n"),
        ACE_static_cast (int, this->proxy ()->id ()),
        request->sequence ()
          ));
      TAO_Notify_Event_var event_var;
      request->event ()->queueable_copy (event_var ACE_ENV_ARG_PARAMETER);
      ACE_CHECK_RETURN (false);
      TAO_Notify_Method_Request_Event_Queueable * queue_entry;
      ACE_NEW_THROW_EX (queue_entry,
        TAO_Notify_Method_Request_Event_Queueable (*request, event_var),
        CORBA::NO_MEMORY ());
      ACE_CHECK_RETURN (false);
      this->pending_events_->enqueue_tail (queue_entry);
      this->schedule_timer (false);
      return true;
    }
  if (this->is_suspended_ == 1)
    {
      if (DEBUG_LEVEL > 3) ACE_DEBUG ( (LM_DEBUG,
        ACE_TEXT ("Suspended Consumer %d enqueing event. %d\n"),
        ACE_static_cast (int, this->proxy ()->id ()),
        request->sequence ()
        ));
      TAO_Notify_Event_var event_var;
      request->event ()->queueable_copy (event_var ACE_ENV_ARG_PARAMETER);
      ACE_CHECK_RETURN (false);
      TAO_Notify_Method_Request_Event_Queueable * queue_entry;
      ACE_NEW_THROW_EX (queue_entry,
        TAO_Notify_Method_Request_Event_Queueable (*request, event_var),
        CORBA::NO_MEMORY ());
      ACE_CHECK_RETURN (false);
      this->pending_events_->enqueue_tail (queue_entry);
      this->schedule_timer (false);
      return true;
    }
  return false;
}

void
TAO_Notify_Consumer::deliver (TAO_Notify_Method_Request_Event * request ACE_ENV_ARG_DECL)
{
  // Increment reference counts (safely) to prevent this object and its proxy
  // from being deleted while the push is in progress.
  TAO_Notify_Refcountable_Guard_T<TAO_Notify_Proxy> proxy_guard (this->proxy ());
  TAO_Notify_Refcountable_Guard_T<TAO_Notify_Consumer> this_guard (this);
  bool queued = enqueue_if_necessary (request ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;
  if (!queued)
    {
      DispatchStatus status = this->dispatch_request (request);
      switch (status)
      {
        case DISPATCH_SUCCESS:
        {
          request->complete ();
          break;
        }
        case DISPATCH_RETRY:
        {
          if (DEBUG_LEVEL > 1) ACE_DEBUG ( (LM_DEBUG,
            ACE_TEXT ("Consumer %d enqueing event %d due to failed dispatch.\n"),
            ACE_static_cast (int, this->proxy ()->id ()),
            request->sequence ()));
          this->enqueue_request (request ACE_ENV_ARG_PARAMETER);
          ACE_CHECK;
          this->schedule_timer (true);
          break;
        }
        case DISPATCH_DISCARD:
        {
          if (DEBUG_LEVEL  > 0) ACE_DEBUG ( (LM_DEBUG,
            ACE_TEXT ("(%P|%t) Consumer %d: Error during direct dispatch. Discarding event:%d.\n"),
            ACE_static_cast (int, this->proxy ()->id ()),
            request->sequence ()
            ));
          request->complete ();
          break;
        }
        case DISPATCH_FAIL:
        {
          if (DEBUG_LEVEL  > 0) ACE_DEBUG ( (LM_DEBUG,
            ACE_TEXT ("(%P|%t) Consumer %d: Failed during direct dispatch :%d. Discarding event.\n"),
            ACE_static_cast (int, this->proxy ()->id ()),
            request->sequence ()
            ));
          request->complete ();
          ACE_TRY_NEW_ENV
          {
            this->proxy_supplier ()->destroy (ACE_ENV_SINGLE_ARG_PARAMETER);
            ACE_TRY_CHECK;
          }
          ACE_CATCHANY
          {
            // todo is there something meaningful we can do here?
            ;
          }
          ACE_ENDTRY;
          break;
        }
      }
    }
}

TAO_Notify_Consumer::DispatchStatus
TAO_Notify_Consumer::dispatch_request (TAO_Notify_Method_Request_Event * request)
{
  DispatchStatus result = DISPATCH_SUCCESS;
  ACE_TRY_NEW_ENV
  {
    request->event ()->push (this ACE_ENV_ARG_PARAMETER);
    ACE_TRY_CHECK;
    if (DEBUG_LEVEL  > 8) ACE_DEBUG ( (LM_DEBUG,
      ACE_TEXT ("Consumer %d dispatched single event %d.\n"),
      ACE_static_cast (int, this->proxy ()->id ()),
      request->sequence ()
      ));
  }
  ACE_CATCH (CORBA::OBJECT_NOT_EXIST, ex)
  {
    if (DEBUG_LEVEL  > 0) ACE_DEBUG ( (LM_ERROR,
        ACE_TEXT ("(%P|%t) TAO_Notify_Consumer %d::push (request) OBJECT_NOT_EXIST %s\n"),
        ACE_static_cast (int, this->proxy ()->id ()),
        ex._info ().c_str ()
        ));
    result = DISPATCH_FAIL;
  }
  ACE_CATCH (CORBA::TRANSIENT, ex)
  {
    if (DEBUG_LEVEL  > 0) ACE_DEBUG ( (LM_ERROR,
        ACE_TEXT ("(%P|%t) TAO_Notify_Consumer %d::push (request) Transient (minor=%d) %s\n"),
        ACE_static_cast (int, this->proxy ()->id ()),
        ex.minor (),
        ex._info ().c_str ()
        ));
    const CORBA::ULong BITS_5_THRU_12_MASK = 0x00000f80u;
    switch (ex.minor () & 0xfffff000u)
      {
      case CORBA::OMGVMCID:
        switch (ex.minor () & 0x00000fffu)
        {
        case 2: // No usable profile
        case 3: // Request cancelled
        case 4: // POA destroyed
          result = DISPATCH_FAIL;
          break;
        default:
          result = DISPATCH_DISCARD;
        }
        break;

      case TAO_DEFAULT_MINOR_CODE:
      default:
        switch (ex.minor () & BITS_5_THRU_12_MASK)
          {
          case TAO_INVOCATION_SEND_REQUEST_MINOR_CODE:
            result = DISPATCH_FAIL;
            break;
          case TAO_POA_DISCARDING:
          case TAO_POA_HOLDING:
          default:
            result = DISPATCH_RETRY;
          } break;
            }

  }
  ACE_CATCH (CORBA::SystemException, ex)
  {
    if (DEBUG_LEVEL  > 0) ACE_DEBUG ( (LM_ERROR,
        ACE_TEXT ("(%P|%t) TAO_Notify_Consumer %d::push (request) SystemException %s\n"),
        ACE_static_cast (int, this->proxy ()->id ()),
        ex._info ().c_str ()
        ));
    result = DISPATCH_DISCARD;
  }
  ACE_CATCHANY
  {
    ACE_ERROR ( (LM_ERROR,
      ACE_TEXT ("(%P|%t) TAO_Notify_Consumer %d::push (request) Caught unexpected exception pushing event to consumer.\n"),
      ACE_static_cast (int, this->proxy ()->id ())
      ));
    result = DISPATCH_DISCARD;
  }
  ACE_ENDTRY;

  // for persistent events that haven't timed out
  // convert "FAIL" & "DISCARD" to "RETRY"
  // for transient events, convert RETRY to DISCARD (hey, best_effort.)
  if (result == DISPATCH_FAIL || result == DISPATCH_DISCARD)
  {
    if (request->should_retry ())
    {
      result = DISPATCH_RETRY;
    }
  }
  else if (result == DISPATCH_RETRY)
  {
    if (! request->should_retry ())
    {
      result = DISPATCH_DISCARD;
    }
  }

  return result;
}

TAO_Notify_Consumer::DispatchStatus
TAO_Notify_Consumer::dispatch_batch (const CosNotification::EventBatch& batch)
{
  ACE_TRY_NEW_ENV
  {
    this->push (batch ACE_ENV_ARG_PARAMETER);
    ACE_TRY_CHECK;
  }
  ACE_CATCH (CORBA::OBJECT_NOT_EXIST, not_exist)
  {
    if (DEBUG_LEVEL  > 0) ACE_DEBUG ( (LM_ERROR,
        ACE_TEXT ("(%P|%t) TAO_Notify_Consumer %d::dispatch_batch OBJECT_NOT_EXIST %s\n"),
        ACE_static_cast (int, this->proxy ()->id ()),
        not_exist._info ().c_str ()
        ));
    return DISPATCH_FAIL;
  }
  ACE_CATCH (CORBA::SystemException, ex)
  {
    if (DEBUG_LEVEL  > 0) ACE_DEBUG ( (LM_ERROR,
        ACE_TEXT ("(%P|%t) TAO_Notify_Consumer %d::dispatch_batch SystemException %s\n"),
        ACE_static_cast (int, this->proxy ()->id ()),
        ex._info ().c_str ()
        ));
    // @@todo what to return here?
    return DISPATCH_RETRY;
  }
  ACE_CATCHANY
  {
    ACE_ERROR ( (LM_ERROR,
      ACE_TEXT ("(%P|%t) Consumer %d: Caught unexpected exception pushing EventBatch to consumer.\n"),
      ACE_static_cast (int, this->proxy ()->id ())
      ));
    return DISPATCH_FAIL;
  }
  ACE_ENDTRY;
  return DISPATCH_SUCCESS;
}

void
TAO_Notify_Consumer::dispatch_pending (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
{
  if (DEBUG_LEVEL  > 5) ACE_DEBUG ( (LM_DEBUG,
    ACE_TEXT ("Consumer %d dispatching pending events.  Queue size: %d\n"),
    ACE_static_cast (int, this->proxy ()->id ()),
    this->pending_events_->size ()
    ));

  // lock ourselves in memory for the duration
  TAO_Notify_Refcountable_Guard_T<TAO_Notify_Consumer> self_grd (this);

  // dispatch events until: 1) the queue is empty; 2) the proxy shuts down, or 3) the dispatch fails
  ACE_GUARD (TAO_SYNCH_MUTEX, ace_mon, *this->proxy_lock ());
  bool ok = true;
  while (ok && !this->proxy_supplier ()->has_shutdown () && !this->pending_events_->is_empty ())
  {
    if (! dispatch_from_queue (*this->pending_events_, ace_mon))
    {
      this->schedule_timer (true);
      ok = false;
    }
  }
}


// virtual: this is the default, overridden for SequencePushConsumer
bool
TAO_Notify_Consumer::dispatch_from_queue (Request_Queue & requests, ACE_Guard <TAO_SYNCH_MUTEX> & ace_mon)
{
  bool result = true;
  TAO_Notify_Method_Request_Event_Queueable * request;
  if (requests.dequeue_head (request) == 0)
  {
    ace_mon.release ();
    DispatchStatus status = this->dispatch_request (request);
    switch (status)
    {
      case DISPATCH_SUCCESS:
      {
        request->complete ();
        request->release ();
        result = true;
        ace_mon.acquire ();
        break;
      }
      case DISPATCH_RETRY:
      {
        if (DEBUG_LEVEL  > 0) ACE_DEBUG ( (LM_DEBUG,
          ACE_TEXT ("(%P|%t) Consumer %d: Will retry %d\n"),
          ACE_static_cast (int, this->proxy ()->id ()),
          request->sequence ()
          ));
        ace_mon.acquire ();
        requests.enqueue_head (request); // put the failed event back where it was
        result = false;
        break;
      }
      case DISPATCH_DISCARD:
      {
        if (DEBUG_LEVEL  > 0) ACE_DEBUG ( (LM_DEBUG,
          ACE_TEXT ("(%P|%t) Consumer %d: Error during dispatch. Discarding event:%d.\n"),
          ACE_static_cast (int, this->proxy ()->id ()),
          request->sequence ()
          ));
        request->complete ();
        ace_mon.acquire ();
        result = true;
        break;
      }
      case DISPATCH_FAIL:
      {
        if (DEBUG_LEVEL  > 0) ACE_DEBUG ( (LM_DEBUG,
          ACE_TEXT ("(%P|%t) Consumer %d: Failed. Discarding event %d.\n"),
          ACE_static_cast (int, this->proxy ()->id ()),
          request->sequence ()
          ));
        request->complete ();
        ace_mon.acquire ();
        while (requests.dequeue_head (request) == 0)
        {
          ace_mon.release ();
          request->complete ();
          ace_mon.acquire ();
        }
        ace_mon.release ();
        ACE_TRY_NEW_ENV
        {
          this->proxy_supplier ()->destroy (ACE_ENV_SINGLE_ARG_PARAMETER);
          ACE_TRY_CHECK;
        }
        ACE_CATCHANY
        {
          // todo is there something reasonable to do here?
        }
        ACE_ENDTRY;
        ace_mon.acquire ();
        result = true;
        break;
      }
    }
  }
  return result;
}

//@@todo: rather than is_error, use pacing interval so it will be configurable
//@@todo: find some way to use batch buffering stratgy for sequence consumers.
void
TAO_Notify_Consumer::schedule_timer (bool is_error)
{
  if (this->timer_id_ != -1)
  {
    return; // We only want a single timeout scheduled.
  }
  // don't schedule timer if there's nothing that can be done
  if (this->is_suspended ())
  {
    return;
  }

  ACE_ASSERT (this->timer_ != 0);

  // If we're scheduling the timer due to an error then we want to
  // use the retry timeout, otherwise we'll assume that the pacing
  // interval is sufficient for now.
  ACE_Time_Value tv (DEFAULT_RETRY_TIMEOUT);

  if (! is_error)
  {
    if (this->pacing_.is_valid ())
    {
      tv = ORBSVCS_Time::to_Time_Value (this->pacing_.value ());
    }
  }

  if (DEBUG_LEVEL  > 5) ACE_DEBUG ( (LM_DEBUG,
    ACE_TEXT ("Consumer %d: scheduling pacing/retry for %dms.\n"),
      ACE_static_cast (int, this->proxy ()->id ()),
      tv.msec ()));

  this->timer_id_ = this->timer_->schedule_timer (
    this,
    tv,
    ACE_Time_Value::zero);
  if (this->timer_id_ == -1)
  {
    ACE_ERROR ( (LM_ERROR,
      ACE_TEXT ("TAO_Notify_Consumer %d::schedule_timer () Error scheduling timer.\n"),
      ACE_static_cast (int, this->proxy ()->id ())
      ));
  }
}

void
TAO_Notify_Consumer::cancel_timer (void)
{
  if (this->timer_ != 0 && this->timer_id_ != -1)
  {
    if (DEBUG_LEVEL  > 5) ACE_DEBUG ( (LM_DEBUG,
      ACE_TEXT ("Consumer %d canceling dispatch timer.\n"),
      static_cast<int> (this->proxy ()->id ())
      ));

    this->timer_->cancel_timer (timer_id_);
  }
  this->timer_id_ = -1;
}

int
TAO_Notify_Consumer::handle_timeout (const ACE_Time_Value&, const void*)
{
  TAO_Notify_Refcountable_Guard_T<TAO_Notify_Consumer> grd (this);
  this->timer_id_ = -1;  // This must come first, because dispatch_pending may try to resched
  ACE_TRY_NEW_ENV
    {
      this->dispatch_pending (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;
    }
  ACE_CATCHALL
    {
    }
  ACE_ENDTRY;

  return 0;
}

void
TAO_Notify_Consumer::shutdown (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
{
  if (this->timer_ == 0)
    {
      this->cancel_timer ();
      this->timer_->_decr_refcnt ();
      this->timer_ = 0;
    }
}


void
TAO_Notify_Consumer::dispatch_updates_i (const CosNotification::EventTypeSeq& added, const CosNotification::EventTypeSeq& removed
                                     ACE_ENV_ARG_DECL)
{
  if (!CORBA::is_nil (this->publish_.in ()))
    this->publish_->offer_change (added, removed ACE_ENV_ARG_PARAMETER);
}
