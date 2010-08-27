// $Id$

#include "orbsvcs/Notify/Consumer.h"

#if ! defined (__ACE_INLINE__)
#include "orbsvcs/Notify/Consumer.inl"
#endif /* __ACE_INLINE__ */

ACE_RCSID (RT_Notify, TAO_Notify_Consumer, "$Id$")

#include "orbsvcs/Notify/Timer.h"
#include "orbsvcs/Notify/ProxySupplier.h"
#include "orbsvcs/Notify/Method_Request_Event.h"
#include "orbsvcs/Notify/QoSProperties.h"
#include "orbsvcs/Notify/Properties.h"

#include "orbsvcs/Time_Utilities.h"

#include "tao/debug.h"
#include "tao/corba.h"
#include "tao/Messaging/Messaging_TypesC.h"

#include "ace/Bound_Ptr.h"
#include "ace/Unbounded_Queue.h"

#ifndef DEBUG_LEVEL
# define DEBUG_LEVEL TAO_debug_level
#endif //DEBUG_LEVEL

static const int DEFAULT_RETRY_TIMEOUT = 10;//120; // Note : This should be a config param or qos setting

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

TAO_Notify_Consumer::TAO_Notify_Consumer (TAO_Notify_ProxySupplier* proxy)
: proxy_ (proxy)
, is_suspended_ (0)
, have_not_yet_verified_publish_ (true)
, pacing_ (proxy->qos_properties_.pacing_interval ())
, max_batch_size_ (CosNotification::MaximumBatchSize, 0)
, timer_id_ (-1)
, timer_ (0)
{
  Request_Queue* pending_events = 0;
  ACE_NEW (pending_events, TAO_Notify_Consumer::Request_Queue ());
  this->pending_events_.reset( pending_events );

  this->timer_.reset( this->proxy ()->timer () );

  // Enable reference counting on the event handler.
  this->reference_counting_policy ().value (
    ACE_Event_Handler::Reference_Counting_Policy::ENABLED);
}

TAO_Notify_Consumer::~TAO_Notify_Consumer ()
{
  if (this->timer_.isSet())
  {
    this->cancel_timer ();
    this->timer_.reset ();
  }
}

CORBA::ULong
TAO_Notify_Consumer::_incr_refcnt (void)
{
  return this->add_reference();
}

CORBA::ULong
TAO_Notify_Consumer::_decr_refcnt (void)
{
  return this->remove_reference();
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
}

void
TAO_Notify_Consumer::resume (void)
{
  this->is_suspended_ = 0;

  this->dispatch_pending ();
}

void
TAO_Notify_Consumer::enqueue_request (
  TAO_Notify_Method_Request_Event * request)
{
  TAO_Notify_Event::Ptr event (
    request->event ()->queueable_copy ());

  TAO_Notify_Method_Request_Event_Queueable * queue_entry;
  ACE_NEW_THROW_EX (queue_entry,
    TAO_Notify_Method_Request_Event_Queueable (*request, event),
    CORBA::NO_MEMORY ());

  if (DEBUG_LEVEL  > 3) ACE_DEBUG ( (LM_DEBUG,
    ACE_TEXT ("Consumer %d: enqueue_request (%d) @%@.\n"),
    static_cast<int> (this->proxy ()->id ()),
    request->sequence (),
    request
    ));
  ACE_GUARD (TAO_SYNCH_MUTEX, ace_mon, *this->proxy_lock ());
  this->pending_events().enqueue_tail (queue_entry);
}

bool
TAO_Notify_Consumer::enqueue_if_necessary (TAO_Notify_Method_Request_Event * request)
{
  ACE_GUARD_RETURN (TAO_SYNCH_MUTEX, ace_mon, *this->proxy_lock (), false);
  if (! this->pending_events().is_empty ())
    {
      if (DEBUG_LEVEL > 3)
        ACE_DEBUG ((LM_DEBUG,
                    ACE_TEXT ("Consumer %d: enqueuing another event. %d\n"),
                    static_cast<int> (this->proxy ()->id ()),
                    request->sequence ()
                    ));
      TAO_Notify_Event::Ptr event (
        request->event ()->queueable_copy ());
      TAO_Notify_Method_Request_Event_Queueable * queue_entry;
      ACE_NEW_THROW_EX (queue_entry,
                        TAO_Notify_Method_Request_Event_Queueable (*request,
                                                                   event),
                        CORBA::NO_MEMORY ());
      this->pending_events().enqueue_tail (queue_entry);
      this->schedule_timer (false);
      return true;
    }
  if (this->is_suspended_ == 1)
    {
      if (DEBUG_LEVEL > 3)
        ACE_DEBUG ((LM_DEBUG,
                    ACE_TEXT ("Suspended Consumer %d enqueing event. %d\n"),
                    static_cast<int> (this->proxy ()->id ()),
                    request->sequence ()
                    ));
      TAO_Notify_Event::Ptr event (
        request->event ()->queueable_copy ());
      TAO_Notify_Method_Request_Event_Queueable * queue_entry;
      ACE_NEW_THROW_EX (queue_entry,
                        TAO_Notify_Method_Request_Event_Queueable (*request,
                                                                   event),
                        CORBA::NO_MEMORY ());
      this->pending_events().enqueue_tail (queue_entry);
      this->schedule_timer (false);
      return true;
    }
  return false;
}

void
TAO_Notify_Consumer::deliver (TAO_Notify_Method_Request_Event * request)
{
  // Increment reference counts (safely) to prevent this object and its proxy
  // from being deleted while the push is in progress.
  TAO_Notify_Proxy::Ptr proxy_guard (this->proxy ());
  bool queued = enqueue_if_necessary (request);
  if (!queued)
    {
      bool from_timeout = false;
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
            if (DEBUG_LEVEL > 1)
              ACE_DEBUG ((LM_DEBUG,
                          ACE_TEXT ("Consumer %d enqueing event %d due ")
                          ACE_TEXT ("to failed dispatch.\n"),
                          static_cast<int> (this->proxy ()->id ()),
                          request->sequence ()));
            this->enqueue_request (request);
            this->schedule_timer (true);
            break;
          }
        case DISPATCH_DISCARD:
          {
            if (DEBUG_LEVEL  > 0)
              ACE_DEBUG ((LM_DEBUG,
                          ACE_TEXT ("(%P|%t) Consumer %d: Error during ")
                          ACE_TEXT ("direct dispatch. Discarding event:%d.\n"),
                          static_cast<int> (this->proxy ()->id ()),
                          request->sequence ()
                          ));
            request->complete ();
            break;
          }
        case DISPATCH_FAIL_TIMEOUT:
          from_timeout = true;
          // Fall through
        case DISPATCH_FAIL:
          {
            if (DEBUG_LEVEL  > 0)
              ACE_DEBUG ((LM_DEBUG,
                          ACE_TEXT ("(%P|%t) Consumer %d: Failed during ")
                          ACE_TEXT ("direct dispatch :%d. Discarding event.\n"),
                          static_cast<int> (this->proxy ()->id ()),
                          request->sequence ()
                          ));
            request->complete ();
            try
              {
                this->proxy_supplier ()->destroy (from_timeout);
              }
            catch (const CORBA::Exception&)
              {
                // todo is there something meaningful we can do here?
                ;
              }
            break;
          }
        }
    }
}

TAO_Notify_Consumer::DispatchStatus
TAO_Notify_Consumer::dispatch_request (TAO_Notify_Method_Request_Event * request)
{
  DispatchStatus result = DISPATCH_SUCCESS;
  try
    {
      request->event ()->push (this);
      if (DEBUG_LEVEL  > 8)
        ACE_DEBUG ((LM_DEBUG,
                    ACE_TEXT ("Consumer %d dispatched single event %d.\n"),
                    static_cast<int> (this->proxy ()->id ()),
                    request->sequence ()
                    ));
    }
  catch (const CORBA::OBJECT_NOT_EXIST& ex)
    {
      if (DEBUG_LEVEL  > 0)
        {
          ACE_DEBUG ((LM_ERROR,
                      ACE_TEXT ("(%P|%t) TAO_Notify_Consumer %d::push ")
                      ACE_TEXT ("(request) %s\n"),
                      static_cast<int> (this->proxy ()->id ()),
                      ex._info ().c_str ()
                      ));
        }
      result = DISPATCH_FAIL;
    }
  catch (const CORBA::TRANSIENT& ex)
    {
      if (DEBUG_LEVEL  > 0)
        ACE_DEBUG ((LM_ERROR,
                    ACE_TEXT ("(%P|%t) TAO_Notify_Consumer %d::push ")
                    ACE_TEXT ("(request) Transient (minor=%d) %s\n"),
                    static_cast<int> (this->proxy ()->id ()),
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

        case TAO::VMCID:
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
  catch (const CORBA::TIMEOUT& ex)
    {
      if (DEBUG_LEVEL  > 0)
        ACE_DEBUG ((LM_ERROR,
                    ACE_TEXT ("(%P|%t) TAO_Notify_Consumer %u::push ")
                    ACE_TEXT ("(request) %s\n"),
                    this->proxy ()->id (),
                    ex._info().c_str ()
                    ));
      result = DISPATCH_FAIL_TIMEOUT;
    }
  catch (const CORBA::COMM_FAILURE& ex)
    {
      if (DEBUG_LEVEL  > 0)
        ACE_DEBUG ((LM_ERROR,
                    ACE_TEXT ("(%P|%t) TAO_Notify_Consumer %u::push ")
                    ACE_TEXT ("(request) %s\n"),
                    this->proxy ()->id (),
                    ex._info().c_str ()
                    ));
      result = DISPATCH_FAIL;
    }
  catch (const CORBA::SystemException& ex)
    {
      if (DEBUG_LEVEL  > 0)
        {
          ACE_DEBUG ((LM_ERROR,
                      ACE_TEXT ("(%P|%t) TAO_Notify_Consumer %d::push ")
                      ACE_TEXT ("(request) SystemException %s\n"),
                      static_cast<int> (this->proxy ()->id ()),
                      ex._info ().c_str ()
                      ));
        }
      result = DISPATCH_DISCARD;
    }
  catch (const CORBA::Exception&)
    {
      ACE_ERROR ( (LM_ERROR,
                   ACE_TEXT ("(%P|%t) TAO_Notify_Consumer %d::push ")
                   ACE_TEXT ("(request) Caught unexpected exception ")
                   ACE_TEXT ("pushing event to consumer.\n"),
                   static_cast<int> (this->proxy ()->id ())
                   ));
      result = DISPATCH_DISCARD;
    }

  // for persistent events that haven't timed out
  // convert "FAIL" & "DISCARD" to "RETRY"
  // for transient events, convert RETRY to DISCARD (hey, best_effort.)
  if (result == DISPATCH_FAIL ||
      result == DISPATCH_FAIL_TIMEOUT || result == DISPATCH_DISCARD)
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
  DispatchStatus result = DISPATCH_SUCCESS;
  try
    {
      this->push (batch);
    }
  catch (const CORBA::OBJECT_NOT_EXIST& ex)
    {
      if (DEBUG_LEVEL  > 0)
        ACE_DEBUG ((LM_ERROR,
                    ACE_TEXT ("(%P|%t) TAO_Notify_Consumer ")
                    ACE_TEXT ("%d::dispatch_batch() %s\n"),
                    static_cast<int> (this->proxy ()->id ()),
                    ex._info ().c_str ()
                    ));
      result = DISPATCH_FAIL;
    }
  catch (const CORBA::TRANSIENT& ex)
    {
      if (DEBUG_LEVEL  > 0)
        ACE_DEBUG ((LM_ERROR,
                    ACE_TEXT ("(%P|%t) TAO_Notify_Consumer ")
                    ACE_TEXT ("%d::dispatch_batch() Transient (minor=%d) %s\n"),
                    static_cast<int> (this->proxy ()->id ()),
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

        case TAO::VMCID:
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
  catch (const CORBA::TIMEOUT& ex)
    {
      if (DEBUG_LEVEL  > 0)
        ACE_DEBUG ((LM_ERROR,
                    ACE_TEXT ("(%P|%t) TAO_Notify_Consumer ")
                    ACE_TEXT ("%u::dispatch_batch() %s\n"),
                    this->proxy ()->id (),
                    ex._info().c_str ()
                    ));
      result = DISPATCH_FAIL_TIMEOUT;
    }
  catch (const CORBA::COMM_FAILURE& ex)
    {
      if (DEBUG_LEVEL  > 0)
        ACE_DEBUG ((LM_ERROR,
                    ACE_TEXT ("(%P|%t) TAO_Notify_Consumer ")
                    ACE_TEXT ("%u::dispatch_batch() %s\n"),
                    this->proxy ()->id (),
                    ex._info().c_str ()
                    ));
      result = DISPATCH_FAIL;
    }
  catch (const CORBA::SystemException& ex)
    {
      if (DEBUG_LEVEL  > 0)
        {
          ACE_DEBUG ((LM_ERROR,
                      ACE_TEXT ("(%P|%t) TAO_Notify_Consumer ")
                      ACE_TEXT ("%d::dispatch_batch() SystemException %s\n"),
                      static_cast<int>  (this->proxy ()->id ()),
                      ex._info ().c_str ()
                      ));
        }
      result = DISPATCH_DISCARD;
    }
  catch (const CORBA::Exception&)
    {
      ACE_ERROR ((LM_ERROR,
                  ACE_TEXT ("(%P|%t) TAO_Notify_Consumer ")
                  ACE_TEXT ("%d::dispatch_batch() Caught unexpected ")
                  ACE_TEXT ("exception pushing batch to consumer.\n"),
                  static_cast<int> (this->proxy ()->id ())
                  ));
      result = DISPATCH_DISCARD;
    }
  return result;
}

void
TAO_Notify_Consumer::dispatch_pending (void)
{
  if (DEBUG_LEVEL  > 5)
    ACE_DEBUG ((LM_DEBUG,
                ACE_TEXT ("Consumer %d dispatching pending events.  Queue size: %d\n"),
                static_cast<int> (this->proxy ()->id ()),
                this->pending_events().size ()
                ));

  // lock ourselves in memory for the duration
  TAO_Notify_Consumer::Ptr self_grd (this);

  // dispatch events until: 1) the queue is empty; 2) the proxy shuts down, or 3) the dispatch fails
  ACE_GUARD (TAO_SYNCH_MUTEX, ace_mon, *this->proxy_lock ());
  bool ok = true;
  while (ok
         && !this->proxy_supplier ()->has_shutdown ()
         && !this->pending_events().is_empty ())
    {
      if (! dispatch_from_queue ( this->pending_events(), ace_mon))
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
  TAO_Notify_Method_Request_Event_Queueable * request = 0;
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
            if (DEBUG_LEVEL  > 0)
              ACE_DEBUG ((LM_DEBUG,
                          ACE_TEXT ("(%P|%t) Consumer %d: Will retry %d\n"),
                          static_cast<int> (this->proxy ()->id ()),
                          request->sequence ()
                          ));
            ace_mon.acquire ();
            requests.enqueue_head (request); // put the failed event back where it was
            result = false;
            break;
          }
        case DISPATCH_DISCARD:
          {
            if (DEBUG_LEVEL  > 0)
              ACE_DEBUG ((LM_DEBUG,
                          ACE_TEXT ("(%P|%t) Consumer %d: Error during ")
                          ACE_TEXT ("dispatch. Discarding event:%d.\n"),
                          static_cast<int> (this->proxy ()->id ()),
                          request->sequence ()
                          ));
            request->complete ();
            ace_mon.acquire ();
            result = true;
            break;
          }
        case DISPATCH_FAIL:
          {
            if (DEBUG_LEVEL  > 0)
              ACE_DEBUG ((LM_DEBUG,
                          ACE_TEXT ("(%P|%t) Consumer %d: Failed. ")
                          ACE_TEXT ("Discarding event %d.\n"),
                          static_cast<int> (this->proxy ()->id ()),
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
            try
              {
                this->proxy_supplier ()->destroy ();
              }
            catch (const CORBA::Exception&)
              {
                // todo is there something reasonable to do here?
              }
            ace_mon.acquire ();
            result = true;
            break;
          }
        default:
          {
            ace_mon.acquire ();
            result = false;
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

  ACE_ASSERT (this->timer_.get() != 0);

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

  if (DEBUG_LEVEL  > 5)
    {
      ACE_DEBUG ((LM_DEBUG,
                  ACE_TEXT ("Consumer %d: scheduling pacing/retry for %dms.\n"),
                  static_cast<int> (this->proxy ()->id ()), tv.msec ()));
    }

  this->timer_id_ =
    this->timer_->schedule_timer (this, tv, ACE_Time_Value::zero);
  if (this->timer_id_ == -1)
    {
      ACE_ERROR ((LM_ERROR,
                  ACE_TEXT ("TAO_Notify_Consumer %d::schedule_timer () ")
                  ACE_TEXT ("Error scheduling timer.\n"),
                  static_cast<int> (this->proxy ()->id ())
                  ));
    }
  if (this->is_suspended()) // double check to avoid race
  {
    this->cancel_timer();
  }
}

void
TAO_Notify_Consumer::cancel_timer (void)
{
  if (this->timer_.isSet() && this->timer_id_ != -1)
    {
      if (DEBUG_LEVEL  > 5)
        ACE_DEBUG ((LM_DEBUG,
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
  if (!this->is_suspended() && this->timer_.isSet() && this->timer_id_ != -1)
    {
      TAO_Notify_Consumer::Ptr grd (this);
      this->timer_id_ = -1;  // This must come first, because dispatch_pending may try to resched
      try
        {
          this->dispatch_pending ();
        }
      catch (...)
        {
        }
    }

  return 0;
}

void
TAO_Notify_Consumer::shutdown (void)
{
  this->suspend();
  if (this->timer_.isSet ())
    {
      this->cancel_timer ();
      this->timer_.reset ();
    }
}

void
TAO_Notify_Consumer::dispatch_updates_i (const CosNotification::EventTypeSeq& added, const CosNotification::EventTypeSeq& removed)
{
  if (this->have_not_yet_verified_publish_)
    {
      this->have_not_yet_verified_publish_ = false; // no need to check again
      if (! this->publish_->_is_a ("IDL:omg.org/CosNotifyComm/NotifyPublish:1.0"))
        this->publish_ = CosNotifyComm::NotifyPublish::_nil();
    }
  if (! CORBA::is_nil (this->publish_.in ()))
    this->publish_->offer_change (added, removed);
}

TAO_SYNCH_MUTEX*
TAO_Notify_Consumer::proxy_lock (void)
{
  return &this->proxy_->lock_;
}

TAO_Notify_ProxySupplier*
TAO_Notify_Consumer::proxy_supplier (void)
{
  return this->proxy_;
}

void
TAO_Notify_Consumer::assume_pending_events (TAO_Notify_Consumer& rhs)
{
  // No need to lock the this proxy's lock.  It should have been locked
  // by the caller.

  // If the original consumer has pending events
  if (!rhs.pending_events ().is_empty ())
    {
      // We will take them away and cancel it's timer
      this->pending_events_.reset (rhs.pending_events_.release ());
      if (rhs.timer_.isSet ())
        {
          rhs.cancel_timer ();
        }

      // Schedule a new timer for us, which will use the default
      // timer value (unless we have a valid pacing interval).
      this->schedule_timer ();
    }
  if (this->is_suspended()) // double check to avoid race
  {
    this->cancel_timer();
  }
}

bool
TAO_Notify_Consumer::is_alive (bool allow_nil_consumer)
{
  bool status = false;
  CORBA::Object_var consumer = this->get_consumer ();
  if (CORBA::is_nil (consumer.in ()))
  {
    // The consumer may not connected or the consumer did
    // not provide a callback. In this case, the liveliness
    // check should return true so it will be validated in 
    // next period. 
    if (allow_nil_consumer)
      return true;
    else
      return status;
  }

  CORBA::PolicyList policy_list;
  try
    {
      bool do_liveliness_check = false;
      ACE_Time_Value now = ACE_OS::gettimeofday ();

      if (CORBA::is_nil (this->rtt_obj_.in ()))
      {
        // We need to determine if the consumer on the other end is still
        // alive.  Since we may be in an upcall from the owner of the
        // original consumer, we have to put a timeout on the call in case
        // the client side is not processing ORB requests at this time.  In
        // the event that the timeout exception occurs, we will assume that
        // the original consumer is still around.  If we get any other
        // exception we will say that the original consumer is not
        // available anymore.
        TimeBase::TimeT timeout = 10000000;
        CORBA::Any timeout_any;
        timeout_any <<= timeout;

        policy_list.length (1);
        policy_list[0] = TAO_Notify_PROPERTIES::instance()->orb()->
                          create_policy (
                                Messaging::RELATIVE_RT_TIMEOUT_POLICY_TYPE,
                                timeout_any);
        rtt_obj_ =
          consumer->_set_policy_overrides (policy_list,
                                          CORBA::ADD_OVERRIDE);

        // Clean up the policy that was allocated in the try/catch
        for (CORBA::ULong i = 0; i < policy_list.length (); i++)
          policy_list[i]->destroy ();

        do_liveliness_check 
          = (last_ping_ == ACE_Time_Value::zero ? true 
          : now - last_ping_.value () >= TAO_Notify_PROPERTIES::instance()->validate_client_delay ());
      }
      else
        do_liveliness_check = 
          now - last_ping_.value () >= TAO_Notify_PROPERTIES::instance()->validate_client_interval ();

      if (CORBA::is_nil (rtt_obj_.in ()))
        status = false;
      else if (do_liveliness_check || allow_nil_consumer)
      {
        last_ping_ = now;
        status = !rtt_obj_->_non_existent ();
      }
      else
        status = true;
    }
  catch (CORBA::TIMEOUT&)
    {
       status = true;
    }
  catch (CORBA::Exception& ex)
    {
      if (DEBUG_LEVEL > 0)
      {        
        ex._tao_print_exception ("TAO_Notify_Consumer::is_alive: false");
      }
    }

  return status;
}

TAO_END_VERSIONED_NAMESPACE_DECL
