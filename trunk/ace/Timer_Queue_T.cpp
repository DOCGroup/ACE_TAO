// $Id$

#if !defined (ACE_TIMER_QUEUE_T_C)
#define ACE_TIMER_QUEUE_T_C

#define ACE_BUILD_DLL
#include "ace/Synch.h"
#include "ace/Signal.h"
#include "ace/Timer_Queue_T.h"

#if !defined (__ACE_INLINE__)
#include "ace/Timer_Queue_T.i"
#endif /* __ACE_INLINE__ */

template <class TYPE> void
ACE_Timer_Node_T<TYPE>::dump (void) const
{
  ACE_TRACE ("ACE_Timer_Node_T::dump");
  ACE_DEBUG ((LM_DEBUG, ACE_BEGIN_DUMP, this));
  ACE_DEBUG ((LM_DEBUG, "\nact_ = %x", this->act_));
  this->timer_value_.dump ();
  this->interval_.dump ();
  ACE_DEBUG ((LM_DEBUG, "\nprev_ = %x", this->prev_));
  ACE_DEBUG ((LM_DEBUG, "\nnext_ = %x", this->next_));
  ACE_DEBUG ((LM_DEBUG, "\ntimer_id_ = %d\n", this->timer_id_));
  ACE_DEBUG ((LM_DEBUG, ACE_END_DUMP));
}

template <class TYPE> 
ACE_Timer_Node_T<TYPE>::ACE_Timer_Node_T (void)
{
  ACE_TRACE ("ACE_Timer_Node_T::ACE_Timer_Node_T");
}

template <class TYPE, class FUNCTOR, class ACE_LOCK> 
ACE_Timer_Queue_Iterator_T<TYPE, FUNCTOR, ACE_LOCK>::ACE_Timer_Queue_Iterator_T (void)
{
}

template <class TYPE, class FUNCTOR, class ACE_LOCK> 
ACE_Timer_Queue_Iterator_T<TYPE, FUNCTOR, ACE_LOCK>::~ACE_Timer_Queue_Iterator_T (void)
{
}

// Determines the minimum amount of time that the Reactor must wait
// before timing out.  This is computed as the smaller of (1) the
// amount the caller requested when calling handle_events() and (2)
// the earliest time registered in the Timer Queue (if any).  Must be
// called with an external lock held since it returns a pointer to a
// Time_Value type stored in the Timer_Queue type itself.  If some
// external lock isn't held we'll have reentrancy problems!

template <class TYPE, class FUNCTOR, class ACE_LOCK> ACE_Time_Value *
ACE_Timer_Queue_T<TYPE, FUNCTOR, ACE_LOCK>::calculate_timeout (ACE_Time_Value *max_wait_time)
{
  ACE_TRACE ("ACE_Timer_Queue_T::calculate_timeout");
  ACE_MT (ACE_GUARD_RETURN (ACE_LOCK, ace_mon, this->mutex_, max_wait_time));
  
  if (this->is_empty ())
    // Nothing on the Timer_Queue, so use whatever the caller gave us.
    return max_wait_time; 
  else
    {
      ACE_Time_Value cur_time = this->gettimeofday ();

      if (this->earliest_time () > cur_time)
        {
	  // The earliest item on the Timer_Queue is still in the
	  // future.  Therefore, use the smaller of (1) caller's wait
	  // time or (2) the delta time between now and the earliest
	  // time on the Timer_Queue.

          this->timeout_ = this->earliest_time () - cur_time;
          if (max_wait_time == 0 || *max_wait_time > timeout_)
            return &this->timeout_;
	  else
	    return max_wait_time;
        }
      else 	  
        {
	  // The earliest item on the Timer_Queue is now in the past.
	  // Therefore, we've got to "poll" the Reactor, i.e., it must
	  // just check the descriptors and then dispatch timers, etc.
          this->timeout_ = ACE_Time_Value::zero;
	  return &this->timeout_;
        }
    }
}

template <class TYPE, class FUNCTOR, class ACE_LOCK> ACE_Time_Value *
ACE_Timer_Queue_T<TYPE, FUNCTOR, ACE_LOCK>::calculate_timeout (ACE_Time_Value *max_wait_time,
							   ACE_Time_Value *the_timeout)
{
  ACE_TRACE ("ACE_Timer_Queue_T::calculate_timeout");
  
  if (the_timeout == 0)
    return 0;

  if (this->is_empty ())
    {
      // Nothing on the Timer_Queue, so use whatever the caller gave us.
      if (max_wait_time)
        *the_timeout = *max_wait_time;
      else
        return 0;
    }
  else
    {
      ACE_Time_Value cur_time = this->gettimeofday ();

      if (this->earliest_time () > cur_time)
        {
	  // The earliest item on the Timer_Queue is still in the
	  // future.  Therefore, use the smaller of (1) caller's wait
	  // time or (2) the delta time between now and the earliest
	  // time on the Timer_Queue.

          *the_timeout = this->earliest_time () - cur_time;
          if (!(max_wait_time == 0 || *max_wait_time > *the_timeout))
	    *the_timeout = *max_wait_time;
        }
      else 	  
        {
	  // The earliest item on the Timer_Queue is now in the past.
	  // Therefore, we've got to "poll" the Reactor, i.e., it must
	  // just check the descriptors and then dispatch timers, etc.
	  *the_timeout = ACE_Time_Value::zero;
        }
    }
  return the_timeout;
}

template <class TYPE, class FUNCTOR, class ACE_LOCK> void
ACE_Timer_Queue_T<TYPE, FUNCTOR, ACE_LOCK>::dump (void) const
{
  ACE_TRACE ("ACE_Timer_Queue_T::dump");
  ACE_DEBUG ((LM_DEBUG, ACE_BEGIN_DUMP, this));
  this->timeout_.dump ();
  this->timer_skew_.dump ();
  ACE_DEBUG ((LM_DEBUG, ACE_END_DUMP));    
}

template <class TYPE, class FUNCTOR, class ACE_LOCK> 
ACE_Timer_Queue_T<TYPE, FUNCTOR, ACE_LOCK>::ACE_Timer_Queue_T (FUNCTOR *upcall_functor, 
                                                           ACE_Free_List<ACE_Timer_Node_T <TYPE> > *freelist)
  : free_list_ (freelist == 0 ? new ACE_Locked_Free_List<ACE_Timer_Node_T <TYPE>, ACE_Null_Mutex> : freelist),
    gettimeofday_ (ACE_OS::gettimeofday),
    upcall_functor_ (upcall_functor == 0 ? new FUNCTOR : upcall_functor),
    delete_upcall_functor_ (upcall_functor == 0),
    delete_free_list_ (freelist == 0),
    timer_skew_ (0, ACE_TIMER_SKEW)
{
  ACE_TRACE ("ACE_Timer_Queue_T::ACE_Timer_Queue_T");
}

template <class TYPE, class FUNCTOR, class ACE_LOCK> 
ACE_Timer_Queue_T<TYPE, FUNCTOR, ACE_LOCK>::~ACE_Timer_Queue_T (void)
{
  ACE_TRACE ("ACE_Timer_Queue_T::~ACE_Timer_Queue_T");

  // Cleanup the functor and free_list on the way out
  if (this->delete_upcall_functor_)
    delete this->upcall_functor_;

  if (this->delete_free_list_)
    delete this->free_list_;
}

template <class TYPE, class FUNCTOR, class ACE_LOCK> ACE_Timer_Node_T<TYPE> *
ACE_Timer_Queue_T<TYPE, FUNCTOR, ACE_LOCK>::alloc_node (void)
{
  return this->free_list_->remove ();
}

template <class TYPE, class FUNCTOR, class ACE_LOCK> void 
ACE_Timer_Queue_T<TYPE, FUNCTOR, ACE_LOCK>::free_node (ACE_Timer_Node_T<TYPE> *node)
{
  this->free_list_->add (node);
}

// Run the <handle_timeout> method for all Timers whose values are <=
// <cur_time>.

template <class TYPE, class FUNCTOR, class ACE_LOCK> int
ACE_Timer_Queue_T<TYPE, FUNCTOR, ACE_LOCK>::expire (const ACE_Time_Value &cur_time)
{
  ACE_TRACE ("ACE_Timer_Queue_T::expire");
  ACE_MT (ACE_GUARD_RETURN (ACE_LOCK, ace_mon, this->mutex_, -1));

  int number_of_timers_expired = 0;

  ACE_Timer_Node_T<TYPE> *expired;

  // Keep looping while there are timers remaining and the earliest
  // timer is <= the <cur_time> passed in to the method.

  if (this->is_empty ())
    return 0;

  while (this->earliest_time () <= cur_time)
    {
      expired = this->remove_first ();
      TYPE &type = expired->get_type ();
      const void *act = expired->get_act ();
      int reclaim = 1;
      
      // Check if this is an interval timer.
      if (expired->get_interval () > ACE_Time_Value::zero)
	{
	  // Make sure that we skip past values that have already
	  // "expired".
	  do
	    expired->set_timer_value (expired->get_timer_value () + expired->get_interval ());
	  while (expired->get_timer_value () <= cur_time);

	  // Since this is an interval timer, we need to reschedule
	  // it.
	  this->reschedule (expired);
	  reclaim = 0;
	}
      
      // call the functor
      this->upcall (type, act, cur_time);
      
      if (reclaim)
	// Call the factory method to free up the node.
	this->free_node (expired);
      
      number_of_timers_expired++;

      if (this->is_empty ())
        break;
    }

  return number_of_timers_expired;
}

template <class ACE_LOCK> int
ACE_Event_Handler_Handle_Timeout_Upcall<ACE_LOCK>::timeout (ACE_Timer_Queue_T<ACE_Event_Handler *, 
							    ACE_Event_Handler_Handle_Timeout_Upcall<ACE_LOCK>, 
							    ACE_LOCK> &timer_queue,
							    ACE_Event_Handler *handler,
							    const void *act,
							    const ACE_Time_Value &cur_time)
{
  // Upcall to the <handler>s handle_timeout method
  if (handler->handle_timeout (cur_time, act) == -1)
    timer_queue.cancel (handler, 0); // 0 means "call handle_close()".    
  
  return 0;
}

template <class ACE_LOCK> int
ACE_Event_Handler_Handle_Timeout_Upcall<ACE_LOCK>::cancellation (ACE_Timer_Queue_T<ACE_Event_Handler *, 
								 ACE_Event_Handler_Handle_Timeout_Upcall<ACE_LOCK>, 
								 ACE_LOCK> &timer_queue,
								 ACE_Event_Handler *handler)
{
  ACE_UNUSED_ARG (timer_queue);

  // Upcall to the <handler>s handle_close method
  handler->handle_close (ACE_INVALID_HANDLE, 
			 ACE_Event_Handler::TIMER_MASK);
  return 0;
}

template <class ACE_LOCK> int
ACE_Event_Handler_Handle_Timeout_Upcall<ACE_LOCK>::deletion (ACE_Timer_Queue_T<ACE_Event_Handler *, 
							     ACE_Event_Handler_Handle_Timeout_Upcall<ACE_LOCK>, 
							     ACE_LOCK> &timer_queue,
							     ACE_Event_Handler *handler,
							     const void *arg)
{
  ACE_UNUSED_ARG (timer_queue);
  ACE_UNUSED_ARG (handler);
  ACE_UNUSED_ARG (arg);

  // Does nothing
  
  return 0;
}

template <class TQ> TQ &
ACE_Async_Timer_Queue_Adapter<TQ>::timer_queue (void)
{
  return this->timer_queue_;
}

template <class TQ> int
ACE_Async_Timer_Queue_Adapter<TQ>::cancel (long timer_id,
					   const void **act)
{
  // Block all signals.
  ACE_Sig_Guard sg;
  ACE_UNUSED_ARG (sg);

  return this->timer_queue_.cancel (timer_id, act);
}

template <class TQ> long 
ACE_Async_Timer_Queue_Adapter<TQ>::schedule (ACE_Event_Handler *eh,
				 const void *act, 
				 const ACE_Time_Value &delay,
				 const ACE_Time_Value &interval)
{
  ACE_UNUSED_ARG (act);
  ACE_UNUSED_ARG (interval);

  // Block all signals.
  ACE_Sig_Guard sg;
  ACE_UNUSED_ARG (sg);

  long tid = this->timer_queue_.schedule (eh, 0, delay);

  if (tid == -1)
    ACE_ERROR_RETURN ((LM_ERROR, "%p\n", "schedule_timer"), -1);

  ACE_Time_Value tv = this->timer_queue_.earliest_time () 
    - ACE_OS::gettimeofday ();

  ACE_DEBUG ((LM_DEBUG,
	      "scheduling timer %d for (%d, %d)\n",
	      tid, tv.sec (), tv.usec ()));

  // Beware of negative times and zero times (which cause problems for
  // ualarm()).
  if (tv < ACE_Time_Value::zero)
    tv = ACE_Time_Value (0, 1);

  // Schedule a new timer.
  ACE_OS::ualarm (tv);
  return 0;
}

template <class TQ>
ACE_Async_Timer_Queue_Adapter<TQ>::ACE_Async_Timer_Queue_Adapter (void)
{
  // The following code is necessary to selectively "block" all
  // signals when SIGALRM is running.  Also, we always restart system
  // calls that are interrupted by the signals.

  // Block *all* signals when the SIGARLM handler is running!
  ACE_Sig_Set ss (1); 

  ACE_Sig_Action sa ((ACE_SignalHandler) 0,
		     ss,
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
	expired_timers = this->timer_queue_.expire ();

	if (expired_timers > 0)
	  ACE_DEBUG ((LM_DEBUG,
		      "time = %d, timers expired = %d\n", 
		      ACE_OS::time (),
		      expired_timers));

	// Only schedule a new timer if there is one in the list.
	if (this->timer_queue_.is_empty () == 0)
	  ACE_OS::ualarm (this->timer_queue_.earliest_time () 
			  - ACE_OS::gettimeofday ());
	
	return 0;
	/* NOTREACHED */
      }
    default:
      ACE_ERROR_RETURN ((LM_ERROR, "unexpected signal %S\n", signum), -1);
      /* NOTREACHED */
    }
}

#endif /* ACE_TIMER_QUEUE_T_C*/
