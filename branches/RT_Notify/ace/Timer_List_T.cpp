// $Id$

#ifndef ACE_TIMER_LIST_T_C
#define ACE_TIMER_LIST_T_C

#include "ace/Timer_List_T.h"
#include "ace/Log_Msg.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

ACE_RCSID(ace, Timer_List_T, "$Id$")

// Default Constructor

template <class TYPE, class FUNCTOR, class ACE_LOCK>
ACE_Timer_List_Iterator_T<TYPE, FUNCTOR, ACE_LOCK>::ACE_Timer_List_Iterator_T (ACE_Timer_List_T<TYPE, FUNCTOR, ACE_LOCK> &timer_list)
  : timer_list_ (timer_list)
{
  this->first();
  // Nothing
}

template <class TYPE, class FUNCTOR, class ACE_LOCK>
ACE_Timer_List_Iterator_T<TYPE, FUNCTOR, ACE_LOCK>::~ACE_Timer_List_Iterator_T (void)
{
}

// Positions the iterator at the node right after the dummy node

template <class TYPE, class FUNCTOR, class ACE_LOCK> void
ACE_Timer_List_Iterator_T<TYPE, FUNCTOR, ACE_LOCK>::first (void)
{
  this->position_ = this->timer_list_.head_->get_next ();
}

// Positions the iterator at the next node in the Timer Queue

template <class TYPE, class FUNCTOR, class ACE_LOCK> void
ACE_Timer_List_Iterator_T<TYPE, FUNCTOR, ACE_LOCK>::next (void)
{
  // Make sure that if we are at the end, we don't wrap around
  if (this->position_ != this->timer_list_.head_)
    this->position_ = this->position_->get_next ();
}

// Returns true when we are at <head_>

template <class TYPE, class FUNCTOR, class ACE_LOCK> int
ACE_Timer_List_Iterator_T<TYPE, FUNCTOR, ACE_LOCK>::isdone (void) const
{
  return this->position_ == this->timer_list_.head_;
}

// Returns the node at <position_> or 0 if we are at the end

template <class TYPE, class FUNCTOR, class ACE_LOCK> ACE_Timer_Node_T<TYPE> *
ACE_Timer_List_Iterator_T<TYPE, FUNCTOR, ACE_LOCK>::item (void)
{
  if (this->position_ != this->timer_list_.head_)
    return this->position_;
  return 0;
}

// Return our instance of the iterator

template <class TYPE, class FUNCTOR, class ACE_LOCK> ACE_Timer_Queue_Iterator_T<TYPE, FUNCTOR, ACE_LOCK> &
ACE_Timer_List_T<TYPE, FUNCTOR, ACE_LOCK>::iter (void)
{
  this->iterator_->first ();
  return *this->iterator_;
}

// Create an empty list.

template <class TYPE, class FUNCTOR, class ACE_LOCK>
ACE_Timer_List_T<TYPE, FUNCTOR, ACE_LOCK>::ACE_Timer_List_T (FUNCTOR *upcall_functor,
                                                         ACE_Free_List<ACE_Timer_Node_T <TYPE> > *freelist)
  : ACE_Timer_Queue_T<TYPE, FUNCTOR, ACE_LOCK> (upcall_functor, freelist),
    head_ (new ACE_Timer_Node_T<TYPE>),
    timer_id_ (0)
{
  ACE_TRACE ("ACE_Timer_List_T::ACE_Timer_List");

  this->head_->set_next (this->head_);
  this->head_->set_prev (this->head_);

  ACE_NEW (iterator_,
           LIST_ITERATOR (*this));
}


// Checks if list is empty.

template <class TYPE, class FUNCTOR, class ACE_LOCK> int
ACE_Timer_List_T<TYPE, FUNCTOR, ACE_LOCK>::is_empty (void) const
{
  ACE_TRACE ("ACE_Timer_List_T::is_empty");
  return this->head_->get_next () == this->head_;
}


// Returns earliest time in a non-empty list.

template <class TYPE, class FUNCTOR, class ACE_LOCK> const ACE_Time_Value &
ACE_Timer_List_T<TYPE, FUNCTOR, ACE_LOCK>::earliest_time (void) const
{
  ACE_TRACE ("ACE_Timer_List_T::earliest_time");
  return this->head_->get_next ()->get_timer_value ();
}


// Remove all remaining items in the list.

template <class TYPE, class FUNCTOR, class ACE_LOCK>
ACE_Timer_List_T<TYPE, FUNCTOR, ACE_LOCK>::~ACE_Timer_List_T (void)
{
  ACE_TRACE ("ACE_Timer_List_T::~ACE_Timer_List_T");
  ACE_MT (ACE_GUARD (ACE_LOCK, ace_mon, this->mutex_));

  delete iterator_;

  for (ACE_Timer_Node_T<TYPE> *curr = this->head_->get_next ();
       curr != this->head_;
       )
    {
      ACE_Timer_Node_T<TYPE> *next = curr->get_next ();
      this->upcall_functor ().deletion (*this,
                                        next->get_type (),
                                        next->get_act ());
      this->free_node (curr);
      curr = next;
    }

  // delete the dummy node
  delete this->head_;
}

template <class TYPE, class FUNCTOR, class ACE_LOCK> void
ACE_Timer_List_T<TYPE, FUNCTOR, ACE_LOCK>::dump (void) const
{
  ACE_TRACE ("ACE_Timer_List_T::dump");
  ACE_DEBUG ((LM_DEBUG, ACE_BEGIN_DUMP, this));

  size_t count = 0;

  for (ACE_Timer_Node_T<TYPE> *curr = this->head_->get_next ();
       curr != this->head_;
       curr = curr->get_next ())
    count++;

  ACE_DEBUG ((LM_DEBUG, ACE_LIB_TEXT ("\nsize_ = %d"), count));
  ACE_DEBUG ((LM_DEBUG, ACE_LIB_TEXT ("\ntimer_id_ = %d"), this->timer_id_));
  ACE_DEBUG ((LM_DEBUG, ACE_END_DUMP));
}


// Reschedule a periodic timer.  This function must be called with the
// lock held.

template <class TYPE, class FUNCTOR, class ACE_LOCK> void
ACE_Timer_List_T<TYPE, FUNCTOR, ACE_LOCK>::reschedule (ACE_Timer_Node_T<TYPE> *expired)
{
  ACE_TRACE ("ACE_Timer_List_T::reschedule");

  ACE_Timer_Node_T<TYPE> *after = this->head_->get_next ();

  // Locate the proper position in the queue.

  while (after != this->head_
         && expired->get_timer_value () > after->get_timer_value ())
      after = after->get_next ();

  expired->set_next (after);
  expired->set_prev (after->get_prev ());
  after->get_prev ()->set_next (expired);
  after->set_prev (expired);
}


// Insert a new handler that expires at time future_time; if interval
// is > 0, the handler will be reinvoked periodically.

template <class TYPE, class FUNCTOR, class ACE_LOCK> long
ACE_Timer_List_T<TYPE, FUNCTOR, ACE_LOCK>::schedule (const TYPE &type,
                                                 const void *act,
                                                 const ACE_Time_Value &future_time,
                                                 const ACE_Time_Value &interval)
{
  ACE_TRACE ("ACE_Timer_List_T::schedule");
  ACE_MT (ACE_GUARD_RETURN (ACE_LOCK, ace_mon, this->mutex_, -1));

  // Place in the middle of the list where it belongs (i.e., sorted in
  // ascending order of absolute time to expire).
  ACE_Timer_Node_T<TYPE> *after = this->head_->get_next ();

  while (after != this->head_
         && future_time > after->get_timer_value ())
      after = after->get_next ();

  ACE_Timer_Node_T<TYPE> *temp = this->alloc_node ();

  temp->set (type,
             act,
             future_time,
             interval,
             after->get_prev (),
             after,
             (long) temp);

  after->get_prev ()->set_next (temp);
  after->set_prev (temp);

  return ACE_reinterpret_cast (long, temp);
}

// Locate and update the inteval on the timer_id

template <class TYPE, class FUNCTOR, class ACE_LOCK> int 
ACE_Timer_List_T<TYPE, FUNCTOR, ACE_LOCK>::reset_interval (long timer_id, 
                                                           const ACE_Time_Value &interval)
{
  ACE_TRACE ("ACE_Timer_List_T::reset_interval");
  ACE_MT (ACE_GUARD_RETURN (ACE_LOCK, ace_mon, this->mutex_, -1));

  // Make sure we are getting a valid <timer_id>, not an error
  // returned by <schedule>.
  if (timer_id == -1)
    return -1;

  ACE_Timer_Node_T<TYPE> *node =
    ACE_reinterpret_cast (ACE_Timer_Node_T<TYPE> *,
                          timer_id);

  node->set_interval (interval);

  return 0;
}

// Locate and remove the single <ACE_Event_Handler> with a value of
// <timer_id> from the timer queue.

template <class TYPE, class FUNCTOR, class ACE_LOCK> int
ACE_Timer_List_T<TYPE, FUNCTOR, ACE_LOCK>::cancel (long timer_id,
                                                   const void **act,
                                                   int dont_call)
{
  ACE_TRACE ("ACE_Timer_List_T::cancel");
  ACE_MT (ACE_GUARD_RETURN (ACE_LOCK, ace_mon, this->mutex_, -1));

  // Make sure we are getting a valid <timer_id>, not an error
  // returned by schedule ()
  if (timer_id == -1)
    return 0;

  ACE_Timer_Node_T<TYPE> *node = 
    ACE_reinterpret_cast (ACE_Timer_Node_T<TYPE> *,
                          timer_id);

  // Check to see if the node looks like a true ACE_Timer_Node_T<TYPE>
  if (timer_id == node->get_timer_id ())
    {
      node->get_next ()->set_prev (node->get_prev ());
      node->get_prev ()->set_next (node->get_next ());

      if (act != 0)
        *act = node->get_act ();

      if (dont_call == 0)
        this->upcall_functor ().cancellation (*this,
                                              node->get_type ());
      this->free_node (node);
      return 1;
    }

  // Wasn't valid
  return 0;
}


// Locate and remove all values of <handler> from the timer queue.

template <class TYPE, class FUNCTOR, class ACE_LOCK> int
ACE_Timer_List_T<TYPE, FUNCTOR, ACE_LOCK>::cancel (const TYPE &type,
                                               int dont_call)
{
  ACE_TRACE ("ACE_Timer_List_T::cancel");
  ACE_MT (ACE_GUARD_RETURN (ACE_LOCK, ace_mon, this->mutex_, -1));

  int number_of_cancellations = 0;

  for (ACE_Timer_Node_T<TYPE> *curr = this->head_->get_next ();
       curr != this->head_;
       )
    {
      if (curr->get_type () == type)
        {
          number_of_cancellations++;

          curr->get_prev ()->set_next (curr->get_next ());
          curr->get_next ()->set_prev (curr->get_prev ());
          ACE_Timer_Node_T<TYPE> *temp = curr;
          curr = curr->get_next ();
          this->free_node (temp);
        }
      else
        curr = curr->get_next ();
    }

  if (dont_call == 0)
     this->upcall_functor ().cancellation (*this, type);

  return number_of_cancellations;
}

// Reads the first node on the list and returns it.

template <class TYPE, class FUNCTOR, class ACE_LOCK> ACE_Timer_Node_T<TYPE> *
ACE_Timer_List_T<TYPE, FUNCTOR, ACE_LOCK>::get_first (void)
{
  ACE_TRACE ("ACE_Timer_List_T::get_first");

  return this->head_->get_next ();
}

// Removes the first node on the list and returns it.

template <class TYPE, class FUNCTOR, class ACE_LOCK> ACE_Timer_Node_T<TYPE> *
ACE_Timer_List_T<TYPE, FUNCTOR, ACE_LOCK>::remove_first (void)
{
  ACE_TRACE ("ACE_Timer_List_T::remove_first");

  // remove the node and fix the pointers
  ACE_Timer_Node_T<TYPE> *temp = this->head_->get_next ();
  this->head_->set_next (temp->get_next ());
  temp->get_next ()->set_prev (this->head_);

  return temp;
}

#endif /* ACE_TIMER_LIST_T_C */
