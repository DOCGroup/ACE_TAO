#if !defined (ACE_TIMER_WHEEL_T_C)
#define ACE_TIMER_WHEEL_T_C

#define ACE_BUILD_DLL

#include "ace/Timer_Wheel_T.h"

template <class TYPE, class FUNCTOR>
ACE_Timer_Wheel_Iterator_T<TYPE, FUNCTOR>::ACE_Timer_Wheel_Iterator_T (ACE_Timer_Wheel_T<TYPE, FUNCTOR> &wheel)
  : timer_wheel_ (wheel)
{
}

template <class TYPE, class FUNCTOR> int
ACE_Timer_Wheel_Iterator_T<TYPE, FUNCTOR>::next (ACE_Timer_Node_T<TYPE, FUNCTOR> *&node,
	                                         const ACE_Time_Value &cur_time)
{
  if (this->timer_wheel_.wheel_[this->pos_]->next_ == this->timer_wheel_.wheel_[this->pos_]
    || this->timer_wheel_.wheel_[this->pos_]->next_->timer_value_ > this->time_)
  {
    ACE_Time_Value et = this->timer_wheel_.earliest_time ();

    if (this->timer_wheel_.size_ == 0 || et > cur_time)
      return 0;

    this->pos_ = (et.usec () / this->timer_wheel_.resolution_) 
                 % this->timer_wheel_.wheel_size_;
    this->time_ = ACE_Time_Value (et.sec (), 
                                  this->pos_ * this->timer_wheel_.resolution_);
  }

  // Remove the earliest item

  node = this->timer_wheel_.wheel_[this->pos_]->next_;
  node->prev_->next_ = node->next_;
  node->next_->prev_ = node->prev_;
  this->timer_wheel_.size_--;
  return 1;
}

template <class TYPE, class FUNCTOR> void
ACE_Timer_Wheel_Iterator_T<TYPE, FUNCTOR>::reset (void)
{
  ACE_Time_Value et = this->timer_wheel_.earliest_time ();

  this->pos_ = (et.usec () / this->timer_wheel_.resolution_) 
    % this->timer_wheel_.wheel_size_;
  this->time_ = ACE_Time_Value (et.sec (), 
                                this->pos_ * this->timer_wheel_.resolution_);
}

ACE_ALLOC_HOOK_DEFINE(ACE_Timer_Wheel_T)

template <class TYPE, class FUNCTOR>
ACE_Timer_Wheel_T<TYPE, FUNCTOR>::ACE_Timer_Wheel_T (size_t wheelsize, 
                                                     size_t resolution, 
                                                     size_t prealloc,
                                                     FUNCTOR *upcall_functor)
  : INHERITED (upcall_functor),
    wheel_size_ (wheelsize),
    resolution_ (resolution),
    current_pos_ ((ACE_OS::gettimeofday ().usec () / resolution) % wheel_size_),
    current_time_ (ACE_OS::gettimeofday ()),
    size_ (0),
    iterator_ (*this),
    freelist_ (NULL)
{
  ACE_TRACE ("ACE_Timer_Wheel_T::ACE_Timer_Wheel_T");
  size_t i;

  // Create the timing wheel
  ACE_NEW (this->wheel_, (ACE_Timer_Node_T<TYPE, FUNCTOR> *[wheelsize]));

  for (i = 0; i < wheelsize; i++)
  {
    // Create the dummy nodes
    ACE_Timer_Node_T<TYPE, FUNCTOR> *tempnode = this->alloc_node ();
    tempnode->next_ = tempnode->prev_ = tempnode;
    this->wheel_[i] = tempnode;
  }

  // Do the preallocation
  for (i = 0; i < prealloc; i++)
  {
    ACE_Timer_Node_T<TYPE, FUNCTOR> *temp;
    ACE_NEW (temp, (ACE_Timer_Node_T<TYPE, FUNCTOR>));
    temp->next_ = this->freelist_;
    this->freelist_ = temp;
  }
}


template <class TYPE, class FUNCTOR>
ACE_Timer_Wheel_T<TYPE, FUNCTOR>::~ACE_Timer_Wheel_T (void)
{
  ACE_TRACE ("ACE_Timer_Wheel_T::~ACE_Timer_Wheel_T");

  for (size_t i = 0; i < this->wheel_size_; i++)
  {
    // delete nodes until only the dummy node is left
    while (this->wheel_[i]->next_ != this->wheel_[i])
    {
      ACE_Timer_Node_T<TYPE, FUNCTOR> *next = this->wheel_[i]->next_;
      this->wheel_[i]->next_ = next->next_;
      next->next_->prev_ = this->wheel_[i];
      this->free_node (next);
    }

    // and now delete the dummy node
    delete this->wheel_[i];
  }

  // Get rid of the freelist now
  while (this->freelist_ != NULL)
  {
    ACE_Timer_Node_T<TYPE, FUNCTOR> *temp = this->freelist_;
    this->freelist_ = temp->next_;
    delete temp;
  }

  delete [] this->wheel_;
}

template <class TYPE, class FUNCTOR> int 
ACE_Timer_Wheel_T<TYPE, FUNCTOR>::is_empty (void) const
{
  ACE_TRACE ("ACE_Timer_Wheel_T::is_empty");
  
  // Just check the size
  return this->size_ == 0;
}

template <class TYPE, class FUNCTOR> const ACE_Time_Value &
ACE_Timer_Wheel_T<TYPE, FUNCTOR>::earliest_time (void) const
{
  ACE_TRACE ("ACE_Timer_Wheel_T::earliest_time");

  ACE_Time_Value earliest_time;
  size_t earliest_pos = 0;

  // Check every entry in the table
  for (size_t i = 0; i < this->wheel_size_; i++)
  {
    // Check for an empty entry
    if (this->wheel_[i]->next_ != this->wheel_[i])
    {
      // if initialization or if the time is earlier
      if (earliest_time == ACE_Time_Value::zero
          || this->wheel_[i]->timer_value_ < earliest_time)
      {
        earliest_time = this->wheel_[i]->next_->timer_value_;
        earliest_pos = i;
      }
    }
  }

  return this->wheel_[earliest_pos]->next_->timer_value_;
}


template <class TYPE, class FUNCTOR> long
ACE_Timer_Wheel_T<TYPE, FUNCTOR>::schedule (const TYPE &type, 
                                            const void *act,
                                            const ACE_Time_Value &delay,
                                            const ACE_Time_Value &interval)
{
  ACE_TRACE ("ACE_Timer_Wheel_T::schedule");
  ACE_MT (ACE_GUARD_RETURN (ACE_Recursive_Thread_Mutex, ace_mon, this->lock_, -1));

  ACE_Timer_Node_T<TYPE, FUNCTOR> *tempnode = this->alloc_node ();

  if (tempnode)
    {
      // Note that the timer_id is actually the pointer to the node
    
      // Use operator placement new.
      new (tempnode) ACE_Timer_Node_T<TYPE, FUNCTOR> (type,
						      act,
						      delay,
						      interval,
						      NULL,
						      NULL,
						      (long) tempnode);

      this->reschedule (tempnode);
      return tempnode->timer_id_;
    }

  // Failure return.
  errno = ENOMEM;
  return -1;
}

template <class TYPE, class FUNCTOR> int
ACE_Timer_Wheel_T<TYPE, FUNCTOR>::cancel (const TYPE &type,
                                          int dont_call_handle_close)
{
  ACE_TRACE ("ACE_Timer_Wheel_T::cancel");
  ACE_MT (ACE_GUARD_RETURN (ACE_Recursive_Thread_Mutex, ace_mon, this->lock_, -1));

  int number_of_cancellations = 0;

  for (size_t i = 0; i < this->wheel_size_; i++)
  {
    ACE_Timer_Node_T<TYPE, FUNCTOR> *curr = this->wheel_[i]->next_;
  
    while (curr != this->wheel_[i])
    {
      if (curr->type_ == type)
      {
        number_of_cancellations++;

        if (dont_call_handle_close == 0 
            && number_of_cancellations == 1)	  
          // This calls the correct operator () on the functor (the one with
          // two args)
          this->upcall_functor_.operator () (*this, curr->type_);

        ACE_Timer_Node_T<TYPE, FUNCTOR> *tempnode = curr;
        curr->prev_->next_ = curr->next_;
        curr->next_->prev_ = curr->prev_;
        curr = curr->next_;

        this->free_node (tempnode);
      }
      else
      {
        curr = curr->next_;
      }
    }
  }

  this->size_ -= number_of_cancellations;
  return number_of_cancellations;
}

template <class TYPE, class FUNCTOR> int
ACE_Timer_Wheel_T<TYPE, FUNCTOR>::cancel (long timer_id,
                                          const void **act,
                                          int dont_call_handle_close)
{
  ACE_TRACE ("ACE_Timer_Wheel_T::cancel");
  ACE_MT (ACE_GUARD_RETURN (ACE_Recursive_Thread_Mutex, ace_mon, this->lock_, -1));

  ACE_Timer_Node_T<TYPE, FUNCTOR> *node = (ACE_Timer_Node_T<TYPE, FUNCTOR> *) timer_id;
  
  ACE_ASSERT (timer_id == node->timer_id_);

  // Check to see if the node looks like a true ACE_Timer_Node_T<TYPE, FUNCTOR>
  if (timer_id == node->timer_id_)
  {
    node->next_->prev_ = node->prev_;
    node->prev_->next_ = node->next_;
    
    if (act != 0)
      *act = node->act_;

    if (dont_call_handle_close == 0)
      // This calls the correct operator () on the functor (the one with
      // two args)
      this->upcall_functor_.operator () (*this, node->type_);
    
    this->free_node (node);
    this->size_--;
    return 1;
  }

  // Didn't find it if we are here
  return 0;
}
  
template <class TYPE, class FUNCTOR> void 
ACE_Timer_Wheel_T<TYPE, FUNCTOR>::dump (void) const
{
  ACE_TRACE ("ACE_Timer_Wheel_T::dump");
  ACE_DEBUG ((LM_DEBUG, ACE_BEGIN_DUMP, this));

  ACE_DEBUG ((LM_DEBUG, "\nwheel_size_ = %d", this->wheel_size_));
  ACE_DEBUG ((LM_DEBUG, "\nresolution_ = %d", this->resolution_));
  ACE_DEBUG ((LM_DEBUG, "\ncurrent_pos_ = %d", this->current_pos_));
  ACE_DEBUG ((LM_DEBUG, "\nsize_ = %d", this->size_));
  ACE_DEBUG ((LM_DEBUG, "\nwheel_ = \n"));

  for (size_t i = 0; i < this->wheel_size_; i++)
  {
    ACE_DEBUG ((LM_DEBUG, "%d\n", i));
    ACE_Timer_Node_T<TYPE, FUNCTOR> *temp = this->wheel_[i]->next_;
    while (temp != this->wheel_[i])
    {
      temp->dump ();
      temp = temp->next_;
    }
  }

  ACE_DEBUG ((LM_DEBUG, ACE_END_DUMP));
}

template <class TYPE, class FUNCTOR> ACE_Timer_Node_T<TYPE, FUNCTOR> *
ACE_Timer_Wheel_T<TYPE, FUNCTOR>::alloc_node (void)
{
  ACE_Timer_Node_T<TYPE, FUNCTOR> *temp;

  if (this->freelist_ == NULL)
  {
    ACE_NEW_RETURN (temp,
                    (ACE_Timer_Node_T<TYPE, FUNCTOR>),
                    0);
  }
  else
  {
    temp = this->freelist_;
    this->freelist_ = temp->next_;
  }
  
  return temp;
}

template <class TYPE, class FUNCTOR> void 
ACE_Timer_Wheel_T<TYPE, FUNCTOR>::free_node (ACE_Timer_Node_T<TYPE, FUNCTOR> *node)
{
  // Add to the beginning of the freelist
  node->next_ = this->freelist_;
  this->freelist_ = node;

  // Change the timer id so a cancel on this would not try to 
  // delete it
  node->timer_id_ = 0; 
}

template <class TYPE, class FUNCTOR> ACE_Timer_Node_T<TYPE, FUNCTOR> *
ACE_Timer_Wheel_T<TYPE, FUNCTOR>::remove (void)
{
  ACE_TRACE ("ACE_Timer_Wheel_T::remove");

  ACE_Time_Value earliest_time;
  size_t earliest_pos = 0;

  // Check every entry in the table
  for (size_t i = 0; i < this->wheel_size_; i++)
  {
    // Check for an empty entry
    if (this->wheel_[i]->next_ != this->wheel_[i])
    {
      // if initialization or if the time is earlier
      if (earliest_time == ACE_Time_Value::zero
          || this->wheel_[i]->timer_value_ < earliest_time)
      {
        earliest_time = this->wheel_[i]->next_->timer_value_;
        earliest_pos = i;
      }
    }
  }

  // Remove and return
  ACE_Timer_Node_T<TYPE, FUNCTOR> *temp = this->wheel_[earliest_pos]->next_;
  temp->prev_->next_ = temp->next_;
  temp->next_->prev_ = temp->prev_;

  this->size_--;

  return temp;
}

template <class TYPE, class FUNCTOR> void 
ACE_Timer_Wheel_T<TYPE, FUNCTOR>::reschedule (ACE_Timer_Node_T<TYPE, FUNCTOR> *expired)
{
  ACE_TRACE ("ACE_Timer_Wheel_T::reschedule");

  size_t pos = (expired->timer_value_.usec () / this->resolution_) % this->wheel_size_;

  // Insert time into dummy node
  this->wheel_[pos]->timer_value_ = expired->timer_value_;
  ACE_Timer_Node_T<TYPE, FUNCTOR> *cursor = this->wheel_[pos]->next_;

  // Find position to insert
  while (cursor->timer_value_ < expired->timer_value_)
    cursor = cursor->next_;

  // Insert
  expired->prev_ = cursor->prev_;
  expired->next_ = cursor;
  cursor->prev_ = expired;
  expired->prev_->next_ = expired;

  this->size_++;
}

template <class TYPE, class FUNCTOR> ACE_Timer_Queue_Iterator_T<TYPE, FUNCTOR> &
ACE_Timer_Wheel_T<TYPE, FUNCTOR>::iter (void)
{
  this->iterator_.reset ();
  return this->iterator_;
}

#endif /* ACE_TIMER_WHEEL_T_C */
