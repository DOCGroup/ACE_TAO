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

ACE_ALLOC_HOOK_DEFINE(ACE_Timer_Queue)

void
ACE_Timer_Queue::dump (void) const
{
  ACE_TRACE ("ACE_Timer_Queue::dump");
  ACE_DEBUG ((LM_DEBUG, ACE_BEGIN_DUMP, this));
  this->timer_skew_.dump ();
  ACE_DEBUG ((LM_DEBUG, ACE_END_DUMP));    
}

// Create an empty queue.

ACE_Timer_Queue::ACE_Timer_Queue (void)
  : head_ (0),
    timer_id_ (0),
    timer_skew_ (0, ACE_TIMER_SKEW)
{
  ACE_TRACE ("ACE_Timer_Queue::ACE_Timer_Queue");
}

// Checks if queue is empty. 

int 
ACE_Timer_Queue::is_empty (void) const
{
  ACE_TRACE ("ACE_Timer_Queue::is_empty");
  return this->head_ == 0;
}

// Returns earliest time in a non-empty queue. 

const ACE_Time_Value &
ACE_Timer_Queue::earliest_time (void) const
{
  ACE_TRACE ("ACE_Timer_Queue::earliest_time");
  return this->head_->timer_value_;
}

// Remove all remaining items in the queue. 

ACE_Timer_Queue::~ACE_Timer_Queue (void)
{
  ACE_TRACE ("ACE_Timer_Queue::~ACE_Timer_Queue");
  ACE_MT (ACE_GUARD (ACE_Recursive_Thread_Mutex, ace_mon, this->lock_));
  
  ACE_Timer_Node *curr = this->head_;
  
  while (curr != 0)
    {
      ACE_Timer_Node *next = curr->next_;
      delete curr;
      curr = next;
    }
}

// Reschedule a periodic timer.  This function must be called with the
// mutex lock held.

void 
ACE_Timer_Queue::reschedule (ACE_Timer_Node *expired)
{
  ACE_TRACE ("ACE_Timer_Queue::reschedule");
  if (this->is_empty () || expired->timer_value_ < this->earliest_time ())
    {
      expired->next_ = this->head_;
      this->head_ = expired;
    }
  else
    {
      ACE_Timer_Node *prev = this->head_;
      ACE_Timer_Node *after  = this->head_->next_;

      // Locate the proper position in the queue. 

      while (after != 0 
	     && expired->timer_value_ > after->timer_value_)
	{
	  prev = after;
	  after  = after->next_;
	}

      expired->next_ = after;
      prev->next_  = expired;
    }
}

// Insert a new handler that expires at time future_time; if interval
// is > 0, the handler will be reinvoked periodically.

int
ACE_Timer_Queue::schedule (ACE_Event_Handler *handler,
			   const void *arg,
			   const ACE_Time_Value	&future_time, 
			   const ACE_Time_Value	&interval)
{
  ACE_TRACE ("ACE_Timer_Queue::schedule");
  ACE_MT (ACE_GUARD_RETURN (ACE_Recursive_Thread_Mutex, ace_mon, this->lock_, -1));

  // Increment the sequence number (it will wrap around).
  this->timer_id_++;

  if (this->is_empty () || future_time < this->earliest_time ())
    {
      // Place at the beginning of the list.
      ACE_NEW_RETURN (this->head_, 
		      ACE_Timer_Node (handler, 
				      arg, 
				      future_time,
				      interval, 
				      this->head_,
				      this->timer_id_),
		      -1);
      return this->head_ ? this->timer_id_ : -1;
    }
  else // Place in the middle of the list somewhere.
    {
      ACE_Timer_Node *prev = this->head_;
      ACE_Timer_Node *after = this->head_->next_;

      while (after != 0 && future_time > after->timer_value_)
	{
	  prev = after;
	  after = after->next_;
	}

      ACE_NEW_RETURN (prev->next_,
		      ACE_Timer_Node (handler, 
				      arg, 
				      future_time,
				      interval, 
				      after,
				      this->timer_id_),
		      -1);
      return prev->next_ ? this->timer_id_ : -1;
    }
}

// Locate and remove the single <ACE_Event_Handler> with a value of
// <timer_id> from the timer queue.

int
ACE_Timer_Queue::cancel (int timer_id, const void **arg)
{
  ACE_TRACE ("ACE_Timer_Queue::cancel");
  ACE_MT (ACE_GUARD_RETURN (ACE_Recursive_Thread_Mutex, ace_mon, this->lock_, -1));

  ACE_Timer_Node *prev = 0;
  ACE_Timer_Node *curr = 0;

  // Try to locate the ACE_Timer_Node that matches the timer_id.

  for (curr = this->head_; 
       curr != 0 && curr->timer_id_ != timer_id;
       curr = curr->next_)
    prev = curr;

  if (curr != 0)
    {
      if (prev == 0)
	this->head_ = curr->next_;
      else
	prev->next_ = curr->next_;

      if (arg != 0)
	*arg = curr->arg_;

      delete curr;
      return 0;
    }
  else
    return -1;
}

// Locate and remove all values of <handler> from the timer queue.

int
ACE_Timer_Queue::cancel (ACE_Event_Handler *handler)
{
  ACE_TRACE ("ACE_Timer_Queue::cancel");
  ACE_MT (ACE_GUARD_RETURN (ACE_Recursive_Thread_Mutex, ace_mon, this->lock_, -1));

  ACE_Timer_Node *prev = 0;
  ACE_Timer_Node *curr = this->head_;

  while (curr != 0)
    {
      if (curr->handler_ == handler)
	{
	  if (prev == 0)
	    {
	      this->head_ = curr->next_;
	      delete curr;
	      curr = this->head_;
	    }
	  else
	    {
	      prev->next_ = curr->next_;
	      delete curr;
	      curr = prev->next_;
	    }
	}
      else
	{
	  prev = curr;
	  curr = curr->next_;
	}
    }

  return 0;
}

// Run the <handle_timeout> method for all Timers whose values are <=
// <cur_time>.

int
ACE_Timer_Queue::expire (const ACE_Time_Value &cur_time)
{
  ACE_TRACE ("ACE_Timer_Queue::expire");
  ACE_MT (ACE_GUARD_RETURN (ACE_Recursive_Thread_Mutex, ace_mon, this->lock_, -1));

  int number_of_timers_expired = 0;

  // Keep looping while there are timers remaining and the earliest
  // timer is <= the <cur_time> passed in to the method.

  for (; 
       this->is_empty () == 0 && this->earliest_time () <= cur_time;
       number_of_timers_expired++)
    {
      ACE_Timer_Node *expired = this->head_;
      ACE_Event_Handler *handler = 
	(ACE_Event_Handler *) expired->handler_;
      const void *arg = expired->arg_;
      int reclaim = 1;

      this->head_ = this->head_->next_;

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
	this->cancel (handler);

      if (reclaim)
	delete expired;
    }

  return number_of_timers_expired;
}

// Determines the maximum amount of time that the Reactor must wait
// before timing out.  This is computed as the smaller of (1) the
// amount the caller requested when calling handle_events() and (2)
// the earliest time registered in the Timer Queue (if any).  Must be
// called with locks held since it returns a pointer to a Time_Value
// object stored in the Timer_Queue object itself.  If the lock isn't
// held we'll have reentrancy problems!)

ACE_Time_Value *
ACE_Timer_Queue::calculate_timeout (ACE_Time_Value *max_wait_time)
{
  ACE_TRACE ("ACE_Timer_Queue::calculate_timeout");
  ACE_MT (ACE_GUARD_RETURN (ACE_Recursive_Thread_Mutex, ace_mon, this->lock_, max_wait_time));

  if (this->is_empty ())
    // Nothing on the Timer_Queue, so use whatever the caller gave us.
    return max_wait_time; 
  else
    {
      ACE_Time_Value cur_time = ACE_OS::gettimeofday ();

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
