// Timer_List.cpp
// $Id$

#define ACE_BUILD_DLL
#include "ace/Timer_List.h"

ACE_Timer_List_Iterator::ACE_Timer_List_Iterator (ACE_Timer_List &list)
  : timer_list_ (list)
{
}

int
ACE_Timer_List_Iterator::next (ACE_Timer_Node *&node,
			       const ACE_Time_Value &cur_time)
{
  if (this->timer_list_.head_ == 0 
      || this->timer_list_.head_->timer_value_ > cur_time)
    return 0;
  else
    {
      node = this->timer_list_.head_;
      this->timer_list_.head_ = this->timer_list_.head_->next_;
      return 1;
    }
}

ACE_ALLOC_HOOK_DEFINE(ACE_Timer_List)

ACE_Timer_Queue_Iterator &
ACE_Timer_List::get_iterator (void)
{
  return this->iterator_;
}

// Create an empty list.

ACE_Timer_List::ACE_Timer_List (void)
  : head_ (0),
    iterator_ (*this)
{
  ACE_TRACE ("ACE_Timer_List::ACE_Timer_List");
}

// Checks if list is empty.

int 
ACE_Timer_List::is_empty (void) const
{
  ACE_TRACE ("ACE_Timer_List::is_empty");
  return this->head_ == 0;
}

ACE_Timer_Node *
ACE_Timer_List::alloc_node (void)
{
  return new ACE_Timer_Node;
}

void
ACE_Timer_List::free_node (ACE_Timer_Node *node)
{
  delete node;
}

// Returns earliest time in a non-empty list.

const ACE_Time_Value &
ACE_Timer_List::earliest_time (void) const
{
  ACE_TRACE ("ACE_Timer_List::earliest_time");
  return this->head_->timer_value_;
}

// Remove all remaining items in the list.

ACE_Timer_List::~ACE_Timer_List (void)
{
  ACE_TRACE ("ACE_Timer_List::~ACE_Timer_List");
  ACE_MT (ACE_GUARD (ACE_Recursive_Thread_Mutex, ace_mon, this->lock_));
  
  ACE_Timer_Node *curr = this->head_;
  
  while (curr != 0)
    {
      ACE_Timer_Node *next = curr->next_;
      this->free_node (curr);
      curr = next;
    }
}

void 
ACE_Timer_List::dump (void) const
{
  ACE_TRACE ("ACE_Timer_List::dump");
  ACE_DEBUG ((LM_DEBUG, ACE_BEGIN_DUMP, this));
  ACE_DEBUG ((LM_DEBUG, "\nhead_ = %d", this->head_));
  ACE_DEBUG ((LM_DEBUG, "\ntimer_id_ = %d", this->timer_id_));
  ACE_DEBUG ((LM_DEBUG, ACE_END_DUMP));    
}

// Reschedule a periodic timer.  This function must be called with the
// mutex lock held.

void 
ACE_Timer_List::reschedule (ACE_Timer_Node *expired)
{
  ACE_TRACE ("ACE_Timer_List::reschedule");
  if (this->is_empty () 
      || expired->timer_value_ < this->earliest_time ())
    {
      expired->next_ = this->head_;
      this->head_ = expired;
    }
  else
    {
      ACE_Timer_Node *prev = this->head_;
      ACE_Timer_Node *after = this->head_->next_;

      // Locate the proper position in the queue. 

      while (after != 0 
	     && expired->timer_value_ > after->timer_value_)
	{
	  prev = after;
	  after = after->next_;
	}

      expired->next_ = after;
      prev->next_ = expired;
    }
}

// Insert a new handler that expires at time future_time; if interval
// is > 0, the handler will be reinvoked periodically.

int
ACE_Timer_List::schedule (ACE_Event_Handler *handler,
			  const void *arg,
			  const ACE_Time_Value	&future_time, 
			  const ACE_Time_Value	&interval)
{
  ACE_TRACE ("ACE_Timer_List::schedule");
  ACE_MT (ACE_GUARD_RETURN (ACE_Recursive_Thread_Mutex, ace_mon, this->lock_, -1));

  // Increment the sequence number (it will wrap around).
  int timer_id = this->timer_id ();

  if (this->is_empty () || future_time < this->earliest_time ())
    {
      // Place at the beginning of the list.
      ACE_Timer_Node *temp = this->alloc_node ();

      // Use operator placement new.
      this->head_ = new (temp) ACE_Timer_Node (handler, 
					       arg, 
					       future_time,
					       interval, 
					       this->head_,
					       timer_id);
      return timer_id;
    }

  // Place in the middle of the list where it belongs (i.e., sorted in
  // ascending order of absolute time to expire).
  else 
    {
      ACE_Timer_Node *prev = this->head_;
      ACE_Timer_Node *after = this->head_->next_;

      while (after != 0 && future_time > after->timer_value_)
	{
	  prev = after;
	  after = after->next_;
	}

      ACE_Timer_Node *temp = this->alloc_node ();

      // Use operator placement new.
      prev->next_ = new (temp) ACE_Timer_Node (handler, 
					       arg, 
					       future_time,
					       interval, 
					       after,
					       timer_id);
      return timer_id;
    }
}

int
ACE_Timer_List::timer_id (void)
{
  this->timer_id_++;

  // Make sure this never == -1
  if (this->timer_id_ == -1)
    this->timer_id_ = 0;

  return this->timer_id_;
}

// Locate and remove the single <ACE_Event_Handler> with a value of
// <timer_id> from the timer queue.

int
ACE_Timer_List::cancel (int timer_id, 
			const void **arg)
{
  ACE_TRACE ("ACE_Timer_List::cancel");
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

      this->free_node (curr);
      return 1;
    }
  else
    return 0;
}

// Locate and remove all values of <handler> from the timer queue.

int
ACE_Timer_List::cancel (ACE_Event_Handler *handler)
{
  ACE_TRACE ("ACE_Timer_List::cancel");
  ACE_MT (ACE_GUARD_RETURN (ACE_Recursive_Thread_Mutex, ace_mon, this->lock_, -1));

  ACE_Timer_Node *prev = 0;
  ACE_Timer_Node *curr = this->head_;

  int number_of_cancellations = 0;

  while (curr != 0)
    {
      if (curr->handler_ == handler)
	{
	  number_of_cancellations++;

	  if (prev == 0)
	    {
	      this->head_ = curr->next_;
	      this->free_node (curr);
	      curr = this->head_;
	    }
	  else
	    {
	      prev->next_ = curr->next_;
	      this->free_node (curr);
	      curr = prev->next_;
	    }
	}
      else
	{
	  prev = curr;
	  curr = curr->next_;
	}
    }

  return number_of_cancellations;
}
