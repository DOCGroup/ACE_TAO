/* -*- C++ -*- */
// $Id$

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
ACE_Message_Queue_Vx::high_water_mark (size_t hwm)
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
ACE_Message_Queue_Vx::low_water_mark (size_t lwm)
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
