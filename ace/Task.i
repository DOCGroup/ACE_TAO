/* -*- C++ -*- */
// $Id$

// Task.i

#include "ace/Log_Msg.h"

ACE_INLINE ACE_Thread_Manager *
ACE_Task_Base::thr_mgr (void)
{
  ACE_TRACE ("ACE_Task_Base::thr_mgr");
  return this->thr_mgr_;
}

ACE_INLINE void
ACE_Task_Base::thr_mgr (ACE_Thread_Manager *thr_mgr)
{
  ACE_TRACE ("ACE_Task_Base::thr_mgr");
  this->thr_mgr_ = thr_mgr;
}

// Return the count of the current number of threads.
ACE_INLINE size_t
ACE_Task_Base::thr_count (void)
{
  ACE_TRACE ("ACE_Task_Base::thr_count");
  ACE_MT (ACE_GUARD_RETURN (ACE_Thread_Mutex, ace_mon, this->lock_, 0));

  return this->thr_count_;
}

// Decrement the count of the active threads by 1.

ACE_INLINE void
ACE_Task_Base::thr_count_dec (void)
{
  ACE_TRACE ("ACE_Task_Base::thr_count_dec");
  ACE_MT (ACE_GUARD (ACE_Thread_Mutex, ace_mon, this->lock_));

  this->thr_count_--;
}

ACE_INLINE int
ACE_Task_Base::is_reader (void)
{
  ACE_TRACE ("ACE_Task_Base::is_reader");
  return (ACE_BIT_ENABLED (this->flags_, ACE_Task_Flags::ACE_READER));
}

ACE_INLINE int
ACE_Task_Base::is_writer (void)
{
  ACE_TRACE ("ACE_Task_Base::is_writer");
  return (ACE_BIT_DISABLED (this->flags_, ACE_Task_Flags::ACE_READER));
}

// Default ACE_Task service routine

int
ACE_Task_Base::svc (void)
{
  ACE_TRACE ("ACE_Task_Base::svc");
  return 0;
}

template <ACE_SYNCH_1> ACE_INLINE void
ACE_Task<ACE_SYNCH_2>::water_marks (ACE_IO_Cntl_Msg::ACE_IO_Cntl_Cmds cmd, 
				    size_t size)
{
  ACE_TRACE ("ACE_Task<ACE_SYNCH_2>::water_marks");
  if (cmd == ACE_IO_Cntl_Msg::SET_LWM)
    this->msg_queue_->low_water_mark (size);
  else /* cmd == ACE_IO_Cntl_Msg::SET_HWM */
    this->msg_queue_->high_water_mark (size);
}

template <ACE_SYNCH_1> ACE_INLINE int 
ACE_Task<ACE_SYNCH_2>::getq (ACE_Message_Block *&mb, ACE_Time_Value *tv)
{
  ACE_TRACE ("ACE_Task<ACE_SYNCH_2>::getq");
  return this->msg_queue_->dequeue_head (mb, tv);
}

template <ACE_SYNCH_1> ACE_INLINE int 
ACE_Task<ACE_SYNCH_2>::can_put (ACE_Message_Block *)
{
  ACE_TRACE ("ACE_Task<ACE_SYNCH_2>::can_put");
  assert (!"not implemented");
  return -1;
}

template <ACE_SYNCH_1> ACE_INLINE int
ACE_Task<ACE_SYNCH_2>::putq (ACE_Message_Block *mb, ACE_Time_Value *tv)
{
  ACE_TRACE ("ACE_Task<ACE_SYNCH_2>::putq");
  return this->msg_queue_->enqueue_tail (mb, tv);
}

template <ACE_SYNCH_1> ACE_INLINE int
ACE_Task<ACE_SYNCH_2>::ungetq (ACE_Message_Block *mb, ACE_Time_Value *tv)
{
  ACE_TRACE ("ACE_Task<ACE_SYNCH_2>::ungetq");
  return this->msg_queue_->enqueue_head (mb, tv);
}

template <ACE_SYNCH_1> ACE_INLINE int
ACE_Task<ACE_SYNCH_2>::flush (u_long flag)
{
  ACE_TRACE ("ACE_Task<ACE_SYNCH_2>::flush");
  if (ACE_BIT_ENABLED (flag, ACE_Task_Flags::ACE_FLUSHALL))
    return this->msg_queue_ != 0 && this->msg_queue_->close ();
  else
    return -1;   // Note, need to be more careful about what we free...
}

template <ACE_SYNCH_1> ACE_INLINE void
ACE_Task<ACE_SYNCH_2>::msg_queue (ACE_Message_Queue<ACE_SYNCH_2> *mq)
{
  ACE_TRACE ("ACE_Task<ACE_SYNCH_2>::msg_queue");
  this->msg_queue_ = mq;
}

template <ACE_SYNCH_1> ACE_Message_Queue<ACE_SYNCH_2> *
ACE_Task<ACE_SYNCH_2>::msg_queue (void)
{
  ACE_TRACE ("ACE_Task<ACE_SYNCH_2>::msg_queue");
  return this->msg_queue_;
}

template <ACE_SYNCH_1> ACE_INLINE int
ACE_Task<ACE_SYNCH_2>::reply (ACE_Message_Block *mb, ACE_Time_Value *tv)
{
  ACE_TRACE ("ACE_Task<ACE_SYNCH_2>::reply");
  return this->sibling ()->put_next (mb, tv);
}

template <ACE_SYNCH_1> ACE_INLINE ACE_Task<ACE_SYNCH_2> *
ACE_Task<ACE_SYNCH_2>::next (void)
{
  ACE_TRACE ("ACE_Task<ACE_SYNCH_2>::next");
  return this->next_;
}

template <ACE_SYNCH_1> ACE_INLINE void
ACE_Task<ACE_SYNCH_2>::next (ACE_Task<ACE_SYNCH_2> *q)
{
  ACE_TRACE ("ACE_Task<ACE_SYNCH_2>::next");
  this->next_ = q;
}

// Transfer msg to the next ACE_Task.

template <ACE_SYNCH_1> ACE_INLINE int
ACE_Task<ACE_SYNCH_2>::put_next (ACE_Message_Block *msg, ACE_Time_Value *tv)
{
  ACE_TRACE ("ACE_Task<ACE_SYNCH_2>::put_next");
  return this->next_ == 0 ? -1 : this->next_->put (msg, tv);
}

