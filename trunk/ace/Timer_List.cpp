// $Id$

#if !defined (ACE_TIMER_LIST_C)
#define ACE_TIMER_LIST_C

#define ACE_BUILD_DLL

#include "ace/Timer_List.h"

template <class TYPE, class FUNCTOR>
ACE_Timer_List_Iterator_T<TYPE, FUNCTOR>::ACE_Timer_List_Iterator_T (ACE_Timer_List_T<TYPE, FUNCTOR> &list)
  : timer_list_ (list)
{
}


template <class TYPE, class FUNCTOR> int
ACE_Timer_List_Iterator_T<TYPE, FUNCTOR>::next (ACE_Timer_Node_T<TYPE, FUNCTOR> *&node,
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

ACE_ALLOC_HOOK_DEFINE(ACE_Timer_List_T)

template <class TYPE, class FUNCTOR> ACE_Timer_Queue_Iterator_T<TYPE, FUNCTOR> &
ACE_Timer_List_T<TYPE, FUNCTOR>::iter (void)
{
  return this->iterator_;
}

// Create an empty list.

template <class TYPE, class FUNCTOR> 
ACE_Timer_List_T<TYPE, FUNCTOR>::ACE_Timer_List_T (FUNCTOR *upcall_functor)
  : ACE_Timer_Queue_T<TYPE, FUNCTOR> (upcall_functor),
    head_ (0),
    iterator_ (*this),
    timer_id_ (0)
{
  ACE_TRACE ("ACE_Timer_List::ACE_Timer_List");
}

// Checks if list is empty.


template <class TYPE, class FUNCTOR> int 
ACE_Timer_List_T<TYPE, FUNCTOR>::is_empty (void) const
{
  ACE_TRACE ("ACE_Timer_List::is_empty");
  return this->head_ == 0;
}


template <class TYPE, class FUNCTOR> ACE_Timer_Node_T<TYPE, FUNCTOR> *
ACE_Timer_List_T<TYPE, FUNCTOR>::alloc_node (void)
{
  return new NODE;
}


template <class TYPE, class FUNCTOR> void
ACE_Timer_List_T<TYPE, FUNCTOR>::free_node (ACE_Timer_Node_T<TYPE, FUNCTOR> *node)
{
  delete node;
}

// Returns earliest time in a non-empty list.


template <class TYPE, class FUNCTOR> const ACE_Time_Value &
ACE_Timer_List_T<TYPE, FUNCTOR>::earliest_time (void) const
{
  ACE_TRACE ("ACE_Timer_List::earliest_time");
  return this->head_->timer_value_;
}

// Remove all remaining items in the list.

template <class TYPE, class FUNCTOR> 
ACE_Timer_List_T<TYPE, FUNCTOR>::~ACE_Timer_List_T (void)
{
  ACE_TRACE ("ACE_Timer_List::~ACE_Timer_List");
  ACE_MT (ACE_GUARD (ACE_Recursive_Thread_Mutex, ace_mon, this->lock_));
  
  NODE *curr = this->head_;
  
  while (curr != 0)
    {
      NODE *next = curr->next_;
      this->free_node (curr);
      curr = next;
    }
}


template <class TYPE, class FUNCTOR> void 
ACE_Timer_List_T<TYPE, FUNCTOR>::dump (void) const
{
  ACE_TRACE ("ACE_Timer_List::dump");
  ACE_DEBUG ((LM_DEBUG, ACE_BEGIN_DUMP, this));
  ACE_DEBUG ((LM_DEBUG, "\nhead_ = %d", this->head_));
  ACE_DEBUG ((LM_DEBUG, "\ntimer_id_ = %d", this->timer_id_));
  ACE_DEBUG ((LM_DEBUG, ACE_END_DUMP));    
}

// Reschedule a periodic timer.  This function must be called with the
// mutex lock held.


template <class TYPE, class FUNCTOR> void 
ACE_Timer_List_T<TYPE, FUNCTOR>::reschedule (ACE_Timer_Node_T<TYPE, FUNCTOR> *expired)
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
      NODE *prev = this->head_;
      NODE *after = this->head_->next_;

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


template <class TYPE, class FUNCTOR> int
ACE_Timer_List_T<TYPE, FUNCTOR>::schedule (const TYPE &type,
					   const void *act,
					   const ACE_Time_Value &future_time, 
					   const ACE_Time_Value &interval)
{
  ACE_TRACE ("ACE_Timer_List::schedule");
  ACE_MT (ACE_GUARD_RETURN (ACE_Recursive_Thread_Mutex, ace_mon, this->lock_, -1));

  // Increment the sequence number (it will wrap around).
  int timer_id = this->timer_id ();
  
  if (this->is_empty () || future_time < this->earliest_time ())
    {
      // Place at the beginning of the list.
      NODE *temp = this->alloc_node ();
      
      // Use operator placement new.
      this->head_ = new (temp) NODE (type, 
				     act, 
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
      NODE *prev = this->head_;
      NODE *after = this->head_->next_;
	  
      while (after != 0 && future_time > after->timer_value_)
	{
	  prev = after;
	  after = after->next_;
	}

      NODE *temp = this->alloc_node ();

      // Use operator placement new.
      prev->next_ = new (temp) NODE (type, 
				     act, 
				     future_time,
				     interval, 
				     after,
				     timer_id);
      return timer_id;
    }
}


template <class TYPE, class FUNCTOR> int
ACE_Timer_List_T<TYPE, FUNCTOR>::timer_id (void)
{
  this->timer_id_++;

  // Make sure this never == -1
  if (this->timer_id_ == -1)
    this->timer_id_ = 0;

  return this->timer_id_;
}

// Locate and remove the single <ACE_Event_Handler> with a value of
// <timer_id> from the timer queue.


template <class TYPE, class FUNCTOR> int
ACE_Timer_List_T<TYPE, FUNCTOR>::cancel (int timer_id, 
					 const void **act,
					 int dont_call)
{
  ACE_TRACE ("ACE_Timer_List::cancel");
  ACE_MT (ACE_GUARD_RETURN (ACE_Recursive_Thread_Mutex, ace_mon, this->lock_, -1));

  NODE *prev = 0;
  NODE *curr = 0;

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

      if (act != 0)
	*act = curr->act_;

      if (dont_call == 0)
	// This calls the correct operator () on the functor (the one with
	// two args)
	this->upcall_functor_.operator () (*this, curr->type_);
      this->free_node (curr);
      return 1;
    }
  else
    return 0;
}

// Locate and remove all values of <handler> from the timer queue.


template <class TYPE, class FUNCTOR> int
ACE_Timer_List_T<TYPE, FUNCTOR>::cancel (const TYPE &type,
					 int dont_call)
{
  ACE_TRACE ("ACE_Timer_List::cancel");
  ACE_MT (ACE_GUARD_RETURN (ACE_Recursive_Thread_Mutex, ace_mon, this->lock_, -1));

  NODE *prev = 0;
  NODE *curr = this->head_;

  int number_of_cancellations = 0;

  while (curr != 0)
    {
      if (curr->type_ == type)
	{
	  number_of_cancellations++;

	  if (dont_call == 0 
	      && number_of_cancellations == 1)	  
	    // This calls the correct operator () on the functor (the one with
	    // two args)
	    this->upcall_functor_.operator () (*this, curr->type_);
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

#endif /* ACE_TIMER_LIST_C */

