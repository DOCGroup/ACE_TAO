// $Id$

#include "ace/Timer_Queue_Adapters.h"

#if !defined (ACE_TIMER_QUEUE_ADAPTERS_C)
#define ACE_TIMER_QUEUE_ADAPTERS_C

#define ACE_BUILD_DLL
#include "ace/Timer_Queue_Adapters.h"

#if !defined (__ACE_INLINE__)
#include "ace/Timer_Queue_Adapters.i"
#endif /* __ACE_INLINE__ */

template <class TQ> TQ &
ACE_Async_Timer_Queue_Adapter<TQ>::timer_queue (void)
{
  return this->timer_queue_;
}

template <class TQ> int
ACE_Async_Timer_Queue_Adapter<TQ>::cancel (long timer_id,
                                           const void **act)
{
  // Block designated signals.
  ACE_Sig_Guard sg (&this->mask_);
  ACE_UNUSED_ARG (sg);

  return this->timer_queue_.cancel (timer_id, act);
}

template <class TQ> int
ACE_Async_Timer_Queue_Adapter<TQ>::expire (void)
{
  // Block designated signals.
  ACE_Sig_Guard sg (&this->mask_);
  ACE_UNUSED_ARG (sg);

  return this->timer_queue_.expire ();
}

template <class TQ> int
ACE_Async_Timer_Queue_Adapter<TQ>::schedule_ualarm (void)
{
  ACE_Time_Value tv = this->timer_queue_.earliest_time () 
    - ACE_OS::gettimeofday ();

  // Beware of negative times and zero times (which cause problems for
  // ualarm()).
  if (tv < ACE_Time_Value::zero)
    tv = ACE_Time_Value (0, 1);

  // @@ This code should be clever enough to avoid updating the
  // ualarm() if we haven't actually changed the earliest time.
  // Schedule a new timer.
  ACE_OS::ualarm (tv);
  return 0;
}

template <class TQ> long 
ACE_Async_Timer_Queue_Adapter<TQ>::schedule (ACE_Event_Handler *eh,
                                             const void *act, 
                                             const ACE_Time_Value &delay,
                                             const ACE_Time_Value &interval)
{
  ACE_UNUSED_ARG (act);
  ACE_UNUSED_ARG (interval);

  // Block designated signals.
  ACE_Sig_Guard sg (&this->mask_);
  ACE_UNUSED_ARG (sg);

  // @@ We need to figure out how to implement interval timers...
  long tid = this->timer_queue_.schedule (eh, 0, delay);

  if (tid == -1)
    ACE_ERROR_RETURN ((LM_ERROR, "%p\n", "schedule_timer"), -1);

  return this->schedule_ualarm ();
}

template <class TQ>
ACE_Async_Timer_Queue_Adapter<TQ>::ACE_Async_Timer_Queue_Adapter (ACE_Sig_Set *mask)
  // If <mask> == 0, block *all* signals when the SIGARLM handler is
  // running, else just block those in the mask.
  : mask_ (mask)
{
  // The following code is necessary to selectively "block" certain
  // signals when SIGALRM is running.  Also, we always restart system
  // calls that are interrupted by the signals.

  ACE_Sig_Action sa ((ACE_SignalHandler) 0,
                     this->mask_,
                     SA_RESTART);

  if (this->sig_handler_.register_handler (SIGALRM, this, &sa) == -1)
    ACE_ERROR ((LM_ERROR, "%p\n", "register_handler"));
}

// This is the signal handler function for the asynchronous timer
// list.  It gets invoked asynchronously when the SIGALRM signal
// occurs.

template <class TQ> int
ACE_Async_Timer_Queue_Adapter<TQ>::handle_signal (int signum,
                                                  siginfo_t *,
                                                  ucontext_t *)
{
  ACE_DEBUG ((LM_DEBUG, "handling signal %S\n", signum));

  switch (signum)
    {
    case SIGALRM:
      {
        int expired_timers;

        // Expire the pending timers.  
	// @@ We need to figure out how to implement interval timers...
        expired_timers = this->timer_queue_.expire ();

        if (expired_timers > 0)
          ACE_DEBUG ((LM_DEBUG,
                      "time = %d, timers expired = %d\n", 
                      ACE_OS::time (),
                      expired_timers));

        // Only schedule a new timer if there is one in the list.
        // @@ This code should also become smarter to avoid
        // unnecessary calls to ualarm().
        if (this->timer_queue_.is_empty () == 0)
          return this->schedule_ualarm ();
        else
          return 0;
        /* NOTREACHED */
      }
    default:
      ACE_ERROR_RETURN ((LM_ERROR, "unexpected signal %S\n", signum), -1);
      /* NOTREACHED */
    }
}

template<class TQ> ACE_SYNCH_MUTEX &
ACE_Thread_Timer_Queue_Adapter<TQ>::lock (void)
{
  return this->lock_;
}

template<class TQ> long 
ACE_Thread_Timer_Queue_Adapter<TQ>::schedule
    (ACE_Event_Handler* handler,
     const void *act,
     const ACE_Time_Value &delay,
     const ACE_Time_Value &interval)
{
  ACE_GUARD_RETURN (ACE_SYNCH_MUTEX, ace_mon, this->lock_, -1);

  long result = this->timer_queue_.schedule (handler, act, delay, interval);
  this->condition_.signal ();
  return result;
}

template<class TQ> int 
ACE_Thread_Timer_Queue_Adapter<TQ>::cancel (long timer_id,
					    const void **act)
{
  ACE_GUARD_RETURN (ACE_SYNCH_MUTEX, ace_mon, this->lock_, -1);

  int result = this->timer_queue_.cancel (timer_id, act);
  condition_.signal ();
  return result;
}

template<class TQ> void 
ACE_Thread_Timer_Queue_Adapter<TQ>::deactivate (void)
{
  ACE_GUARD (ACE_SYNCH_MUTEX, ace_mon, this->lock_);

  this->active_ = 0;
  this->condition_.signal ();
}

template<class TQ> int 
ACE_Thread_Timer_Queue_Adapter<TQ>::svc (void)
{
  ACE_GUARD_RETURN (ACE_SYNCH_MUTEX, ace_mon, this->lock_, -1);

  while (this->active_)
    {
      // If the queue is empty, sleep until there is a change on it.
      if (this->timer_queue_.is_empty ())
	this->condition_.wait ();
      else
	{
	  // Compute the remaining time, being careful not to sleep
	  // for "negative" amounts of time.
	  ACE_Time_Value tv = this->timer_queue_.earliest_time ();

	  // ACE_DEBUG ((LM_DEBUG, "waiting until %u.%3.3u secs\n",
	  // tv.sec(), tv.msec()));
	  this->condition_.wait (&tv);
	}

      // Expire timers anyway, at worst this is a no-op.
      this->timer_queue_.expire ();
    }

  ACE_DEBUG ((LM_DEBUG, "terminating dispatching thread\n"));
  return 0;
}

#endif /* ACE_TIMER_QUEUE_ADAPTERS_C*/
