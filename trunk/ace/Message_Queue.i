/* -*- C++ -*- */
// $Id$

// Message_Queue.i

template <ACE_SYNCH_1> ACE_INLINE ACE_Notification_Strategy *
ACE_Message_Queue<ACE_SYNCH_2>::notification_strategy (void)
{
  ACE_TRACE ("ACE_Message_Queue<ACE_SYNCH_2>::notification_strategy");

  return this->notification_strategy_;
}

template <ACE_SYNCH_1> ACE_INLINE void
ACE_Message_Queue<ACE_SYNCH_2>::notification_strategy (ACE_Notification_Strategy *s)
{
  ACE_TRACE ("ACE_Message_Queue<ACE_SYNCH_2>::notification_strategy");

  this->notification_strategy_ = s;
}

// Check if queue is empty (does not hold locks). 

template <ACE_SYNCH_1> ACE_INLINE int
ACE_Message_Queue<ACE_SYNCH_2>::is_empty_i (void)
{
  ACE_TRACE ("ACE_Message_Queue<ACE_SYNCH_2>::is_empty_i");
  return this->cur_bytes_ <= 0 && this->cur_count_ <= 0;
}

// Check if queue is full (does not hold locks). 

template <ACE_SYNCH_1> ACE_INLINE int 
ACE_Message_Queue<ACE_SYNCH_2>::is_full_i (void)
{
  ACE_TRACE ("ACE_Message_Queue<ACE_SYNCH_2>::is_full_i");
  return this->cur_bytes_ > this->high_water_mark_;
}

// Check if queue is empty (holds locks).

template <ACE_SYNCH_1> ACE_INLINE int 
ACE_Message_Queue<ACE_SYNCH_2>::is_empty (void)
{
  ACE_TRACE ("ACE_Message_Queue<ACE_SYNCH_2>::is_empty");
  ACE_GUARD_RETURN (ACE_SYNCH_MUTEX_T, ace_mon, this->lock_, -1);

  return this->is_empty_i ();
}

// Check if queue is full (holds locks).

template <ACE_SYNCH_1> ACE_INLINE int 
ACE_Message_Queue<ACE_SYNCH_2>::is_full (void)
{
  ACE_TRACE ("ACE_Message_Queue<ACE_SYNCH_2>::is_full");
  ACE_GUARD_RETURN (ACE_SYNCH_MUTEX_T, ace_mon, this->lock_, -1);

  return this->is_full_i ();
}

template <ACE_SYNCH_1> ACE_INLINE size_t
ACE_Message_Queue<ACE_SYNCH_2>::high_water_mark (void)
{
  ACE_TRACE ("ACE_Message_Queue<ACE_SYNCH_2>::high_water_mark");
  ACE_GUARD_RETURN (ACE_SYNCH_MUTEX_T, ace_mon, this->lock_, 0);

  return this->high_water_mark_;
}

template <ACE_SYNCH_1> ACE_INLINE void 
ACE_Message_Queue<ACE_SYNCH_2>::high_water_mark (size_t hwm)
{
  ACE_TRACE ("ACE_Message_Queue<ACE_SYNCH_2>::high_water_mark");
  ACE_GUARD (ACE_SYNCH_MUTEX_T, ace_mon, this->lock_);

  this->high_water_mark_ = hwm;
}

template <ACE_SYNCH_1> ACE_INLINE size_t
ACE_Message_Queue<ACE_SYNCH_2>::low_water_mark (void)
{
  ACE_TRACE ("ACE_Message_Queue<ACE_SYNCH_2>::low_water_mark");
  ACE_GUARD_RETURN (ACE_SYNCH_MUTEX_T, ace_mon, this->lock_, 0);

  return this->low_water_mark_;
}

template <ACE_SYNCH_1> ACE_INLINE void 
ACE_Message_Queue<ACE_SYNCH_2>::low_water_mark (size_t lwm)
{
  ACE_TRACE ("ACE_Message_Queue<ACE_SYNCH_2>::low_water_mark");
  ACE_GUARD (ACE_SYNCH_MUTEX_T, ace_mon, this->lock_);

  this->low_water_mark_ = lwm;
}

// Return the current number of bytes in the queue.

template <ACE_SYNCH_1> ACE_INLINE size_t
ACE_Message_Queue<ACE_SYNCH_2>::message_bytes (void)
{ 
  ACE_TRACE ("ACE_Message_Queue<ACE_SYNCH_2>::message_bytes");
  ACE_GUARD_RETURN (ACE_SYNCH_MUTEX_T, ace_mon, this->lock_, 0);

  return this->cur_bytes_;
}

// Return the current number of messages in the queue.

template <ACE_SYNCH_1> ACE_INLINE size_t
ACE_Message_Queue<ACE_SYNCH_2>::message_count (void)
{
  ACE_TRACE ("ACE_Message_Queue<ACE_SYNCH_2>::message_count");
  ACE_GUARD_RETURN (ACE_SYNCH_MUTEX_T, ace_mon, this->lock_, 0);

  return this->cur_count_;
}

template <ACE_SYNCH_1> int
ACE_Message_Queue<ACE_SYNCH_2>::activate (void)
{
  ACE_TRACE ("ACE_Message_Queue<ACE_SYNCH_2>::activate");
  ACE_GUARD_RETURN (ACE_SYNCH_MUTEX_T, ace_mon, this->lock_, -1);

  return this->activate_i ();
}

template <ACE_SYNCH_1> int
ACE_Message_Queue<ACE_SYNCH_2>::deactivate (void)
{
  ACE_TRACE ("ACE_Message_Queue<ACE_SYNCH_2>::deactivate");
  ACE_GUARD_RETURN (ACE_SYNCH_MUTEX_T, ace_mon, this->lock_, -1);

  return this->deactivate_i ();
}

template <ACE_SYNCH_1>
ACE_Message_Queue_Iterator<ACE_SYNCH_2>::ACE_Message_Queue_Iterator (ACE_Message_Queue <ACE_SYNCH_2> &queue)
: queue_ (queue),
  curr_ (queue_.head_)
{
}

template <ACE_SYNCH_1> int 
ACE_Message_Queue_Iterator<ACE_SYNCH_2>::next (ACE_Message_Block *&entry)
{
  ACE_Read_Guard<ACE_SYNCH_MUTEX_T> m (this->queue_.lock_);

  if (this->curr_ != 0)
    {
      entry = this->curr_;
      return 1;
    }
  else
    return 0;
}

template <ACE_SYNCH_1> int
ACE_Message_Queue_Iterator<ACE_SYNCH_2>::advance (void)
{
  ACE_Read_Guard<ACE_SYNCH_MUTEX_T> m (this->queue_.lock_);

  this->curr_ = this->curr_->next ();
  return this->curr_ != 0;
}

template <ACE_SYNCH_1> void 
ACE_Message_Queue_Iterator<ACE_SYNCH_2>::dump (void) const
{
}

ACE_ALLOC_HOOK_DEFINE(ACE_Message_Queue_Iterator)


template <ACE_SYNCH_1>
ACE_Message_Queue_Reverse_Iterator<ACE_SYNCH_2>::ACE_Message_Queue_Reverse_Iterator (ACE_Message_Queue <ACE_SYNCH_2> &queue)
: queue_ (queue),
  curr_ (queue_.tail_)
{
}

template <ACE_SYNCH_1> int 
ACE_Message_Queue_Reverse_Iterator<ACE_SYNCH_2>::next (ACE_Message_Block *&entry)
{
  ACE_Read_Guard<ACE_SYNCH_MUTEX_T> m (this->queue_.lock_);

  if (this->curr_ != 0)
    {
      entry = this->curr_;
      return 1;
    }
  else
    return 0;
}

template <ACE_SYNCH_1> int
ACE_Message_Queue_Reverse_Iterator<ACE_SYNCH_2>::advance (void)
{
  ACE_Read_Guard<ACE_SYNCH_MUTEX_T> m (this->queue_.lock_);

  this->curr_ = this->curr_->prev ();
  return this->curr_ != 0;
}

template <ACE_SYNCH_1> void 
ACE_Message_Queue_Reverse_Iterator<ACE_SYNCH_2>::dump (void) const
{
}

ACE_ALLOC_HOOK_DEFINE(ACE_Message_Queue_Reverse_Iterator)


