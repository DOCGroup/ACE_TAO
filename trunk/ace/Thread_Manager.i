/* -*- C++ -*- */
// $Id$

// Thread_Manager.i

ACE_INLINE
ACE_Thread_Descriptor_Base::ACE_Thread_Descriptor_Base (void)
  : thr_handle_ (ACE_OS::NULL_hthread)
{
}

ACE_INLINE
ACE_Thread_Descriptor_Base::~ACE_Thread_Descriptor_Base (void)
{
}

// Unique thread id.
ACE_INLINE ACE_thread_t
ACE_Thread_Descriptor::self (void)
{
  ACE_TRACE ("ACE_Thread_Descriptor::self");
  return this->thr_id_;
}

ACE_INLINE ACE_Task_Base *
ACE_Thread_Descriptor::task (void)
{
  ACE_TRACE ("ACE_Thread_Descriptor::task");
  return this->task_;
}

// Unique kernel-level thread handle.

ACE_INLINE void
ACE_Thread_Descriptor::self (ACE_hthread_t &handle)
{
  ACE_TRACE ("ACE_Thread_Descriptor::self");
  handle = this->thr_handle_;
}

// Group ID.

ACE_INLINE int
ACE_Thread_Descriptor::grp_id (void)
{
  ACE_TRACE ("ACE_Thread_Descriptor::grp_id");
  return grp_id_;
}

// Current state of the thread.
ACE_INLINE ACE_Thread_State
ACE_Thread_Descriptor::state (void)
{
  ACE_TRACE ("ACE_Thread_Descriptor::state");
  return thr_state_;
}

// Get the thread creation
ACE_INLINE long
ACE_Thread_Descriptor::flags (void) const
{
  ACE_TRACE ("ACE_Thread_Descriptor::flag");
  return flags_;
}

// Set the <next_> pointer
ACE_INLINE void
ACE_Thread_Descriptor::set_next (ACE_Thread_Descriptor *td)
{
  ACE_TRACE ("ACE_Thread_Descriptor::set_next");
  this->next_ = td;
}

// Get the <next_> pointer
ACE_INLINE ACE_Thread_Descriptor *
ACE_Thread_Descriptor::get_next (void)
{
  ACE_TRACE ("ACE_Thread_Descriptor::flag");
  return this->next_;
}

// Set the exit status.

ACE_INLINE void *
ACE_Thread_Control::status (void *s)
{
  ACE_TRACE ("ACE_Thread_Control::status");
  return this->status_ = s;
}

// Get the exit status.

ACE_INLINE void *
ACE_Thread_Control::status (void)
{
  ACE_TRACE ("ACE_Thread_Control::status");
  return this->status_;
}

// Returns the current <Thread_Manager>.

ACE_INLINE ACE_Thread_Manager *
ACE_Thread_Control::thr_mgr (void)
{
  ACE_TRACE ("ACE_Thread_Control::thr_mgr");
  return this->tm_;
}

// Atomically set a new <Thread_Manager> and return the old
// <Thread_Manager>.

ACE_INLINE ACE_Thread_Manager *
ACE_Thread_Control::thr_mgr (ACE_Thread_Manager *tm)
{
  ACE_TRACE ("ACE_Thread_Control::thr_mgr");
  ACE_Thread_Manager *o_tm = this->tm_;
  this->tm_ = tm;
  return o_tm;
}

// Return the unique ID of the thread.

ACE_INLINE ACE_thread_t
ACE_Thread_Manager::thr_self (void)
{
  ACE_TRACE ("ACE_Thread_Manager::thr_self");
  return ACE_Thread::self ();
}

ACE_INLINE ACE_Task_Base *
ACE_Thread_Manager::task (void)
{
  ACE_TRACE ("ACE_Thread_Manager::task");

  ACE_Thread_Descriptor *td = this->thread_desc_self () ;

  if (td == 0)
    return 0;
  else
    return td->task ();
}

ACE_INLINE int
ACE_Thread_Manager::open (size_t)
{
  // Currently no-op.
  return 0;
}

ACE_INLINE void
ACE_Thread_Manager::wait_on_exit (int do_wait)
{
  this->automatic_wait_ = do_wait;
}

ACE_INLINE int
ACE_Thread_Manager::wait_on_exit (void)
{
  return this->automatic_wait_;
}
