// $Id$

#if !defined (ACE_TIMER_WHEEL_T_C)
#define ACE_TIMER_WHEEL_T_C

#define ACE_BUILD_DLL

#include "ace/Timer_Wheel_T.h"
#include "ace/High_Res_Timer.h"


// Constructor that takes in a <wheel>, a reference to the timer queue

template <class TYPE, class FUNCTOR, class LOCK>
ACE_Timer_Wheel_Iterator_T<TYPE, FUNCTOR, LOCK>::ACE_Timer_Wheel_Iterator_T (ACE_Timer_Wheel_T<TYPE, FUNCTOR, LOCK> &wheel)
  : timer_wheel_ (wheel)
{
  // Nothing
}


// Positions the iterator at the first node in the timing wheel

template <class TYPE, class FUNCTOR, class LOCK> void 
ACE_Timer_Wheel_Iterator_T<TYPE, FUNCTOR, LOCK>::first (void)
{
  for (this->pos_ = 0; 
       this->pos_ < this->timer_wheel_.wheel_size_; 
       this->pos_++)
    {
      // Skip over empty entries
      if (this->timer_wheel_.wheel_[this->pos_]->get_next () != 
            this->timer_wheel_.wheel_[this->pos_])
        {          
          this->list_item_ = this->timer_wheel_.wheel_[this->pos_]->get_next ();
          return;
        }
    }

  // The queue is empty if we are here
  this->list_item_ = NULL;
}


// Positions the iterator at the next node in list or goes to the next
// list

template <class TYPE, class FUNCTOR, class LOCK> void 
ACE_Timer_Wheel_Iterator_T<TYPE, FUNCTOR, LOCK>::next (void)
{
  if (this->isdone ())
    return;

  this->list_item_ = this->list_item_->get_next ();

  // If there is no more in the current list, go to the next
  if (this->list_item_ == this->timer_wheel_.wheel_[this->pos_])
    {
      for (this->pos_++; this->pos_ < this->timer_wheel_.wheel_size_; this->pos_++)
        {
          // Check for an empty entry
          if (this->timer_wheel_.wheel_[this->pos_]->get_next () != 
                this->timer_wheel_.wheel_[this->pos_])
            {
              this->list_item_ = this->timer_wheel_.wheel_[this->pos_]->get_next ();
              return;
            }
        }
  
      this->list_item_ = NULL;
    }
}


// Returns true when we are at the end (when list_item_ == 0)

template <class TYPE, class FUNCTOR, class LOCK> int 
ACE_Timer_Wheel_Iterator_T<TYPE, FUNCTOR, LOCK>::isdone (void)
{
  return this->list_item_ == NULL;
}


// Returns the node at the current position in the sequence

template <class TYPE, class FUNCTOR, class LOCK> ACE_Timer_Node_T<TYPE> *
ACE_Timer_Wheel_Iterator_T<TYPE, FUNCTOR, LOCK>::item (void)
{
  if (this->isdone ())
    return NULL;

  return this->list_item_;
}


// Constructor that sets up the timing wheel and also may preallocate some
// nodes on the free list

template <class TYPE, class FUNCTOR, class LOCK>
ACE_Timer_Wheel_T<TYPE, FUNCTOR, LOCK>::ACE_Timer_Wheel_T (size_t wheelsize, 
							   size_t resolution, 
							   size_t prealloc,
							   FUNCTOR *upcall_functor,
                                                           ACE_Free_List<ACE_Timer_Node_T <TYPE> > *freelist)
  : INHERITED (upcall_functor, freelist),
    wheel_size_ (wheelsize),
    resolution_ (resolution),
    earliest_pos_ (0),
    iterator_ (*this)
{
  ACE_TRACE ("ACE_Timer_Wheel_T::ACE_Timer_Wheel_T");
  size_t i;

  this->gettimeofday (ACE_OS::gettimeofday);

  // Create the timing wheel
  ACE_NEW (this->wheel_, (ACE_Timer_Node_T<TYPE> *[wheelsize]));


  // Create the dummy nodes
  for (i = 0; i < wheelsize; i++)
    {
      ACE_Timer_Node_T<TYPE> *tempnode = this->alloc_node ();
      tempnode->set_next (tempnode);
      tempnode->set_prev (tempnode);
      this->wheel_[i] = tempnode;
    }

  // Do the preallocation
  this->free_list_->resize (prealloc);
}

template <class TYPE, class FUNCTOR, class LOCK>
ACE_Timer_Wheel_T<TYPE, FUNCTOR, LOCK>::ACE_Timer_Wheel_T (FUNCTOR *upcall_functor,
                                                           ACE_Free_List<ACE_Timer_Node_T <TYPE> > *freelist)
  : INHERITED (upcall_functor, freelist),
    wheel_size_ (ACE_DEFAULT_TIMER_WHEEL_SIZE),
    resolution_ (ACE_DEFAULT_TIMER_WHEEL_RESOLUTION),
    earliest_pos_ (0),
    iterator_ (*this)
{
  ACE_TRACE ("ACE_Timer_Wheel_T::ACE_Timer_Wheel_T");
  size_t i;

  this->gettimeofday (ACE_OS::gettimeofday);

  // Create the timing wheel
  ACE_NEW (this->wheel_, (ACE_Timer_Node_T<TYPE> *[this->wheel_size_]));

  // Create the dummy nodes
  for (i = 0; i < this->wheel_size_; i++)
    {
      ACE_Timer_Node_T<TYPE> *tempnode = this->alloc_node ();
      tempnode->set_next (tempnode);
      tempnode->set_prev (tempnode);
      this->wheel_[i] = tempnode;
    }
}

// Destructor just cleans up its memory

template <class TYPE, class FUNCTOR, class LOCK>
ACE_Timer_Wheel_T<TYPE, FUNCTOR, LOCK>::~ACE_Timer_Wheel_T (void)
{
  ACE_TRACE ("ACE_Timer_Wheel_T::~ACE_Timer_Wheel_T");

  for (size_t i = 0; i < this->wheel_size_; i++)
    {
      // delete nodes until only the dummy node is left
      while (this->wheel_[i]->get_next () != this->wheel_[i])
	{
	  ACE_Timer_Node_T<TYPE> *next = this->wheel_[i]->get_next ();
	  this->wheel_[i]->set_next (next->get_next ());
	  next->get_next ()->set_prev (this->wheel_[i]);
          this->upcall_functor ().deletion (*this, next->get_type (), next->get_act ());
          this->free_node (next);
	}

      // and now delete the dummy node
      delete this->wheel_[i];
    }

  // finally delete the wheel
  delete [] this->wheel_;
}


// Checks to see if <earliest_pos> points to a empty list (then it is empty)

template <class TYPE, class FUNCTOR, class LOCK> int 
ACE_Timer_Wheel_T<TYPE, FUNCTOR, LOCK>::is_empty (void) const
{
  ACE_TRACE ("ACE_Timer_Wheel_T::is_empty");
  
  return this->wheel_[this->earliest_pos_]->get_next () == this->wheel_[this->earliest_pos_];
}


// Returns the first (earliest) node in the <wheel_>'s <earliest_pos_> list

template <class TYPE, class FUNCTOR, class LOCK> const ACE_Time_Value &
ACE_Timer_Wheel_T<TYPE, FUNCTOR, LOCK>::earliest_time (void) const
{
  ACE_TRACE ("ACE_Timer_Wheel_T::earliest_time");

  if (this->is_empty ())
    return ACE_Time_Value::zero;
  else
    return this->wheel_[this->earliest_pos_]->get_next ()->get_timer_value ();
}

// Create the node and pass it to reschedule.  Also check to see if the
// <earliest_pos> should be changed.

template <class TYPE, class FUNCTOR, class LOCK> long
ACE_Timer_Wheel_T<TYPE, FUNCTOR, LOCK>::schedule (const TYPE &type, 
						  const void *act,
						  const ACE_Time_Value &delay,
						  const ACE_Time_Value &interval)
{
  ACE_TRACE ("ACE_Timer_Wheel_T::schedule");
  ACE_MT (ACE_GUARD_RETURN (LOCK, ace_mon, this->mutex_, -1));

  ACE_Timer_Node_T<TYPE> *tempnode = this->alloc_node ();

  if (tempnode)
    {
      // Note that the timer_id is actually the pointer to the node
    
      // Set the details of the node
      tempnode->set (type,
                     act,
                     delay,
                     interval,
                     NULL,
                     NULL,
                     (long) tempnode);

      // Reschedule will insert it into the correct position
      this->reschedule (tempnode);

      return tempnode->get_timer_id ();
    }

  // Failure return
  errno = ENOMEM;
  return -1;
}


// Goes through every list in the wheel and if it finds a node with <type>
// then it removes the node and continues on looking for other nodes

template <class TYPE, class FUNCTOR, class LOCK> int
ACE_Timer_Wheel_T<TYPE, FUNCTOR, LOCK>::cancel (const TYPE &type,
						int dont_call_handle_close)
{
  ACE_TRACE ("ACE_Timer_Wheel_T::cancel");
  ACE_MT (ACE_GUARD_RETURN (LOCK, ace_mon, this->mutex_, -1));

  int number_of_cancellations = 0;
  size_t i;

  // Walk through the wheel
  for (i = 0; i < this->wheel_size_; i++)
    {
      ACE_Timer_Node_T<TYPE> *curr = this->wheel_[i]->get_next ();
  
      // Walk through the list
      while (curr != this->wheel_[i])
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
	    {
	      curr = curr->get_next ();
	    }
	}
    }

  //  Look for a new earliest time
  
  ACE_Time_Value earliest_time; // defaults to zero

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
	      earliest_time = this->wheel_[i]->get_next ()->get_timer_value ();
	      this->earliest_pos_ = i;
	    }
	}
    }

  if (dont_call_handle_close == 0) 
    this->upcall_functor ().cancellation (*this, type);

  return number_of_cancellations;
}


// Takes the <timer_id> and casts it to a pointer.  Then it removes it 
// from its neighbors 

template <class TYPE, class FUNCTOR, class LOCK> int
ACE_Timer_Wheel_T<TYPE, FUNCTOR, LOCK>::cancel (long timer_id,
						const void **act,
						int dont_call_handle_close)
{
  ACE_TRACE ("ACE_Timer_Wheel_T::cancel");
  ACE_MT (ACE_GUARD_RETURN (LOCK, ace_mon, this->mutex_, -1));

  // Make sure we are getting a valid <timer_id>, not an error
  // returned by schedule ()
  if (timer_id == -1)
    return 0;
  
  ACE_Timer_Node_T<TYPE> *node = (ACE_Timer_Node_T<TYPE> *) timer_id;
  
  // Check to see if the node looks like a true ACE_Timer_Node_T<TYPE>
  if (timer_id == node->get_timer_id ())
    {
      node->get_next ()->set_prev (node->get_prev ());
      node->get_prev ()->set_next (node->get_next ());
    
      if (act != 0)
	*act = node->get_act ();

      if (dont_call_handle_close == 0)
	this->upcall_functor ().cancellation (*this, node->get_type ());

      // Find out what position it is in
      size_t pos = (node->get_timer_value ().usec () / this->resolution_) % this->wheel_size_;
    
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
	              earliest_time = this->wheel_[i]->get_next ()->get_timer_value ();
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


// Dumps out some properties of this object
  
template <class TYPE, class FUNCTOR, class LOCK> void 
ACE_Timer_Wheel_T<TYPE, FUNCTOR, LOCK>::dump (void) const
{
  ACE_TRACE ("ACE_Timer_Wheel_T::dump");
  ACE_DEBUG ((LM_DEBUG, ACE_BEGIN_DUMP, this));

  ACE_DEBUG ((LM_DEBUG, "\nwheel_size_ = %d", this->wheel_size_));
  ACE_DEBUG ((LM_DEBUG, "\nresolution_ = %d", this->resolution_));
  ACE_DEBUG ((LM_DEBUG, "\nwheel_ = \n"));

  for (size_t i = 0; i < this->wheel_size_; i++)
    {
      ACE_DEBUG ((LM_DEBUG, "%d\n", i));
      ACE_Timer_Node_T<TYPE> *temp = this->wheel_[i]->get_next ();
      while (temp != this->wheel_[i])
	{
	  temp->dump ();
	  temp = temp->get_next ();
	}
    }

  ACE_DEBUG ((LM_DEBUG, ACE_END_DUMP));
}


// Removes the earliest node and then find the new <earliest_pos_>

template <class TYPE, class FUNCTOR, class LOCK> ACE_Timer_Node_T<TYPE> *
ACE_Timer_Wheel_T<TYPE, FUNCTOR, LOCK>::remove_first (void)
{
  ACE_TRACE ("ACE_Timer_Wheel_T::remove_first");

  // Remove the item
  ACE_Timer_Node_T<TYPE> *temp = this->wheel_[this->earliest_pos_]->get_next ();
  temp->get_prev ()->set_next (temp->get_next ());
  temp->get_next ()->set_prev (temp->get_prev ());

  ACE_Time_Value earliest_time;
  
  // Check every entry in the table for the new earliest item
  for (size_t i = 0; i < this->wheel_size_; i++)
    {
      // Check for an empty entry
      if (this->wheel_[i]->get_next () != this->wheel_[i])
	{
	  // if initialization or if the time is earlier
	  if (earliest_time == ACE_Time_Value::zero
	      || this->wheel_[i]->get_timer_value () < earliest_time)
	    {
	      earliest_time = this->wheel_[i]->get_next ()->get_timer_value ();
	      this->earliest_pos_ = i;
	    }
	}
    }


  return temp;
}


// Takes an ACE_Timer_Node and inserts it into the correct position in the correct
// list

template <class TYPE, class FUNCTOR, class LOCK> void 
ACE_Timer_Wheel_T<TYPE, FUNCTOR, LOCK>::reschedule (ACE_Timer_Node_T<TYPE> *expired)
{
  ACE_TRACE ("ACE_Timer_Wheel_T::reschedule");

  size_t pos = (expired->get_timer_value ().usec () / this->resolution_) % this->wheel_size_;

  // See if we need to update the earliest time
  if (this->earliest_time () == ACE_Time_Value::zero
      || this->earliest_time () < expired->get_timer_value ())
    this->earliest_pos_ = pos;

  // Insert time into dummy node
  this->wheel_[pos]->set_timer_value (expired->get_timer_value ());
  ACE_Timer_Node_T<TYPE> *cursor = this->wheel_[pos]->get_next ();

  // Find position to insert
  while (cursor->get_timer_value () < expired->get_timer_value ())
    cursor = cursor->get_next ();

  // Insert
  expired->set_prev (cursor->get_prev ());
  expired->set_next (cursor);
  cursor->set_prev (expired);
  expired->get_prev ()->set_next (expired);
}


// Just return the iterator

template <class TYPE, class FUNCTOR, class LOCK> ACE_Timer_Queue_Iterator_T<TYPE, FUNCTOR, LOCK> &
ACE_Timer_Wheel_T<TYPE, FUNCTOR, LOCK>::iter (void)
{
  return this->iterator_;
}

// Dummy version of expire to get rid of warnings in Sun CC 4.2

template <class TYPE, class FUNCTOR, class LOCK> int
ACE_Timer_Wheel_T<TYPE, FUNCTOR, LOCK>::expire ()
{
  return INHERITED::expire ();
}


// Specialized expire which expires in total order.  It is optimized by keeping 
// track of the list with the earliest element and the next earliest list.  It 
// then goes through the earliest list until it can switch to the second list.
// it keeps going until it finishes with everything before the <cur_time>

template <class TYPE, class FUNCTOR, class LOCK> int
ACE_Timer_Wheel_T<TYPE, FUNCTOR, LOCK>::expire (const ACE_Time_Value &cur_time)
{
  ACE_TRACE ("ACE_Timer_Wheel_T::expire");
  ACE_MT (ACE_GUARD_RETURN (LOCK, ace_mon, this->mutex_, -1));

  int number_of_timers_expired = 0;
  size_t i; 
  size_t earliest = this->wheel_size_;
  ACE_Time_Value earliest_time = cur_time;
  size_t next_earliest = this->wheel_size_;
  ACE_Time_Value next_earliest_time;

  // Find the earliest time
  for (i = 0; i < this->wheel_size_; i++)
    {
      if (this->wheel_[i]->get_next () != this->wheel_[i]
          && this->wheel_[i]->get_next ()->get_timer_value () <= earliest_time)
        {
          earliest = i;
          earliest_time = this->wheel_[i]->get_next ()->get_timer_value ();
        }
    }

  // Check to see if there is nothing to expire
  if (earliest == this->wheel_size_) 
    return 0;

  do
    {
      next_earliest_time = cur_time;
      next_earliest = this->wheel_size_;

      // Find 2nd earliest position
      for (i = 0; i < this->wheel_size_; i++)
        {
          if (i != earliest
              && this->wheel_[i]->get_next () != this->wheel_[i]            
              && this->wheel_[i]->get_next ()->get_timer_value () <= next_earliest_time)
            {
              next_earliest = i;
              next_earliest_time = this->wheel_[i]->get_next ()->get_timer_value ();
            }
        }

      while (this->wheel_[earliest]->get_next ()->get_timer_value () <= next_earliest_time)
        {
          // Remove the first node in the earliest position
          ACE_Timer_Node_T<TYPE> *expired = this->wheel_[earliest]->get_next ();
          this->wheel_[earliest]->set_next (expired->get_next ());
          expired->get_next ()->set_prev (this->wheel_[earliest]);
          
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
            // Free up the node and the token
            this->free_node (expired);
    
          ++number_of_timers_expired;

          // Check to see if we are empty
          if (this->wheel_[earliest]->get_next () == this->wheel_[earliest])
            break;
        }

      if (next_earliest_time == this->wheel_size_)
        break;

      earliest = next_earliest;
    } while (next_earliest != this->wheel_size_);

  return number_of_timers_expired;
}


#endif /* ACE_TIMER_WHEEL_T_C */
