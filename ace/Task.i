/* -*- C++ -*- */
// $Id$

// Task.i

ACE_INLINE ACE_Thread_Manager *
ACE_Task_Base::thr_mgr (void) const
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
ACE_Task_Base::thr_count (void) const
{
  ACE_TRACE ("ACE_Task_Base::thr_count");
  ACE_MT (ACE_GUARD_RETURN (ACE_Thread_Mutex, ace_mon, (ACE_Thread_Mutex &) this->lock_, 0));

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
ACE_Task_Base::is_reader (void) const
{
  ACE_TRACE ("ACE_Task_Base::is_reader");
  return (ACE_BIT_ENABLED (this->flags_, ACE_Task_Flags::ACE_READER));
}

ACE_INLINE int
ACE_Task_Base::is_writer (void) const
{
  ACE_TRACE ("ACE_Task_Base::is_writer");
  return (ACE_BIT_DISABLED (this->flags_, ACE_Task_Flags::ACE_READER));
}

// Default ACE_Task service routine

ACE_INLINE int
ACE_Task_Base::svc (void)
{
  ACE_TRACE ("ACE_Task_Base::svc");
  return 0;
}

// Default ACE_Task open routine

ACE_INLINE int
ACE_Task_Base::open (void *)
{
  ACE_TRACE ("ACE_Task_Base::open");
  return 0;
}

// Default ACE_Task close routine

ACE_INLINE int
ACE_Task_Base::close (u_long)
{
  ACE_TRACE ("ACE_Task_Base::close");
  return 0;
}

// Default ACE_Task put routine.

ACE_INLINE int
ACE_Task_Base::put (ACE_Message_Block *, ACE_Time_Value *)
{
  ACE_TRACE ("ACE_Task_Base::put");
  return 0;
}

// Get the current group id.
ACE_INLINE int
ACE_Task_Base::grp_id (void) const
{
  ACE_TRACE ("ACE_Task_Base::grp_id");
  ACE_MT (ACE_GUARD_RETURN (ACE_Thread_Mutex, ace_mon, (ACE_Thread_Mutex &) this->lock_, -1));
  return this->grp_id_;
}

// Set the current group id.
ACE_INLINE void
ACE_Task_Base::grp_id (int id)
{
  ACE_TRACE ("ACE_Task_Base::grp_id");
  ACE_MT (ACE_GUARD (ACE_Thread_Mutex, ace_mon, this->lock_));
  this->grp_id_ = id;
}

