// Timer_Queue_T.cpp
// $Id$

#if !defined (ACE_TIMER_QUEUE_T_C)
#define ACE_TIMER_QUEUE_T_C

#define ACE_BUILD_DLL
#include "ace/Timer_Queue_T.h"

#if !defined (__ACE_INLINE__)
#include "ace/Timer_Queue_T.i"
#endif /* __ACE_INLINE__ */

ACE_ALLOC_HOOK_DEFINE(ACE_Timer_Node_T)

template <class TYPE, class FUNCTOR> void
ACE_Timer_Node_T<TYPE, FUNCTOR>::dump (void) const
{
  ACE_TRACE ("ACE_Timer_Node::dump");
  ACE_DEBUG ((LM_DEBUG, ACE_BEGIN_DUMP, this));
  // ACE_DEBUG ((LM_DEBUG, "\type_ = %x", this->type_));
  ACE_DEBUG ((LM_DEBUG, "\nact_ = %x", this->act_));
  this->timer_value_.dump ();
  this->interval_.dump ();
  ACE_DEBUG ((LM_DEBUG, "\nnext_ = %x", this->next_));
  ACE_DEBUG ((LM_DEBUG, "\ntimer_id_ = %d", this->timer_id_));
  ACE_DEBUG ((LM_DEBUG, ACE_END_DUMP));
}

template <class TYPE, class FUNCTOR> 
ACE_Timer_Node_T<TYPE, FUNCTOR>::ACE_Timer_Node_T (void)
{
  ACE_TRACE ("ACE_Timer_Node::ACE_Timer_Node");
}

template <class TYPE, class FUNCTOR> 
ACE_Timer_Node_T<TYPE, FUNCTOR>::ACE_Timer_Node_T (const TYPE &type, 
						   const void *a, 
						   const ACE_Time_Value &t, 
						   const ACE_Time_Value &i, 
						   ACE_Timer_Node_T<TYPE, FUNCTOR> *n,
						   int timer_id)
  : type_ (type), 
    act_ (a), 
    timer_value_ (t), 
    interval_ (i), 
    next_ (n),
    timer_id_ (timer_id)
{
  ACE_TRACE ("ACE_Timer_Node::ACE_Timer_Node");
}

template <class TYPE, class FUNCTOR> 
ACE_Timer_Queue_Iterator_T<TYPE, FUNCTOR>::ACE_Timer_Queue_Iterator_T (void)
{
}

template <class TYPE, class FUNCTOR> 
ACE_Timer_Queue_Iterator_T<TYPE, FUNCTOR>::~ACE_Timer_Queue_Iterator_T (void)
{
}

// Determines the minimum amount of time that the Reactor must wait
// before timing out.  This is computed as the smaller of (1) the
// amount the caller requested when calling handle_events() and (2)
// the earliest time registered in the Timer Queue (if any).  Must be
// called with an external lock held since it returns a pointer to a
// Time_Value type stored in the Timer_Queue type itself.  If some
// external lock isn't held we'll have reentrancy problems!

template <class TYPE, class FUNCTOR> ACE_Time_Value *
ACE_Timer_Queue_T<TYPE, FUNCTOR>::calculate_timeout (ACE_Time_Value *max_wait_time)
{
  ACE_TRACE ("ACE_Timer_List::calculate_timeout");
  ACE_MT (ACE_GUARD_RETURN (ACE_Recursive_Thread_Mutex, ace_mon, this->lock_, max_wait_time));
  
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

template <class TYPE, class FUNCTOR> void
ACE_Timer_Queue_T<TYPE, FUNCTOR>::dump (void) const
{
  ACE_TRACE ("ACE_Timer_Queue::dump");
  ACE_DEBUG ((LM_DEBUG, ACE_BEGIN_DUMP, this));
  this->timeout_.dump ();
  this->timer_skew_.dump ();
  ACE_DEBUG ((LM_DEBUG, ACE_END_DUMP));    
}

template <class TYPE, class FUNCTOR> 
ACE_Timer_Queue_T<TYPE, FUNCTOR>::ACE_Timer_Queue_T (FUNCTOR *upcall_functor)
  : gettimeofday_ (ACE_OS::gettimeofday),
    upcall_functor_ (upcall_functor == 0 ? *(new FUNCTOR) : *upcall_functor),
    delete_upcall_functor_ (upcall_functor == 0),
    timer_skew_ (0, ACE_TIMER_SKEW)
{
  ACE_TRACE ("ACE_Timer_Queue::ACE_Timer_Queue");
}

template <class TYPE, class FUNCTOR> 
ACE_Timer_Queue_T<TYPE, FUNCTOR>::~ACE_Timer_Queue_T (void)
{
  // Cleanup the functor on the way out
  if (delete_upcall_functor_)
    delete &this->upcall_functor_;

  ACE_TRACE ("ACE_Timer_Queue::~ACE_Timer_Queue");
}

// Run the <handle_timeout> method for all Timers whose values are <=
// <cur_time>.

template <class TYPE, class FUNCTOR> int
ACE_Timer_Queue_T<TYPE, FUNCTOR>::expire (const ACE_Time_Value &cur_time)
{
  ACE_TRACE ("ACE_Timer_List::expire");
  ACE_MT (ACE_GUARD_RETURN (ACE_Recursive_Thread_Mutex, ace_mon, this->lock_, -1));

  int number_of_timers_expired = 0;

  ITERATOR &iter = this->iter ();

  // Keep looping while there are timers remaining and the earliest
  // timer is <= the <cur_time> passed in to the method.

  for (NODE *expired;
       iter.next (expired, cur_time) != 0;
       )
    {
      TYPE &type = expired->type_;
      const void *act = expired->act_;
      int reclaim = 1;
      
      // Check if this is an interval timer.
      if (expired->interval_ > ACE_Time_Value::zero)
	{
	  // Make sure that we skip past values that have already
	  // "expired".
	  do
	    expired->timer_value_ += expired->interval_;
	  while (expired->timer_value_ <= cur_time);

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
    }

  return number_of_timers_expired;
}

template <class TYPE, class FUNCTOR> void
ACE_Timer_Queue_T<TYPE, FUNCTOR>::upcall (TYPE &type,
					  const void *act,
					  const ACE_Time_Value &cur_time)
{
  // This calls the correct operator () on the functor (the one with
  // four args)
  this->upcall_functor_.operator () (*this, type, act, cur_time);
}


template <class TYPE, class FUNCTOR> ACE_Time_Value
ACE_Timer_Queue_T<TYPE, FUNCTOR>::gettimeofday (void)
{
  // Invoke gettimeofday via pointer to function.
  return gettimeofday_ ();
}

template <class TYPE, class FUNCTOR> void
ACE_Timer_Queue_T<TYPE, FUNCTOR>::gettimeofday (ACE_Time_Value (*gettimeofday)(void))
{
  gettimeofday_ = gettimeofday;
}

#if defined (ACE_MT_SAFE)

template <class TYPE, class FUNCTOR> ACE_Recursive_Thread_Mutex &
ACE_Timer_Queue_T<TYPE, FUNCTOR>::lock (void)
{
  return this->lock_;
}

#endif /* ACE_MT_SAFE */

template <class TYPE, class FUNCTOR> FUNCTOR &
ACE_Timer_Queue_T<TYPE, FUNCTOR>::upcall_functor (void)
{
  return this->upcall_functor_;
}

#endif /* ACE_TIMER_QUEUE_T_C*/
