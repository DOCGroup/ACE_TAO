/* -*- C++ -*- */
// $Id$

// Task_T.i

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
  if (this->delete_msg_queue_)
    delete this->msg_queue_;

  this->delete_msg_queue_ = 0;
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
