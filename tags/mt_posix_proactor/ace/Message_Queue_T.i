/* -*- C++ -*- */
// $Id$

template <ACE_SYNCH_DECL> ACE_INLINE int
ACE_Message_Queue<ACE_SYNCH_USE>::dequeue (ACE_Message_Block *&first_item,
                                           ACE_Time_Value *timeout)
{
  ACE_TRACE ("ACE_Message_Queue<ACE_SYNCH_USE>::notification_strategy");
  return this->dequeue_head (first_item, timeout);
}

template <ACE_SYNCH_DECL> ACE_INLINE ACE_Notification_Strategy *
ACE_Message_Queue<ACE_SYNCH_USE>::notification_strategy (void)
{
  ACE_TRACE ("ACE_Message_Queue<ACE_SYNCH_USE>::notification_strategy");

  return this->notification_strategy_;
}

template <ACE_SYNCH_DECL> ACE_INLINE void
ACE_Message_Queue<ACE_SYNCH_USE>::notification_strategy (ACE_Notification_Strategy *s)
{
  ACE_TRACE ("ACE_Message_Queue<ACE_SYNCH_USE>::notification_strategy");

  this->notification_strategy_ = s;
}

// Check if queue is empty (does not hold locks).

template <ACE_SYNCH_DECL> ACE_INLINE int
ACE_Message_Queue<ACE_SYNCH_USE>::is_empty_i (void)
{
  ACE_TRACE ("ACE_Message_Queue<ACE_SYNCH_USE>::is_empty_i");
  return this->tail_ == 0;
}

// Check if queue is full (does not hold locks).

template <ACE_SYNCH_DECL> ACE_INLINE int
ACE_Message_Queue<ACE_SYNCH_USE>::is_full_i (void)
{
  ACE_TRACE ("ACE_Message_Queue<ACE_SYNCH_USE>::is_full_i");
  return this->cur_bytes_ > this->high_water_mark_;
}

// Check if queue is empty (holds locks).

template <ACE_SYNCH_DECL> ACE_INLINE int
ACE_Message_Queue<ACE_SYNCH_USE>::is_empty (void)
{
  ACE_TRACE ("ACE_Message_Queue<ACE_SYNCH_USE>::is_empty");
  ACE_GUARD_RETURN (ACE_SYNCH_MUTEX_T, ace_mon, this->lock_, -1);

  return this->is_empty_i ();
}

// Check if queue is full (holds locks).

template <ACE_SYNCH_DECL> ACE_INLINE int
ACE_Message_Queue<ACE_SYNCH_USE>::is_full (void)
{
  ACE_TRACE ("ACE_Message_Queue<ACE_SYNCH_USE>::is_full");
  ACE_GUARD_RETURN (ACE_SYNCH_MUTEX_T, ace_mon, this->lock_, -1);

  return this->is_full_i ();
}

template <ACE_SYNCH_DECL> ACE_INLINE size_t
ACE_Message_Queue<ACE_SYNCH_USE>::high_water_mark (void)
{
  ACE_TRACE ("ACE_Message_Queue<ACE_SYNCH_USE>::high_water_mark");
  ACE_GUARD_RETURN (ACE_SYNCH_MUTEX_T, ace_mon, this->lock_, 0);

  return this->high_water_mark_;
}

template <ACE_SYNCH_DECL> ACE_INLINE void
ACE_Message_Queue<ACE_SYNCH_USE>::high_water_mark (size_t hwm)
{
  ACE_TRACE ("ACE_Message_Queue<ACE_SYNCH_USE>::high_water_mark");
  ACE_GUARD (ACE_SYNCH_MUTEX_T, ace_mon, this->lock_);

  this->high_water_mark_ = hwm;
}

template <ACE_SYNCH_DECL> ACE_INLINE size_t
ACE_Message_Queue<ACE_SYNCH_USE>::low_water_mark (void)
{
  ACE_TRACE ("ACE_Message_Queue<ACE_SYNCH_USE>::low_water_mark");
  ACE_GUARD_RETURN (ACE_SYNCH_MUTEX_T, ace_mon, this->lock_, 0);

  return this->low_water_mark_;
}

template <ACE_SYNCH_DECL> ACE_INLINE void
ACE_Message_Queue<ACE_SYNCH_USE>::low_water_mark (size_t lwm)
{
  ACE_TRACE ("ACE_Message_Queue<ACE_SYNCH_USE>::low_water_mark");
  ACE_GUARD (ACE_SYNCH_MUTEX_T, ace_mon, this->lock_);

  this->low_water_mark_ = lwm;
}

// Return the current number of bytes in the queue.

template <ACE_SYNCH_DECL> ACE_INLINE size_t
ACE_Message_Queue<ACE_SYNCH_USE>::message_bytes (void)
{
  ACE_TRACE ("ACE_Message_Queue<ACE_SYNCH_USE>::message_bytes");
  ACE_GUARD_RETURN (ACE_SYNCH_MUTEX_T, ace_mon, this->lock_, 0);

  return this->cur_bytes_;
}

// Return the current number of messages in the queue.

template <ACE_SYNCH_DECL> ACE_INLINE size_t
ACE_Message_Queue<ACE_SYNCH_USE>::message_count (void)
{
  ACE_TRACE ("ACE_Message_Queue<ACE_SYNCH_USE>::message_count");
  ACE_GUARD_RETURN (ACE_SYNCH_MUTEX_T, ace_mon, this->lock_, 0);

  return this->cur_count_;
}

template <ACE_SYNCH_DECL> ACE_INLINE int
ACE_Message_Queue<ACE_SYNCH_USE>::activate (void)
{
  ACE_TRACE ("ACE_Message_Queue<ACE_SYNCH_USE>::activate");
  ACE_GUARD_RETURN (ACE_SYNCH_MUTEX_T, ace_mon, this->lock_, -1);

  return this->activate_i ();
}

template <ACE_SYNCH_DECL> ACE_INLINE int
ACE_Message_Queue<ACE_SYNCH_USE>::deactivate (void)
{
  ACE_TRACE ("ACE_Message_Queue<ACE_SYNCH_USE>::deactivate");
  ACE_GUARD_RETURN (ACE_SYNCH_MUTEX_T, ace_mon, this->lock_, -1);

  return this->deactivate_i ();
}

template <ACE_SYNCH_DECL> ACE_INLINE int
ACE_Message_Queue<ACE_SYNCH_USE>::deactivated (void)
{
  ACE_TRACE ("ACE_Message_Queue<ACE_SYNCH_USE>::deactivated");

  return this->deactivated_;
}

ACE_ALLOC_HOOK_DEFINE(ACE_Message_Queue_Reverse_Iterator)
