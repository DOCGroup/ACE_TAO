// $Id$

#if !defined (ACE_TIMER_LIST_T_C)
#define ACE_TIMER_LIST_T_C

#define ACE_BUILD_DLL

#include "ace/Timer_List_T.h"

template <class TYPE, class FUNCTOR, class LOCK>
ACE_Timer_List_Iterator_T<TYPE, FUNCTOR, LOCK>::ACE_Timer_List_Iterator_T (ACE_Timer_List_T<TYPE, FUNCTOR, LOCK> &list)
  : timer_list_ (list)
{
}


template <class TYPE, class FUNCTOR, class LOCK> int
ACE_Timer_List_Iterator_T<TYPE, FUNCTOR, LOCK>::next (ACE_Timer_Node_T<TYPE, FUNCTOR, LOCK> *&node,
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

template <class TYPE, class FUNCTOR, class LOCK> ACE_Timer_Queue_Iterator_T<TYPE, FUNCTOR, LOCK> &
ACE_Timer_List_T<TYPE, FUNCTOR, LOCK>::iter (void)
{
  return this->iterator_;
}

// Create an empty list.

template <class TYPE, class FUNCTOR, class LOCK> 
ACE_Timer_List_T<TYPE, FUNCTOR, LOCK>::ACE_Timer_List_T (FUNCTOR *upcall_functor)
  : ACE_Timer_Queue_T<TYPE, FUNCTOR, LOCK> (upcall_functor),
    head_ (0),
    iterator_ (*this),
    timer_id_ (0)
{
  ACE_TRACE ("ACE_Timer_List::ACE_Timer_List");
}

// Checks if list is empty.


template <class TYPE, class FUNCTOR, class LOCK> int 
ACE_Timer_List_T<TYPE, FUNCTOR, LOCK>::is_empty (void) const
{
  ACE_TRACE ("ACE_Timer_List::is_empty");
  return this->head_ == 0;
}

template <class TYPE, class FUNCTOR, class LOCK> ACE_Timer_Node_T<TYPE, FUNCTOR, LOCK> *
ACE_Timer_List_T<TYPE, FUNCTOR, LOCK>::alloc_node (void)
{
  return new ACE_Timer_Node_T<TYPE, FUNCTOR, LOCK>;
}

template <class TYPE, class FUNCTOR, class LOCK> void
ACE_Timer_List_T<TYPE, FUNCTOR, LOCK>::free_node (ACE_Timer_Node_T<TYPE, FUNCTOR, LOCK> *node)
{
  delete node;
}

// Returns earliest time in a non-empty list.


template <class TYPE, class FUNCTOR, class LOCK> const ACE_Time_Value &
ACE_Timer_List_T<TYPE, FUNCTOR, LOCK>::earliest_time (void) const
{
  ACE_TRACE ("ACE_Timer_List::earliest_time");
  return this->head_->timer_value_;
}

// Remove all remaining items in the list.

template <class TYPE, class FUNCTOR, class LOCK> 
ACE_Timer_List_T<TYPE, FUNCTOR, LOCK>::~ACE_Timer_List_T (void)
{
  ACE_TRACE ("ACE_Timer_List::~ACE_Timer_List");
  ACE_MT (ACE_GUARD (LOCK, ace_mon, this->lock_));
  
  ACE_Timer_Node_T<TYPE, FUNCTOR, LOCK> *curr = this->head_;
  
  while (curr != 0)
    {
      ACE_Timer_Node_T<TYPE, FUNCTOR, LOCK> *next = curr->next_;
      this->free_node (curr);
      curr = next;
    }
}


template <class TYPE, class FUNCTOR, class LOCK> void 
ACE_Timer_List_T<TYPE, FUNCTOR, LOCK>::dump (void) const
{
  ACE_TRACE ("ACE_Timer_List::dump");
  ACE_DEBUG ((LM_DEBUG, ACE_BEGIN_DUMP, this));
  ACE_DEBUG ((LM_DEBUG, "\nhead_ = %d", this->head_));
  ACE_DEBUG ((LM_DEBUG, "\ntimer_id_ = %d", this->timer_id_));
  ACE_DEBUG ((LM_DEBUG, ACE_END_DUMP));    
}

// Reschedule a periodic timer.  This function must be called with the
// mutex lock held.


template <class TYPE, class FUNCTOR, class LOCK> void 
ACE_Timer_List_T<TYPE, FUNCTOR, LOCK>::reschedule (ACE_Timer_Node_T<TYPE, FUNCTOR, LOCK> *expired)
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
      ACE_Timer_Node_T<TYPE, FUNCTOR, LOCK> *prev = this->head_;
      ACE_Timer_Node_T<TYPE, FUNCTOR, LOCK> *after = this->head_->next_;

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


template <class TYPE, class FUNCTOR, class LOCK> long
ACE_Timer_List_T<TYPE, FUNCTOR, LOCK>::schedule (const TYPE &type,
						 const void *act,
						 const ACE_Time_Value &future_time, 
						 const ACE_Time_Value &interval)
{
  ACE_TRACE ("ACE_Timer_List::schedule");
  ACE_MT (ACE_GUARD_RETURN (LOCK, ace_mon, this->lock_, -1));

  // Increment the sequence number (it will wrap around).
  long timer_id = this->timer_id ();
  
  if (this->is_empty () || future_time < this->earliest_time ())
    {
      // Place at the beginning of the list.
      ACE_Timer_Node_T<TYPE, FUNCTOR, LOCK> *temp = this->alloc_node ();
      
      // Use operator placement new.
      this->head_ = new (temp) ACE_Timer_Node_T<TYPE, FUNCTOR, LOCK> (type, 
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
      ACE_Timer_Node_T<TYPE, FUNCTOR, LOCK> *prev = this->head_;
      ACE_Timer_Node_T<TYPE, FUNCTOR, LOCK> *after = this->head_->next_;
	  
      while (after != 0 && future_time > after->timer_value_)
	{
	  prev = after;
	  after = after->next_;
	}

      ACE_Timer_Node_T<TYPE, FUNCTOR, LOCK> *temp = this->alloc_node ();

      // Use operator placement new.
      prev->next_ = new (temp) ACE_Timer_Node_T<TYPE, FUNCTOR, LOCK> (type, 
								      act, 
								      future_time,
								      interval, 
								      after,
								      timer_id);
      return timer_id;
    }
}


template <class TYPE, class FUNCTOR, class LOCK> int
ACE_Timer_List_T<TYPE, FUNCTOR, LOCK>::timer_id (void)
{
  // We need to truncate this to <int> for backwards compatibility.
  int tid = this->timer_id_++;

  // Make sure this never == -1
  if (tid == -1)
    tid = 0;

  return tid;
}

// Locate and remove the single <ACE_Event_Handler> with a value of
// <timer_id> from the timer queue.


template <class TYPE, class FUNCTOR, class LOCK> int
ACE_Timer_List_T<TYPE, FUNCTOR, LOCK>::cancel (long timer_id,
					       const void **act,
					       int dont_call)
{
  ACE_TRACE ("ACE_Timer_List::cancel");
  ACE_MT (ACE_GUARD_RETURN (LOCK, ace_mon, this->lock_, -1));

  ACE_Timer_Node_T<TYPE, FUNCTOR, LOCK> *prev = 0;
  ACE_Timer_Node_T<TYPE, FUNCTOR, LOCK> *curr = 0;

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
	this->upcall_functor_.cancellation (*this, curr->type_);
      this->free_node (curr);
      return 1;
    }
  else
    return 0;
}

// Locate and remove all values of <handler> from the timer queue.


template <class TYPE, class FUNCTOR, class LOCK> int
ACE_Timer_List_T<TYPE, FUNCTOR, LOCK>::cancel (const TYPE &type,
					       int dont_call)
{
  ACE_TRACE ("ACE_Timer_List::cancel");
  ACE_MT (ACE_GUARD_RETURN (LOCK, ace_mon, this->lock_, -1));

  ACE_Timer_Node_T<TYPE, FUNCTOR, LOCK> *prev = 0;
  ACE_Timer_Node_T<TYPE, FUNCTOR, LOCK> *curr = this->head_;

  int number_of_cancellations = 0;

  while (curr != 0)
    {
      if (curr->type_ == type)
	{
	  number_of_cancellations++;

	  if (dont_call == 0 
	      && number_of_cancellations == 1)	  
	    this->upcall_functor_.cancellation (*this, curr->type_);

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

#endif /* ACE_TIMER_LIST_T_C */

