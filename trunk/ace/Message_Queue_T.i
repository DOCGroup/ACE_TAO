/* -*- C++ -*- */
// $Id$

template <ACE_SYNCH_DECL> ACE_INLINE int
ACE_Message_Queue<ACE_SYNCH_USE>::dequeue (ACE_Message_Block *&first_item,
                                           ACE_Time_Value *timeout)
{
  ACE_TRACE ("ACE_Message_Queue<ACE_SYNCH_USE>::dequeue");
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

template <ACE_SYNCH_DECL> ACE_INLINE size_t
ACE_Message_Queue<ACE_SYNCH_USE>::message_bytes (void)
{
  ACE_TRACE ("ACE_Message_Queue<ACE_SYNCH_USE>::message_bytes");
  ACE_GUARD_RETURN (ACE_SYNCH_MUTEX_T, ace_mon, this->lock_, 0);

  return this->cur_bytes_;
}

template <ACE_SYNCH_DECL> ACE_INLINE size_t
ACE_Message_Queue<ACE_SYNCH_USE>::message_length (void)
{
  ACE_TRACE ("ACE_Message_Queue<ACE_SYNCH_USE>::message_length");
  ACE_GUARD_RETURN (ACE_SYNCH_MUTEX_T, ace_mon, this->lock_, 0);

  return this->cur_length_;
}

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

#if 0
// The Sun Forte 6 (CC 5.1) compiler is only happy if this is in the
// header file      (j.russell.noseworthy@objectsciences.com)

template <ACE_SYNCH_DECL> ACE_INLINE ACE_SYNCH_MUTEX_T &
ACE_Message_Queue<ACE_SYNCH_USE>::lock (void)
{
  return this->lock_;
}
#endif /* 0 */

ACE_ALLOC_HOOK_DEFINE(ACE_Message_Queue_Reverse_Iterator)

template <class ACE_MESSAGE_TYPE, ACE_SYNCH_DECL> ACE_INLINE int
ACE_Message_Queue_Ex<ACE_MESSAGE_TYPE, ACE_SYNCH_USE>::dequeue (ACE_MESSAGE_TYPE *&first_item,
                                                                ACE_Time_Value *timeout)
{
  ACE_TRACE ("ACE_Message_Queue_Ex<ACE_MESSAGE_TYPE, ACE_SYNCH_USE>::dequeue");

  return this->dequeue_head (first_item, timeout);
}

template <class ACE_MESSAGE_TYPE, ACE_SYNCH_DECL> ACE_INLINE ACE_Notification_Strategy *
ACE_Message_Queue_Ex<ACE_MESSAGE_TYPE, ACE_SYNCH_USE>::notification_strategy (void)
{
  ACE_TRACE ("ACE_Message_Queue_Ex<ACE_MESSAGE_TYPE, ACE_SYNCH_USE>::notification_strategy");

  return this->queue_.notification_strategy ();
}

template <class ACE_MESSAGE_TYPE, ACE_SYNCH_DECL> ACE_INLINE void
ACE_Message_Queue_Ex<ACE_MESSAGE_TYPE, ACE_SYNCH_USE>::notification_strategy (ACE_Notification_Strategy *s)
{
  ACE_TRACE ("ACE_Message_Queue_Ex<ACE_MESSAGE_TYPE, ACE_SYNCH_USE>::notification_strategy");

  this->queue_.notification_strategy (s);
}

// Check if queue is empty (holds locks).

template <class ACE_MESSAGE_TYPE, ACE_SYNCH_DECL> ACE_INLINE int
ACE_Message_Queue_Ex<ACE_MESSAGE_TYPE, ACE_SYNCH_USE>::is_empty (void)
{
  ACE_TRACE ("ACE_Message_Queue_Ex<ACE_MESSAGE_TYPE, ACE_SYNCH_USE>::is_empty");

  return this->queue_.is_empty ();
}

// Check if queue is full (holds locks).

template <class ACE_MESSAGE_TYPE, ACE_SYNCH_DECL> ACE_INLINE int
ACE_Message_Queue_Ex<ACE_MESSAGE_TYPE, ACE_SYNCH_USE>::is_full (void)
{
  ACE_TRACE ("ACE_Message_Queue_Ex<ACE_MESSAGE_TYPE, ACE_SYNCH_USE>::is_full");

  return this->queue_.is_full ();
}

template <class ACE_MESSAGE_TYPE, ACE_SYNCH_DECL> ACE_INLINE size_t
ACE_Message_Queue_Ex<ACE_MESSAGE_TYPE, ACE_SYNCH_USE>::high_water_mark (void)
{
  ACE_TRACE ("ACE_Message_Queue_Ex<ACE_MESSAGE_TYPE, ACE_SYNCH_USE>::high_water_mark");

  return this->queue_.high_water_mark ();
}

template <class ACE_MESSAGE_TYPE, ACE_SYNCH_DECL> ACE_INLINE void
ACE_Message_Queue_Ex<ACE_MESSAGE_TYPE, ACE_SYNCH_USE>::high_water_mark (size_t hwm)
{
  ACE_TRACE ("ACE_Message_Queue_Ex<ACE_MESSAGE_TYPE, ACE_SYNCH_USE>::high_water_mark");

  this->queue_.high_water_mark (hwm);
}

template <class ACE_MESSAGE_TYPE, ACE_SYNCH_DECL> ACE_INLINE size_t
ACE_Message_Queue_Ex<ACE_MESSAGE_TYPE, ACE_SYNCH_USE>::low_water_mark (void)
{
  ACE_TRACE ("ACE_Message_Queue_Ex<ACE_MESSAGE_TYPE, ACE_SYNCH_USE>::low_water_mark");

  return this->queue_.low_water_mark ();
}

template <class ACE_MESSAGE_TYPE, ACE_SYNCH_DECL> ACE_INLINE void
ACE_Message_Queue_Ex<ACE_MESSAGE_TYPE, ACE_SYNCH_USE>::low_water_mark (size_t lwm)
{
  ACE_TRACE ("ACE_Message_Queue_Ex<ACE_MESSAGE_TYPE, ACE_SYNCH_USE>::low_water_mark");

  this->queue_.low_water_mark (lwm);
}

template <class ACE_MESSAGE_TYPE, ACE_SYNCH_DECL> ACE_INLINE size_t
ACE_Message_Queue_Ex<ACE_MESSAGE_TYPE, ACE_SYNCH_USE>::message_bytes (void)
{
  ACE_TRACE ("ACE_Message_Queue_Ex<ACE_MESSAGE_TYPE, ACE_SYNCH_USE>::message_bytes");

  return this->queue_.message_bytes ();
}

template <class ACE_MESSAGE_TYPE, ACE_SYNCH_DECL> ACE_INLINE size_t
ACE_Message_Queue_Ex<ACE_MESSAGE_TYPE, ACE_SYNCH_USE>::message_length (void)
{
  ACE_TRACE ("ACE_Message_Queue_Ex<ACE_MESSAGE_TYPE, ACE_SYNCH_USE>::message_length");

  return this->queue_.message_length ();
}

template <class ACE_MESSAGE_TYPE, ACE_SYNCH_DECL> ACE_INLINE size_t
ACE_Message_Queue_Ex<ACE_MESSAGE_TYPE, ACE_SYNCH_USE>::message_count (void)
{
  ACE_TRACE ("ACE_Message_Queue_Ex<ACE_MESSAGE_TYPE, ACE_SYNCH_USE>::message_count");

  return this->queue_.message_count ();
}

template <class ACE_MESSAGE_TYPE, ACE_SYNCH_DECL> ACE_INLINE int
ACE_Message_Queue_Ex<ACE_MESSAGE_TYPE, ACE_SYNCH_USE>::activate (void)
{
  ACE_TRACE ("ACE_Message_Queue_Ex<ACE_MESSAGE_TYPE, ACE_SYNCH_USE>::activate");

  return this->queue_.activate ();
}

template <class ACE_MESSAGE_TYPE, ACE_SYNCH_DECL> ACE_INLINE int
ACE_Message_Queue_Ex<ACE_MESSAGE_TYPE, ACE_SYNCH_USE>::deactivate (void)
{
  ACE_TRACE ("ACE_Message_Queue_Ex<ACE_MESSAGE_TYPE, ACE_SYNCH_USE>::deactivate");

  return this->queue_.deactivate ();
}

template <class ACE_MESSAGE_TYPE, ACE_SYNCH_DECL> ACE_INLINE int
ACE_Message_Queue_Ex<ACE_MESSAGE_TYPE, ACE_SYNCH_USE>::deactivated (void)
{
  ACE_TRACE ("ACE_Message_Queue_Ex<ACE_MESSAGE_TYPE, ACE_SYNCH_USE>::deactivated");

  return this->queue_.deactivated ();
}

#if 0
// The Sun Forte 6 (CC 5.1) compiler is only happy if this is in the
// header file      (j.russell.noseworthy@objectsciences.com)
template <class ACE_MESSAGE_TYPE, ACE_SYNCH_DECL> ACE_INLINE ACE_SYNCH_MUTEX_T &
ACE_Message_Queue_Ex<ACE_MESSAGE_TYPE, ACE_SYNCH_USE>::lock (void)
{
  return this->queue_.lock ();
}
#endif /* 0 */
