// Timer_Queue.cpp
// $Id$

#define ACE_BUILD_DLL
#include "ace/Timer_Queue.h"

#if !defined (__ACE_INLINE__)
#include "ace/Timer_Queue.i"
#endif /* __ACE_INLINE__ */

ACE_ALLOC_HOOK_DEFINE(ACE_Timer_Node)

void
ACE_Timer_Node::dump (void) const
{
  ACE_TRACE ("ACE_Timer_Node::dump");
  ACE_DEBUG ((LM_DEBUG, ACE_BEGIN_DUMP, this));
  ACE_DEBUG ((LM_DEBUG, "\nhandler_ = %x", this->handler_));
  ACE_DEBUG ((LM_DEBUG, "\narg_ = %x", this->arg_));
  this->timer_value_.dump ();
  this->interval_.dump ();
  ACE_DEBUG ((LM_DEBUG, "\nnext_ = %x", this->next_));
  ACE_DEBUG ((LM_DEBUG, "\ntimer_id_ = %d", this->timer_id_));
  ACE_DEBUG ((LM_DEBUG, ACE_END_DUMP));
}

ACE_Timer_Node::ACE_Timer_Node (void)
{
  ACE_TRACE ("ACE_Timer_Node::ACE_Timer_Node");
}

ACE_Timer_Node::ACE_Timer_Node (ACE_Event_Handler *h, 
				const void *a, 
				const ACE_Time_Value &t, 
				const ACE_Time_Value &i, 
				ACE_Timer_Node *n,
				int timer_id)
  : handler_ (h), 
    arg_ (a), 
    timer_value_ (t), 
    interval_ (i), 
    next_ (n),
    timer_id_ (timer_id)
{
  ACE_TRACE ("ACE_Timer_Node::ACE_Timer_Node");
}

ACE_Timer_Queue_Iterator::ACE_Timer_Queue_Iterator (void)
{
}

ACE_Timer_Queue_Iterator::~ACE_Timer_Queue_Iterator (void)
{
}

// Determines the minimum amount of time that the Reactor must wait
// before timing out.  This is computed as the smaller of (1) the
// amount the caller requested when calling handle_events() and (2)
// the earliest time registered in the Timer Queue (if any).  Must be
// called with an external lock held since it returns a pointer to a
// Time_Value object stored in the Timer_Queue object itself.  If some
// external lock isn't held we'll have reentrancy problems!

ACE_Time_Value *
ACE_Timer_Queue::calculate_timeout (ACE_Time_Value *max_wait_time)
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

void
ACE_Timer_Queue::dump (void) const
{
  ACE_TRACE ("ACE_Timer_Queue::dump");
  ACE_DEBUG ((LM_DEBUG, ACE_BEGIN_DUMP, this));
  this->timeout_.dump ();
  this->timer_skew_.dump ();
  ACE_DEBUG ((LM_DEBUG, ACE_END_DUMP));    
}

ACE_Timer_Queue::ACE_Timer_Queue (void)
  : gettimeofday_ (ACE_OS::gettimeofday),
    timer_skew_ (0, ACE_TIMER_SKEW)
{
  ACE_TRACE ("ACE_Timer_Queue::ACE_Timer_Queue");
}

ACE_Timer_Queue::~ACE_Timer_Queue (void)
{
  ACE_TRACE ("ACE_Timer_Queue::~ACE_Timer_Queue");
}

// Run the <handle_timeout> method for all Timers whose values are <=
// <cur_time>.

int
ACE_Timer_Queue::expire (const ACE_Time_Value &cur_time)
{
  ACE_TRACE ("ACE_Timer_List::expire");
  ACE_MT (ACE_GUARD_RETURN (ACE_Recursive_Thread_Mutex, ace_mon, this->lock_, -1));

  int number_of_timers_expired = 0;

  ACE_Timer_Queue_Iterator &iter = this->iter ();

  // Keep looping while there are timers remaining and the earliest
  // timer is <= the <cur_time> passed in to the method.

  for (ACE_Timer_Node *expired;
       iter.next (expired, cur_time) != 0;
       )
    {
      ACE_Event_Handler *handler = 
	(ACE_Event_Handler *) expired->handler_;
      const void *arg = expired->arg_;
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

      // Perform the callback.
      if (handler->handle_timeout (cur_time, arg) == -1)
	this->cancel (handler, 0); // 0 means "call handle_close()".

      if (reclaim)
	// Call the factory method to free up the node.
	this->free_node (expired);

      number_of_timers_expired++;
    }

  return number_of_timers_expired;
}

ACE_Time_Value
ACE_Timer_Queue::gettimeofday (void)
{
  // Invoke gettimeofday via pointer to function.
  return gettimeofday_ ();
}

void
ACE_Timer_Queue::gettimeofday (ACE_Time_Value (*gettimeofday)(void))
{
  gettimeofday_ = gettimeofday;
}

