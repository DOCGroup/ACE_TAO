// $Id$

#ifndef ACE_TIMER_WHEEL_T_C
#define ACE_TIMER_WHEEL_T_C

#include "ace/Timer_Wheel_T.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "ace/High_Res_Timer.h"
#include "ace/Log_Msg.h"

ACE_RCSID(ace, Timer_Wheel_T, "$Id$")

/**
 * Just initializes the iterator with a ACE_Timer_Wheel_T and then calls
 * first() to initialize the rest of itself.
 *
 * @param wheel A reference for a timer queue to iterate over
 */
template <class TYPE, class FUNCTOR, class ACE_LOCK>
ACE_Timer_Wheel_Iterator_T<TYPE,
                           FUNCTOR,
                           ACE_LOCK>::ACE_Timer_Wheel_Iterator_T (
  ACE_Timer_Wheel_T<TYPE, FUNCTOR, ACE_LOCK> &wheel
  )
  : timer_wheel_ (wheel)
{
  this->first();
}


/**
 * Destructor, at this level does nothing.
 */
template <class TYPE, class FUNCTOR, class ACE_LOCK>
ACE_Timer_Wheel_Iterator_T<TYPE,
                           FUNCTOR,
                           ACE_LOCK>::~ACE_Timer_Wheel_Iterator_T (void)
{
}


/**
 * Positions the iterator at the first position in the timing wheel
 * that contains something. pos_ will be set to the position of this entry
 * and list_item_ will point to the first entry in that position.  Since
 * this is an iterator,
 *
 * If the wheel is empty, pos_ will be equal timer_wheel_.wheel_size_ and
 * list_item_ would be 0.
 */
template <class TYPE, class FUNCTOR, class ACE_LOCK> void
ACE_Timer_Wheel_Iterator_T<TYPE, FUNCTOR, ACE_LOCK>::first (void)
{
  for (this->pos_ = 0;
       this->pos_ < this->timer_wheel_.wheel_size_;
       this->pos_++)
    {
      // Skip over empty entries
      if (this->timer_wheel_.wheel_[this->pos_]->get_next ()
          != this->timer_wheel_.wheel_[this->pos_])
        {
          this->list_item_ =
            this->timer_wheel_.wheel_[this->pos_]->get_next ();
          return;
        }
    }

  // The queue is empty if we are here
  this->list_item_ = 0;
}


/**
 * Positions the iterator at the next node in list or goes to the next
 * list
 */
template <class TYPE, class FUNCTOR, class ACE_LOCK> void
ACE_Timer_Wheel_Iterator_T<TYPE, FUNCTOR, ACE_LOCK>::next (void)
{
  if (this->isdone ())
    return;

  this->list_item_ =
    this->list_item_->get_next ();

  // If there is no more in the current list, go to the next
  if (this->list_item_ == this->timer_wheel_.wheel_[this->pos_])
    {
      for (this->pos_++;
           this->pos_ < this->timer_wheel_.wheel_size_;
           this->pos_++)
        {
          // Check for an empty entry
          if (this->timer_wheel_.wheel_[this->pos_]->get_next ()
              != this->timer_wheel_.wheel_[this->pos_])
            {
              this->list_item_ =
                this->timer_wheel_.wheel_[this->pos_]->get_next ();
              return;
            }
        }

      this->list_item_ = 0;
    }
}


/**
 * @return True when we there isn't anymore items (when list_item_ == 0)
 */
template <class TYPE, class FUNCTOR, class ACE_LOCK> int
ACE_Timer_Wheel_Iterator_T<TYPE, FUNCTOR, ACE_LOCK>::isdone (void) const
{
  return this->list_item_ == 0;
}


/**
 * @return The node at the current position in the sequence or 0 if the wheel
 *         is empty
 */
template <class TYPE, class FUNCTOR, class ACE_LOCK> ACE_Timer_Node_T<TYPE> *
ACE_Timer_Wheel_Iterator_T<TYPE, FUNCTOR, ACE_LOCK>::item (void)
{
  if (this->isdone ())
    return 0;

  return this->list_item_;
}


/**
 * Constructor that sets up the timing wheel and also may preallocate
 * some nodes on the free list
 *
 * @param wheelsize      The number of lists in the timer wheel
 * @param resolution     The time resolution used by the hashing function
 * @param prealloc       The number of entries to prealloc in the free_list
 * @param upcall_functor A pointer to a functor to use instead of the default
 * @param freelist       A pointer to a freelist to use instead of the default
 */
template <class TYPE, class FUNCTOR, class ACE_LOCK>
ACE_Timer_Wheel_T<TYPE, FUNCTOR, ACE_LOCK>::ACE_Timer_Wheel_T (
  size_t wheelsize,
  size_t resolution,
  size_t prealloc,
  FUNCTOR *upcall_functor,
  ACE_Free_List<ACE_Timer_Node_T <TYPE> > *freelist
  )
  : ACE_Timer_Queue_T<TYPE,FUNCTOR,ACE_LOCK> (upcall_functor, freelist),
    wheel_size_ (wheelsize),
    resolution_ (resolution),
    earliest_pos_ (0)
{
  ACE_TRACE ("ACE_Timer_Wheel_T::ACE_Timer_Wheel_T");
  size_t i;

  this->gettimeofday (ACE_OS::gettimeofday);

  // Create the timing wheel
  ACE_NEW (this->wheel_,
           ACE_Timer_Node_T<TYPE> *[wheelsize]);

  // Create the dummy nodes
  for (i = 0; i < wheelsize; i++)
    {
      ACE_Timer_Node_T<TYPE> *tempnode =
        this->alloc_node ();
      tempnode->set_next (tempnode);
      tempnode->set_prev (tempnode);
      this->wheel_[i] = tempnode;
    }

  // Do the preallocation
  this->free_list_->resize (prealloc);

  ACE_NEW (iterator_,
           WHEEL_ITERATOR (*this));
}


/**
 * Default Constructor that sets defaults for wheel_size_ and resolution_
 * and doesn't do any preallocation.
 *
 * @param upcall_functor A pointer to a functor to use instead of the default
 * @param freelist       A pointer to a freelist to use instead of the default
 */
template <class TYPE, class FUNCTOR, class ACE_LOCK>
ACE_Timer_Wheel_T<TYPE, FUNCTOR, ACE_LOCK>::ACE_Timer_Wheel_T (
  FUNCTOR *upcall_functor,
  ACE_Free_List<ACE_Timer_Node_T <TYPE> > *freelist
  )
  : ACE_Timer_Queue_T<TYPE,FUNCTOR,ACE_LOCK> (upcall_functor, freelist),
    wheel_size_ (ACE_DEFAULT_TIMER_WHEEL_SIZE),
    resolution_ (ACE_DEFAULT_TIMER_WHEEL_RESOLUTION),
    earliest_pos_ (0)
{
  ACE_TRACE ("ACE_Timer_Wheel_T::ACE_Timer_Wheel_T");
  size_t i;

  this->gettimeofday (ACE_OS::gettimeofday);

  // Create the timing wheel
  ACE_NEW (this->wheel_,
           ACE_Timer_Node_T<TYPE> *[this->wheel_size_]);

  // Create the dummy nodes
  for (i = 0;
       i < this->wheel_size_;
       i++)
    {
      ACE_Timer_Node_T<TYPE> *tempnode = this->alloc_node ();
      tempnode->set_next (tempnode);
      tempnode->set_prev (tempnode);
      this->wheel_[i] = tempnode;
    }

  ACE_NEW (iterator_,
           WHEEL_ITERATOR (*this));
}

// Destructor just cleans up its memory

template <class TYPE, class FUNCTOR, class ACE_LOCK>
ACE_Timer_Wheel_T<TYPE, FUNCTOR, ACE_LOCK>::~ACE_Timer_Wheel_T (void)
{
  ACE_TRACE ("ACE_Timer_Wheel_T::~ACE_Timer_Wheel_T");

  delete iterator_;

  for (size_t i = 0;
       i < this->wheel_size_;
       i++)
    {
      // delete nodes until only the dummy node is left
      while (this->wheel_[i]->get_next () != this->wheel_[i])
        {
          ACE_Timer_Node_T<TYPE> *next =
            this->wheel_[i]->get_next ();
          this->wheel_[i]->set_next (next->get_next ());
          next->get_next ()->set_prev (this->wheel_[i]);
          this->upcall_functor ().deletion (*this,
                                            next->get_type (),
                                            next->get_act ());
          this->free_node (next);
        }

      // and now delete the dummy node
      delete this->wheel_[i];
    }

  // finally delete the wheel
  delete [] this->wheel_;
}

 
/**
 * Checks to see if <earliest_pos> points to a empty list (then it is empty).
 *
 * @return True if empty
 */
template <class TYPE, class FUNCTOR, class ACE_LOCK> int
ACE_Timer_Wheel_T<TYPE, FUNCTOR, ACE_LOCK>::is_empty (void) const
{
  ACE_TRACE ("ACE_Timer_Wheel_T::is_empty");

  return this->wheel_[this->earliest_pos_]->get_next () 
         == this->wheel_[this->earliest_pos_];
}


/**
 * @return First (earliest) node in the wheel_'s earliest_pos_ list
 */
template <class TYPE, class FUNCTOR, class ACE_LOCK> const ACE_Time_Value &
ACE_Timer_Wheel_T<TYPE, FUNCTOR, ACE_LOCK>::earliest_time (void) const
{
  ACE_TRACE ("ACE_Timer_Wheel_T::earliest_time");

  return this->wheel_[this->earliest_pos_]->get_next ()->get_timer_value ();
}

/**
 * Creates a ACE_Timer_Node_T based on the input parameters.  Then inserts
 * the node into the wheel using reschedule ().  Then returns a timer_id
 * (which is actually a pointer to the actual timer_node).
 *
 *  @param type     The data of the timer node
 *  @param act      Asynchronous Completion Token (AKA magic cookie)
 *  @param delay    The time the timer is scheduled for (in absolute time)
 *  @param interval If not ACE_Time_Value::zero, then this is a periodic 
 *                  timer and interval is the time period 
 *
 *  @return Unique identifier (can be used to cancel the timer.
 *          -1 on failure.
 */
template <class TYPE, class FUNCTOR, class ACE_LOCK> long
ACE_Timer_Wheel_T<TYPE, FUNCTOR, ACE_LOCK>::schedule (
  const TYPE &type,
  const void *act,
  const ACE_Time_Value &delay,
  const ACE_Time_Value &interval
  )
{
  ACE_TRACE ("ACE_Timer_Wheel_T::schedule");
  ACE_MT (ACE_GUARD_RETURN (ACE_LOCK, ace_mon, this->mutex_, -1));

  ACE_Timer_Node_T<TYPE> *tempnode = this->alloc_node ();

  if (tempnode)
    {
      // Note that the timer_id is actually the pointer to the node

      // Set the details of the node
      tempnode->set (type,
                     act,
                     delay,
                     interval,
                     0,
                     0,
                     (long) tempnode);

      // Reschedule will insert it into the correct position
      this->reschedule (tempnode);

      return tempnode->get_timer_id ();
    }

  // Failure return
  errno = ENOMEM;
  return -1;
}


/**
 * Find the timer node by using the id as a pointer.  Then use set_interval ()
 * on the node to update the interval.
 * 
 * @param timer_id The timer identifier
 * @param interval The new interval
 * 
 * @return 0 if successful, -1 if no.
 */
template <class TYPE, class FUNCTOR, class ACE_LOCK> int 
ACE_Timer_Wheel_T<TYPE, FUNCTOR, ACE_LOCK>::reset_interval (
  long timer_id, 
  const ACE_Time_Value &interval
  )
{
  ACE_TRACE ("ACE_Timer_Wheel_T::reset_interval");
  ACE_MT (ACE_GUARD_RETURN (ACE_LOCK, ace_mon, this->mutex_, -1));

  // Make sure we are getting a valid <timer_id>, not an error
  // returned by <schedule>.
  if (timer_id == -1)
    return -1;

  ACE_Timer_Node_T<TYPE> *node =
    ACE_reinterpret_cast (ACE_Timer_Node_T<TYPE> *,
                          timer_id);

  // Check to see if the node looks like a true
  // ACE_Timer_Node_T<TYPE>.
  if (timer_id != node->get_timer_id ())
    return -1;

  node->set_interval (interval);
  return 0;
}


/**
 * Goes through every list in the wheel and whenever we find one with the
 * correct type value, we remove it and continue.  At the end make sure
 * we reset the earliest time value in case the earliest timers were
 * removed.
 *
 * @param type       The value to search for.
 * @param skip_close If this non-zero, the cancellation method of the
 *                   functor will not be called for each cancelled timer.
 *
 * @return Number of timers cancelled
 */
template <class TYPE, class FUNCTOR, class ACE_LOCK> int
ACE_Timer_Wheel_T<TYPE, FUNCTOR, ACE_LOCK>::cancel (const TYPE &type,
                                                    int skip_close)
{
  ACE_TRACE ("ACE_Timer_Wheel_T::cancel");
  ACE_MT (ACE_GUARD_RETURN (ACE_LOCK, ace_mon, this->mutex_, -1));

  int number_of_cancellations = 0;
  size_t i;

  // Walk through the wheel
  for (i = 0;
       i < this->wheel_size_;
       i++)
    {

      // Walk through the list.
      for (ACE_Timer_Node_T<TYPE> *curr =
             this->wheel_[i]->get_next ();
           curr != this->wheel_[i];
           )
        {
          if (curr->get_type () == type)
            {
              // Cancel it and remove it.
              number_of_cancellations++;

              // Detach it from the list
              ACE_Timer_Node_T<TYPE> *tempnode = curr;
              curr->get_prev ()->set_next (curr->get_next ());
              curr->get_next ()->set_prev (curr->get_prev ());

              // Go on to the next and delete the detached node
              curr = curr->get_next ();
              this->free_node (tempnode);
            }
          else
            curr = curr->get_next ();
        }
    }

  //  Look for a new earliest time

  // Defaults to zero.
  ACE_Time_Value earliest_time; 

  // Check every entry in the table
  for (i = 0; i < this->wheel_size_; i++)
    {
      // Skip empty entries
      if (this->wheel_[i]->get_next () != this->wheel_[i])
        {
          // if initialization or if the time is earlier
          if (earliest_time == ACE_Time_Value::zero
              || this->wheel_[i]->get_timer_value () < earliest_time)
            {
              earliest_time =
                this->wheel_[i]->get_next ()->get_timer_value ();
              this->earliest_pos_ = i;
            }
        }
    }

  if (skip_close == 0)
    this->upcall_functor ().cancellation (*this,
                                          type);
  return number_of_cancellations;
}


/**
 * Cancels the single timer that is specified by the timer_id.  In this
 * case the timer_id is actually a pointer to the node, so we cast it
 * to the node.  This can be dangerous if the timer_id is made up 
 * (or deleted twice) so we do a little sanity check.  Finally we update
 * the earliest time in case the earliest timer was removed.
 *
 * @param timer_id   Timer Identifier
 * @param act        Asychronous Completion Token (AKA magic cookie):
 *                   If this is non-zero, stores the magic cookie of
 *                   the cancelled timer here.
 * @param skip_close If this non-zero, the cancellation method of the
 *                   functor will not be called.
 * 
 * @return 1 for sucess and 0 if the timer_id wasn't found (or was
 *         found to be invalid)
 */
template <class TYPE, class FUNCTOR, class ACE_LOCK> int
ACE_Timer_Wheel_T<TYPE, FUNCTOR, ACE_LOCK>::cancel (long timer_id,
                                                    const void **act,
                                                    int skip_close)
{
  ACE_TRACE ("ACE_Timer_Wheel_T::cancel");
  ACE_MT (ACE_GUARD_RETURN (ACE_LOCK, ace_mon, this->mutex_, -1));

  // Make sure we are getting a valid <timer_id>, not an error
  // returned by <schedule>.
  if (timer_id == -1)
    return 0;

  ACE_Timer_Node_T<TYPE> *node =
    ACE_reinterpret_cast (ACE_Timer_Node_T<TYPE> *,
                          timer_id);

  // Check to see if the node looks like a true ACE_Timer_Node_T<TYPE>.
  if (timer_id == node->get_timer_id ())
    {
      node->get_next ()->set_prev (node->get_prev ());
      node->get_prev ()->set_next (node->get_next ());

      if (act != 0)
        *act = node->get_act ();

      if (skip_close == 0)
        this->upcall_functor ().cancellation (*this,
                                              node->get_type ());

      // Find out what position it is in.
      size_t pos = (node->get_timer_value ().usec () / this->resolution_) 
                   % this->wheel_size_;

      this->free_node (node);

      // Get the new earliest time if we have to

      if (pos == this->earliest_pos_)
        {
          ACE_Time_Value earliest_time; // defaults to zero

          // Check every entry in the table
          for (size_t i = 0; i < this->wheel_size_; i++)
            {
              // Skip empty entries
              if (this->wheel_[i]->get_next () != this->wheel_[i])
                {
                  // if initialization or if the time is earlier
                  if (earliest_time == ACE_Time_Value::zero
                      || this->wheel_[i]->get_timer_value () < earliest_time)
                    {
                      earliest_time =
                        this->wheel_[i]->get_next ()->get_timer_value ();
                      this->earliest_pos_ = i;
                    }
                }
            }
        }

      return 1;
    }

  // Didn't find it if we are here
  return 0;
}


/**
 * Dumps out the size of the wheel, the resolution, and the contents
 * of the wheel.
 */
template <class TYPE, class FUNCTOR, class ACE_LOCK> void
ACE_Timer_Wheel_T<TYPE, FUNCTOR, ACE_LOCK>::dump (void) const
{
  ACE_TRACE ("ACE_Timer_Wheel_T::dump");
  ACE_DEBUG ((LM_DEBUG, ACE_BEGIN_DUMP, this));

  ACE_DEBUG ((LM_DEBUG, 
             ACE_LIB_TEXT ("\nwheel_size_ = %d"), this->wheel_size_));
  ACE_DEBUG ((LM_DEBUG, 
             ACE_LIB_TEXT ("\nresolution_ = %d"), this->resolution_));
  ACE_DEBUG ((LM_DEBUG, 
             ACE_LIB_TEXT ("\nwheel_ = \n")));

  for (size_t i = 0; i < this->wheel_size_; i++)
    {
      ACE_DEBUG ((LM_DEBUG, ACE_LIB_TEXT ("%d\n"), i));
      ACE_Timer_Node_T<TYPE> *temp = this->wheel_[i]->get_next ();
      while (temp != this->wheel_[i])
        {
          temp->dump ();
          temp = temp->get_next ();
        }
    }

  ACE_DEBUG ((LM_DEBUG, ACE_END_DUMP));
}


/**
 * Removes the earliest node and then find the new <earliest_pos_>
 *
 * @return The earliest timer node.
 */
template <class TYPE, class FUNCTOR, class ACE_LOCK> ACE_Timer_Node_T<TYPE> *
ACE_Timer_Wheel_T<TYPE, FUNCTOR, ACE_LOCK>::remove_first (void)
{
  ACE_TRACE ("ACE_Timer_Wheel_T::remove_first");

  // Remove the item
  ACE_Timer_Node_T<TYPE> *temp =
    this->wheel_[this->earliest_pos_]->get_next ();
  temp->get_prev ()->set_next (temp->get_next ());
  temp->get_next ()->set_prev (temp->get_prev ());

  ACE_Time_Value earliest_time;

  // Check every entry in the table for the new earliest item
  for (size_t i = 0;
       i < this->wheel_size_;
       i++)
    {
      // Check for an empty entry
      if (this->wheel_[i]->get_next () != this->wheel_[i])
        {
          // if initialization or if the time is earlier
          if (earliest_time == ACE_Time_Value::zero
              || this->wheel_[i]->get_timer_value () < earliest_time)
            {
              earliest_time =
                this->wheel_[i]->get_next ()->get_timer_value ();
              this->earliest_pos_ = i;
            }
        }
    }

  return temp;
}


/**
 * Returns the earliest node without removing it
 * 
 * @return The earliest timer node.
 */
template <class TYPE, class FUNCTOR, class ACE_LOCK> ACE_Timer_Node_T<TYPE> *
ACE_Timer_Wheel_T<TYPE, FUNCTOR, ACE_LOCK>::get_first (void)
{ 
  ACE_TRACE ("ACE_Timer_Wheel_T::get_first");

  return this->wheel_[this->earliest_pos_]->get_next ();
}

/**
 * Takes an ACE_Timer_Node and inserts it into the correct position in
 * the correct list.  Also makes sure to update the earliest time.
 *
 * @param expired The timer node to reschedule
 */
template <class TYPE, class FUNCTOR, class ACE_LOCK> void
ACE_Timer_Wheel_T<TYPE, FUNCTOR, ACE_LOCK>::reschedule (
  ACE_Timer_Node_T<TYPE> *expired
  )
{
  ACE_TRACE ("ACE_Timer_Wheel_T::reschedule");

  size_t pos = (expired->get_timer_value ().usec () / this->resolution_) 
               % this->wheel_size_;

  // See if we need to update the earliest time
  if (this->is_empty ()
      || expired->get_timer_value () < this->earliest_time ())
    this->earliest_pos_ = pos;

  // Insert time into dummy node.
  this->wheel_[pos]->set_timer_value (expired->get_timer_value ());
  ACE_Timer_Node_T<TYPE> *cursor =
    this->wheel_[pos]->get_next ();

  // Find position to insert
  while (cursor->get_timer_value () < expired->get_timer_value ())
    cursor = cursor->get_next ();

  // Insert
  expired->set_prev (cursor->get_prev ());
  expired->set_next (cursor);
  cursor->set_prev (expired);
  expired->get_prev ()->set_next (expired);
}

/**
 * @return The iterator
 */
template <class TYPE, class FUNCTOR, class ACE_LOCK> 
ACE_Timer_Queue_Iterator_T<TYPE, FUNCTOR, ACE_LOCK> &
ACE_Timer_Wheel_T<TYPE, FUNCTOR, ACE_LOCK>::iter (void)
{
  this->iterator_->first ();
  return *this->iterator_;
}

/**
 * Dummy version of expire to get rid of warnings in Sun CC 4.2
 * Just call the expire of the base class.
 */
template <class TYPE, class FUNCTOR, class ACE_LOCK> int
ACE_Timer_Wheel_T<TYPE, FUNCTOR, ACE_LOCK>::expire ()
{
  return ACE_Timer_Queue_T<TYPE,FUNCTOR,ACE_LOCK>::expire ();
}

/**
 * This is a specialized version of expire that is more suited for the
 * internal data representation.  Notice that we are still expiring 
 * timers in order, even though this can be really speeded up if we 
 * didn't worry about this.  
 *
 * @param cur_time The time to expire timers up to.
 *
 * @return Number of timers expired
 */
template <class TYPE, class FUNCTOR, class ACE_LOCK> int
ACE_Timer_Wheel_T<TYPE, FUNCTOR, ACE_LOCK>::expire (
  const ACE_Time_Value &cur_time
  )
{
  ACE_TRACE ("ACE_Timer_Wheel_T::expire");
  ACE_MT (ACE_GUARD_RETURN (ACE_LOCK, ace_mon, this->mutex_, -1));

  int number_of_timers_expired = 0;
  size_t i;

  size_t earliest_pos = this->wheel_size_;
  ACE_Time_Value earliest_time = cur_time;
  
  size_t next_earliest_pos = this->wheel_size_;
  ACE_Time_Value next_earliest_time;

  // Find the earliest time and location
  for (i = 0; i < this->wheel_size_; i++)
    {
      if (this->wheel_[i]->get_next () != this->wheel_[i]
          && this->wheel_[i]->get_next ()->get_timer_value () 
             <= earliest_time)
        {
          earliest_pos = i;
          earliest_time = this->wheel_[i]->get_next ()->get_timer_value ();
        }
    }

  // Check to see if the timer queue is empty
  if (earliest_pos == this->wheel_size_)
    return 0;

  do 
    {
      next_earliest_time = cur_time;
      next_earliest_pos = this->wheel_size_;

      // Find the next earliest position and time.
      for (i = 0; i < this->wheel_size_; i++)
        {
          if (i != earliest_pos
              && this->wheel_[i]->get_next () != this->wheel_[i]
              && this->wheel_[i]->get_next ()->get_timer_value () 
                 <= next_earliest_time)
            {
              next_earliest_pos = i;
              next_earliest_time = 
                this->wheel_[i]->get_next ()->get_timer_value ();
            }
        }

      // Keep expiring timers until we need to move to the next list
      while (this->wheel_[earliest_pos]->get_next () 
               != this->wheel_[earliest_pos]
             && this->wheel_[earliest_pos]->get_next ()->get_timer_value () 
                <= next_earliest_time)
        {
          // Remove the first node in the earliest position
          ACE_Timer_Node_T<TYPE> *expired =
            this->wheel_[earliest_pos]->get_next ();
          this->wheel_[earliest_pos]->set_next (expired->get_next ());
          expired->get_next ()->set_prev (this->wheel_[earliest_pos]);

          TYPE &type = expired->get_type ();
          const void *act = expired->get_act ();
          int reclaim = 1;

          // Check if this is an interval timer.
          if (expired->get_interval () > ACE_Time_Value::zero)
            {
              // Make sure that we skip past values that have already
              // "expired".
              do
                expired->set_timer_value (expired->get_timer_value () 
                  + expired->get_interval ());
              while (expired->get_timer_value () <= cur_time);

              // Since this is an interval timer, we need to
              // reschedule it.
              this->reschedule (expired);
              reclaim = 0;
            }

          // Call the functor.
          this->upcall (type, act, cur_time);

          if (reclaim)
            // Free up the node and the token.
            this->free_node (expired);

          ++number_of_timers_expired;
        }

      earliest_pos = next_earliest_pos;
    } 
  while (earliest_pos != this->wheel_size_);

  //  Look for a new earliest time

  earliest_time = ACE_Time_Value::zero; 

  // Check every entry in the table
  for (i = 0; i < this->wheel_size_; i++)
    {
      // Skip empty entries
      if (this->wheel_[i]->get_next () != this->wheel_[i])
        {
          // if initialization or if the time is earlier
          if (earliest_time == ACE_Time_Value::zero
              || this->wheel_[i]->get_timer_value () < earliest_time)
            {
              earliest_time =
                this->wheel_[i]->get_next ()->get_timer_value ();
              this->earliest_pos_ = i;
            }
        }
    }

  return number_of_timers_expired;
}

#endif /* ACE_TIMER_WHEEL_T_C */
