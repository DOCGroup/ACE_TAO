/* -*- C++ -*- */
// $Id$

// Thread_Manager.i

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

//////////////////////////////////////////////////
#if defined (NANBOR_EXP_CODES)
ACE_INLINE
ACE_Thread_Descriptor_Adapter::ACE_Thread_Descriptor_Adapter (void)
  : thr_desc_ (0)
{
}

ACE_INLINE
ACE_Thread_Descriptor_Adapter::ACE_Thread_Descriptor_Adapter
   (ACE_Thread_Descriptor *thr_desc)
     : thr_desc_ (thr_desc)
{
}

ACE_INLINE
ACE_Thread_Descriptor_Adapter::ACE_Thread_Descriptor_Adapter
  (const ACE_Thread_Descriptor_Adapter &td)
    : thr_desc_ (td.thr_desc_)
{
}

ACE_INLINE ACE_Thread_Descriptor_Adatper&
ACE_Thread_Descriptor_Adapter::operator= (const ACE_Thread_Descriptor_Adapter &td)
{
  this->thr_desc_ = td.thr_desc_;
}

ACE_INLINE int
ACE_Thread_Descriptor_Adapter::operator== (const ACE_Thread_Descriptor_Adapter &td) const
{
  return (this->self () == td.self ());
}

// = Accessing the underlying methods of ACE_Thread_Descriptor.

ACE_INLINE ACE_thread_t
ACE_Thread_Descriptor_Adapter::self (void)
{
  return this->thr_desc_->self ();
}

ACE_INLINE void
ACE_Thread_Descriptor_Adapter::self (ACE_hthread_t &handle)
{
  this->thr_desc_->self (handle);
}

ACE_INLINE int
ACE_Thread_Descriptor_Adapter::grp_id (void)
{
  return this->thr_desc_->grp_id ();
}

ACE_INLINE ACE_Thread_State
ACE_Thread_Descriptor_Adapter::state (void)
{
  return this->thr_desc_->state ();
}

ACE_INLINE ACE_Task_Base *
ACE_Thread_Descriptor_Adapter::task (void)
{
  return this->thr_desc_->task ();
}

ACE_INLINE void
ACE_Thread_Descriptor_Adapter::dump (void) const
{
  this->thr_desc_->dump ();
}

ACE_INLINE int
ACE_Thread_Descriptor_Adapter::at_exit (void *object,
                                        ACE_CLEANUP_FUNC cleanup_hook,
                                        void *param)
{
  return this->thr_desc_->at_exit (object, cleanup_hook, param);
}

ACE_INLINE ACE_Thread_Descriptor *
ACE_Thread_Descriptor_Adapter::get_thread_descriptor (void)
{
  return this->thr_desc_;
}
#endif /* NANBOR_EXP_CODES */
//////////////////////////////////////////////////

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
  ACE_Thread_Descriptor td;

  if (this->thread_descriptor (ACE_Thread::self (), td) == -1)
    return 0;
  else
    return td.task ();
}
