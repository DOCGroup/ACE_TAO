// $Id$

#include "ace/Timer_Queue_Adapters.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#ifndef ACE_TIMER_QUEUE_ADAPTERS_C
# define ACE_TIMER_QUEUE_ADAPTERS_C

ACE_RCSID(ace, Timer_Queue_Adapters, "$Id$")

# if !defined (__ACE_INLINE__)
#  include "ace/Timer_Queue_Adapters.i"
# endif /* __ACE_INLINE__ */

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
  // <ualarm>).
  if (tv < ACE_Time_Value::zero)
    tv = ACE_Time_Value (0, 1);

  // @@ This code should be clever enough to avoid updating the
  // <ualarm> if we haven't actually changed the earliest time.
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

  // @@ We still need to implement interval timers...
  long tid = this->timer_queue_.schedule (eh, act, delay);

  if (tid == -1)
    ACE_ERROR_RETURN ((LM_ERROR,
                       ACE_LIB_TEXT ("%p\n"),
                       ACE_LIB_TEXT ("schedule_timer")),
                      -1);

  if (this->schedule_ualarm () == -1)
    return 0;

  return tid;
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
    ACE_ERROR ((LM_ERROR,
                ACE_LIB_TEXT ("%p\n"),
                ACE_LIB_TEXT ("register_handler")));
}

// This is the signal handler function for the asynchronous timer
// list.  It gets invoked asynchronously when the SIGALRM signal
// occurs.

template <class TQ> int
ACE_Async_Timer_Queue_Adapter<TQ>::handle_signal (int signum,
                                                  siginfo_t *,
                                                  ucontext_t *)
{
  switch (signum)
    {
    case SIGALRM:
      {
        // Expire the pending timers.

        // @@ We need to figure out how to implement interval
        // timers...
        this->timer_queue_.expire ();

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
      ACE_ERROR_RETURN ((LM_ERROR,
                         "unexpected signal %S\n",
                         signum),
                        -1);
      /* NOTREACHED */
    }
}

template<class TQ>
ACE_Thread_Timer_Queue_Adapter<TQ>::ACE_Thread_Timer_Queue_Adapter (ACE_Thread_Manager *tm)
  : ACE_Task_Base (tm),
    condition_ (mutex_),
    active_ (1), // Assume that we start in active mode.
    thr_id_ (ACE_OS::NULL_thread)
{
}

template<class TQ> ACE_SYNCH_MUTEX &
ACE_Thread_Timer_Queue_Adapter<TQ>::mutex (void)
{
  return this->mutex_;
}

template<class TQ> long
ACE_Thread_Timer_Queue_Adapter<TQ>::schedule
    (ACE_Event_Handler* handler,
     const void *act,
     const ACE_Time_Value &delay,
     const ACE_Time_Value &interval)
{
  ACE_GUARD_RETURN (ACE_SYNCH_MUTEX, ace_mon, this->mutex_, -1);

  long result = this->timer_queue_.schedule (handler, act, delay, interval);
  this->condition_.signal ();
  return result;
}

template<class TQ> int
ACE_Thread_Timer_Queue_Adapter<TQ>::cancel (long timer_id,
                                            const void **act)
{
  ACE_GUARD_RETURN (ACE_SYNCH_MUTEX, ace_mon, this->mutex_, -1);

  int result = this->timer_queue_.cancel (timer_id, act);
  condition_.signal ();
  return result;
}

template<class TQ> void
ACE_Thread_Timer_Queue_Adapter<TQ>::deactivate (void)
{
  ACE_GUARD (ACE_SYNCH_MUTEX, ace_mon, this->mutex_);

  this->active_ = 0;
  this->condition_.signal ();
}

template<class TQ> int
ACE_Thread_Timer_Queue_Adapter<TQ>::svc (void)
{
  ACE_GUARD_RETURN (ACE_SYNCH_MUTEX, ace_mon, this->mutex_, -1);

  this->thr_id_ = ACE_Thread::self ();

  // Thread cancellation point, if ACE supports it.
  //
  // Note: This call generates a warning under Solaris because the header
  //       file /usr/include/pthread.h redefines the routine argument. This
  //       is a bug in the Solaris header files and has nothing to do with
  //       ACE.
# if !defined (ACE_LACKS_PTHREAD_CANCEL)
  ACE_PTHREAD_CLEANUP_PUSH (&this->condition_.mutex ());
# endif /* ACE_LACKS_PTHREAD_CANCEL */

  while (this->active_)
    {
# if defined (ACE_HAS_DEFERRED_TIMER_COMMANDS)
      // Temporarily suspend ownership of the timer queue mutex in
      // order to dispatch deferred execution commands.  These
      // commands are to be treated as executing in a context
      // "external" to the timer queue adapter, and thus must compete
      // separately for this lock.
      mutex_.release ();
      this->dispatch_commands ();

      // Re-acquire ownership of the timer queue mutex in order to
      // restore the "internal" timer queue adapter context
      mutex_.acquire ();
# endif /* ACE_HAS_DEFERRED_TIMER_COMMANDS */

      // If the queue is empty, sleep until there is a change on it.
      if (this->timer_queue_.is_empty ())
        this->condition_.wait ();
      else
        {
          // Compute the remaining time, being careful not to sleep
          // for "negative" amounts of time.
          ACE_Time_Value tv = this->timer_queue_.earliest_time ();

          // ACE_DEBUG ((LM_DEBUG,  ACE_LIB_TEXT ("waiting until %u.%3.3u secs\n"),
          // tv.sec(), tv.msec()));
          this->condition_.wait (&tv);
        }

      // Expire timers anyway, at worst this is a no-op.
      this->timer_queue_.expire ();
    }

   // Thread cancellation point, if ACE supports it.
# if !defined (ACE_LACKS_PTHREAD_CANCEL)
  ACE_PTHREAD_CLEANUP_POP (0);
# endif /* ACE_LACKS_PTHREAD_CANCEL */

  ACE_DEBUG ((LM_DEBUG, ACE_LIB_TEXT ("terminating dispatching thread\n")));
  return 0;
}


# if defined (ACE_HAS_DEFERRED_TIMER_COMMANDS)

// Enqueues a command object for execution just before waiting on the next
// timer event. This allows deferred execution of commands that cannot
// be performed in the timer event handler context, such as registering
// or cancelling timers on platforms where the timer queue mutex is not
// recursive.

template<class TQ> int
ACE_Thread_Timer_Queue_Adapter<TQ>::enqueue_command (ACE_Command_Base *cmd,
                                                     COMMAND_ENQUEUE_POSITION pos)
{
  // Serialize access to the command queue.
  ACE_GUARD_RETURN (ACE_SYNCH_MUTEX, ace_mon,
                    this->command_mutex_, -1);

  if (pos == ACE_Thread_Timer_Queue_Adapter<TQ>::TAIL)
    {
      return command_queue_.enqueue_tail (cmd);
    }
  else
    {
      return command_queue_.enqueue_head (cmd);
    }
}


// Dispatches all command objects enqueued in the most
// recent event handler context.

template<class TQ> int
ACE_Thread_Timer_Queue_Adapter<TQ>::dispatch_commands (void)
{
  // Serialize access to the command queue.
  ACE_GUARD_RETURN (ACE_SYNCH_MUTEX, ace_mon,
                    this->command_mutex_, -1);

  // loop through the enqueued commands
  ACE_Command_Base *cmd = 0;
  while (command_queue_.dequeue_head (cmd) == 0)
    if (cmd)
      {
        cmd->execute ();
        delete cmd;
      }

  return 0;
}

# endif /* ACE_HAS_DEFERRED_TIMER_COMMANDS */

#endif /* ACE_TIMER_QUEUE_ADAPTERS_C*/
