/* -*- C++ -*- */
// $Id$

// Message_Queue.i

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
  return this->cur_bytes_ <= 0 && this->cur_count_ <= 0;
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

ACE_INLINE int 
ACE_Dynamic_Message_Strategy::is_pending (const ACE_Message_Block & mb, 
                                          const ACE_Time_Value & tv)
{
  return ((mb.msg_priority () < pending_threshold_) || 
          this->is_beyond_late (mb, tv))
         ? 0 : 1;
}
  // returns true if the message has a pending (not late) priority value

ACE_INLINE u_long 
ACE_Dynamic_Message_Strategy::static_bit_field_mask (void)
{
  return static_bit_field_mask_;
}
  // get static bit field mask

ACE_INLINE void 
ACE_Dynamic_Message_Strategy::static_bit_field_mask (u_long ul)
{
  static_bit_field_mask_ = ul;
}
  // set static bit field mask

ACE_INLINE u_long 
ACE_Dynamic_Message_Strategy::static_bit_field_shift (void)
{
  return static_bit_field_shift_;
}
  // get left shift value to make room for static bit field

ACE_INLINE void 
ACE_Dynamic_Message_Strategy::static_bit_field_shift (u_long ul)
{
  static_bit_field_shift_ = ul;
}
  // set left shift value to make room for static bit field

ACE_INLINE u_long 
ACE_Dynamic_Message_Strategy::pending_threshold (void)
{
  return pending_threshold_;
}
  // get pending threshold priority value

ACE_INLINE void 
ACE_Dynamic_Message_Strategy::pending_threshold (u_long ul)
{
  pending_threshold_ = ul;
}
  // set pending threshold priority value

ACE_INLINE u_long 
ACE_Dynamic_Message_Strategy::dynamic_priority_max (void)
{
  return dynamic_priority_max_;
}
  // get maximum supported priority value

ACE_INLINE void 
ACE_Dynamic_Message_Strategy::dynamic_priority_max (u_long ul)
{
  dynamic_priority_max_ = ul;
}
  // set maximum supported priority value

ACE_INLINE u_long 
ACE_Dynamic_Message_Strategy::dynamic_priority_offset (void)
{
  return dynamic_priority_offset_;
}
  // get axis shift to map signed range into unsigned range

ACE_INLINE void 
ACE_Dynamic_Message_Strategy::dynamic_priority_offset (u_long ul)
{
  dynamic_priority_offset_ = ul;
}
  // set axis shift to map signed range into unsigned range
