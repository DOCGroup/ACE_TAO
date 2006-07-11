// -*- C++ -*-
//
// $Id$

ACE_BEGIN_VERSIONED_NAMESPACE_DECL

ACE_INLINE
ACE_Message_Queue_Base::ACE_Message_Queue_Base (void)
{
}

#if defined (ACE_VXWORKS)
// Specialization to use native VxWorks Message Queues.

ACE_INLINE MSG_Q_ID
ACE_Message_Queue_Vx::msgq (void)
{
  // Hijack the tail_ field to store the MSG_Q_ID.
  return reinterpret_cast<MSG_Q_ID> (tail_);
}

#endif /* ACE_VXWORKS */

#if defined (ACE_WIN32) && (ACE_HAS_WINNT4 != 0)

#include "ace/Guard_T.h"

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

  return this->cur_bytes_ > 0 || this->cur_count_ > 0 ? 0 : 1;
}

ACE_INLINE size_t
ACE_Message_Queue_NT::message_bytes (void)
{
  ACE_TRACE ("ACE_Message_Queue_NT::message_bytes");
  // Accessing to size_t must be atomic.
  return this->cur_bytes_;
}

ACE_INLINE size_t
ACE_Message_Queue_NT::message_length (void)
{
  ACE_TRACE ("ACE_Message_Queue_NT::message_length");
  // Accessing to size_t must be atomic.
  return this->cur_length_;
}

ACE_INLINE size_t
ACE_Message_Queue_NT::message_count (void)
{
  ACE_TRACE ("ACE_Message_Queue_NT::message_count");
  // Accessing to size_t must be atomic.
  return this->cur_count_;
}

ACE_INLINE void
ACE_Message_Queue_NT::message_bytes (size_t new_value)
{
  ACE_TRACE ("ACE_Message_Queue_NT::message_bytes");
  ACE_GUARD (ACE_Thread_Mutex, ace_mon, this->lock_);

  this->cur_bytes_ = new_value;
}

ACE_INLINE void
ACE_Message_Queue_NT::message_length (size_t new_value)
{
  ACE_TRACE ("ACE_Message_Queue_NT::message_length");
  ACE_GUARD (ACE_Thread_Mutex, ace_mon, this->lock_);

  this->cur_length_ = new_value;
}

ACE_INLINE DWORD
ACE_Message_Queue_NT::max_threads (void)
{
  ACE_TRACE ("ACE_Message_Queue_NT::max_threads");
  return this->max_cthrs_;
}

ACE_INLINE int
ACE_Message_Queue_NT::deactivated (void)
{
  ACE_TRACE ("ACE_Message_Queue_NT::deactivated");
  // Accessing to int must be atomic.
  return this->state_ == ACE_Message_Queue_Base::DEACTIVATED;
}

ACE_INLINE ACE_HANDLE
ACE_Message_Queue_NT::completion_port (void)
{
  ACE_TRACE ("ACE_Message_Queue_NT::completion_port");
  return this->completion_port_;
}

ACE_INLINE int
ACE_Message_Queue_NT::peek_dequeue_head (ACE_Message_Block *&first_item,
                                         ACE_Time_Value *timeout)
{
  ACE_UNUSED_ARG(first_item);
  ACE_UNUSED_ARG(timeout);
  ACE_NOTSUP_RETURN (-1);
}

ACE_INLINE ACE_Notification_Strategy *
ACE_Message_Queue_NT::notification_strategy (void)
{
  ACE_NOTSUP_RETURN (0);
}

ACE_INLINE void
ACE_Message_Queue_NT::notification_strategy (ACE_Notification_Strategy *)
{
}

#endif /* ACE_WIN32 && ACE_HAS_WINNT4 != 0 */

ACE_END_VERSIONED_NAMESPACE_DECL
