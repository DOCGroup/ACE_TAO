/* -*- C++ -*- */
// $Id$

ACE_INLINE
ACE_Message_Queue_Base::ACE_Message_Queue_Base (void)
{
}

ACE_INLINE
ACE_Message_Queue_Base::~ACE_Message_Queue_Base (void)
{
}

#if defined (VXWORKS)
// Specialization to use native VxWorks Message Queues.

ACE_INLINE MSG_Q_ID
ACE_Message_Queue_Vx::msgq ()
{
  // Hijack the tail_ field to store the MSG_Q_ID.
  return ACE_reinterpret_cast (MSG_Q_ID, tail_);
}

ACE_INLINE int
ACE_Message_Queue_Vx::is_empty_i (void)
{
  ACE_TRACE ("ACE_Message_Queue_Vx::is_empty_i");
  return ::msgQNumMsgs (msgq ()) == 0;
}

ACE_INLINE int
ACE_Message_Queue_Vx::is_full_i (void)
{
  ACE_TRACE ("ACE_Message_Queue_Vx::is_full_i");
  return ::msgQNumMsgs (msgq ()) >= max_messages_;
}

ACE_INLINE size_t
ACE_Message_Queue_Vx::high_water_mark (void)
{
  ACE_TRACE ("ACE_Message_Queue_Vx::high_water_mark");
  // Don't need to guard, because this is fixed.

  ACE_NOTSUP_RETURN ((size_t) -1);
}

ACE_INLINE void
ACE_Message_Queue_Vx::high_water_mark (size_t)
{
  ACE_TRACE ("ACE_Message_Queue_Vx::high_water_mark");
  // Don't need to guard, because this is fixed.
  errno = ENOTSUP;
}

ACE_INLINE size_t
ACE_Message_Queue_Vx::low_water_mark (void)
{
  ACE_TRACE ("ACE_Message_Queue_Vx::low_water_mark");
  // Don't need to guard, because this is fixed.

  ACE_NOTSUP_RETURN ((size_t) -1);
}

ACE_INLINE void
ACE_Message_Queue_Vx::low_water_mark (size_t)
{
  ACE_TRACE ("ACE_Message_Queue_Vx::low_water_mark");
  // Don't need to guard, because this is fixed.

  errno = ENOTSUP;
}

// Return the current number of bytes in the queue.

ACE_INLINE size_t
ACE_Message_Queue_Vx::message_bytes (void)
{
  ACE_TRACE ("ACE_Message_Queue_Vx::message_bytes");
  ACE_NOTSUP_RETURN ((size_t) -1);
}

// Return the current number of messages in the queue.

ACE_INLINE size_t
ACE_Message_Queue_Vx::message_count (void)
{
  ACE_TRACE ("ACE_Message_Queue_Vx::message_count");
  // Don't need to guard, because this is a system call.

  return ::msgQNumMsgs (msgq ());
}

#endif /* VXWORKS */

#if defined (ACE_WIN32) && (ACE_HAS_WINNT4 != 0)
ACE_INLINE int
ACE_Message_Queue_NT::enqueue_tail (ACE_Message_Block *new_item,
                                    ACE_Time_Value *timeout)
{
  ACE_TRACE ("ACE_Message_Queue_NT::enqueue_tail");
  return this->enqueue (new_item, timeout);
}

ACE_INLINE int
ACE_Message_Queue_NT::dequeue_head (ACE_Message_Block *&first_item,
                                    ACE_Time_Value *timeout)
{
  ACE_TRACE ("ACE_Message_Queue_NT::dequeue_head");
  return this->dequeue (first_item, timeout);
}

ACE_INLINE int
ACE_Message_Queue_NT::is_full (void)
{
  ACE_TRACE ("ACE_Message_Queue_NT::is_full");
  return 0; // Always not full.
}

ACE_INLINE int
ACE_Message_Queue_NT::is_empty (void)
{
  ACE_TRACE ("ACE_Message_Queue_NT::is_empty");
  ACE_GUARD_RETURN (ACE_Thread_Mutex, ace_mon, this->lock_, 0);

  return (this->cur_bytes_ > 0 && this->cur_count_ > 0 ? 1 : 0);
}

ACE_INLINE size_t
ACE_Message_Queue_NT::message_bytes (void)
{
  ACE_TRACE ("ACE_Message_Queue_NT::message_bytes");
  // Accessing to size_t must be atomic.
  return this->cur_bytes_;
}

ACE_INLINE size_t
ACE_Message_Queue_NT::message_count (void)
{
  ACE_TRACE ("ACE_Message_Queue_NT::message_count");
  // Accessing to size_t must be atomic.
  return this->cur_count_;
}

ACE_INLINE size_t
ACE_Message_Queue_NT::max_threads (void)
{
  ACE_TRACE ("ACE_Message_Queue_NT::max_threads");
  return this->max_cthrs_;
}

ACE_INLINE int
ACE_Message_Queue_NT::deactivated (void)
{
  ACE_TRACE ("ACE_Message_Queue_NT::ceactivated");
  // Accessing to int must be atomic.
  return this->deactivated_;
}

ACE_INLINE ACE_HANDLE
ACE_Message_Queue_NT::completion_port (void)
{
  ACE_TRACE ("ACE_Message_Queue_NT::completion_port");
  return this->completion_port_;
}

#endif /* ACE_WIN32 && ACE_HAS_WINNT4 != 0 */
