/* -*- C++ -*- */
// $Id$

// Thread_Manager.i

#if !defined(ACE_USE_ONE_SHOT_AT_THREAD_EXIT)
ACE_INLINE
ACE_At_Thread_Exit::ACE_At_Thread_Exit (void)
  : next_ (0),
    td_ (0),
    was_applied_ (0),
    is_owner_ (1)
{
}

ACE_INLINE int
ACE_At_Thread_Exit::was_applied() const

{
   return was_applied_;
}

ACE_INLINE int
ACE_At_Thread_Exit::was_applied (int applied)
{
  was_applied_ = applied;
  if (was_applied_)
    td_ = 0;
  return was_applied_;
}

ACE_INLINE int
ACE_At_Thread_Exit::is_owner() const
{
  return is_owner_;
}

ACE_INLINE int
ACE_At_Thread_Exit::is_owner (int owner)
{
  is_owner_ = owner;
  return is_owner_;
}

ACE_INLINE void
ACE_At_Thread_Exit::do_apply (void)
{
  if (!this->was_applied_ && this->is_owner_)
    td_->at_pop();
}

ACE_INLINE
ACE_At_Thread_Exit::~ACE_At_Thread_Exit (void)
{
  this->do_apply ();
}

ACE_INLINE
ACE_At_Thread_Exit_Func::ACE_At_Thread_Exit_Func (void *object,
                                                  ACE_CLEANUP_FUNC func,
                                                  void *param)
  : object_(object),
    func_(func),
    param_(param)
{
}

ACE_INLINE
ACE_At_Thread_Exit_Func::~ACE_At_Thread_Exit_Func (void)
{
  this->do_apply ();
}

ACE_INLINE void
ACE_At_Thread_Exit_Func::apply ()
{
  func_ (object_, param_);
}
#endif /* ! ACE_USE_ONE_SHOT_AT_THREAD_EXIT */

ACE_INLINE
ACE_Thread_Descriptor_Base::ACE_Thread_Descriptor_Base (void)
  : ACE_OS_Thread_Descriptor (),
    thr_id_ (ACE_OS::NULL_thread),
    thr_handle_ (ACE_OS::NULL_hthread),
    grp_id_ (0),
    thr_state_ (ACE_Thread_Manager::ACE_THR_IDLE),
    task_ (0),
    next_ (0),
    prev_ (0)
{
}

ACE_INLINE
ACE_Thread_Descriptor_Base::~ACE_Thread_Descriptor_Base (void)
{
}

ACE_INLINE int
ACE_Thread_Descriptor_Base::operator==(const ACE_Thread_Descriptor_Base &rhs) const
{
  return ACE_OS::thr_cmp (this->thr_handle_, rhs.thr_handle_) == 0
    && ACE_OS::thr_equal (this->thr_id_, rhs.thr_id_) == 0;
}

ACE_INLINE int
ACE_Thread_Descriptor_Base::operator!=(const ACE_Thread_Descriptor_Base &rhs) const
{
  return !(*this == rhs);
}

ACE_INLINE ACE_Task_Base *
ACE_Thread_Descriptor_Base::task (void) const
{
  ACE_TRACE ("ACE_Thread_Descriptor_Base::task");
  return this->task_;
}

// Group ID.

ACE_INLINE int
ACE_Thread_Descriptor_Base::grp_id (void) const
{
  ACE_TRACE ("ACE_Thread_Descriptor_Base::grp_id");
  return grp_id_;
}

// Current state of the thread.
ACE_INLINE ACE_UINT32
ACE_Thread_Descriptor_Base::state (void) const
{
  ACE_TRACE ("ACE_Thread_Descriptor_Base::state");
  return thr_state_;
}

// Reset this base descriptor.
ACE_INLINE void
ACE_Thread_Descriptor_Base::reset (void)
{
  ACE_TRACE ("ACE_Thread_Descriptor_Base::reset");
  this->thr_id_ = ACE_OS::NULL_thread;
  this->thr_handle_ = ACE_OS::NULL_hthread;
  this->grp_id_ = 0;
  this->thr_state_ = ACE_Thread_Manager::ACE_THR_IDLE;
  this->task_ = 0;
  this->flags_ = 0;
}

// Unique thread id.
ACE_INLINE ACE_thread_t
ACE_Thread_Descriptor::self (void) const
{
  ACE_TRACE ("ACE_Thread_Descriptor::self");
  return this->thr_id_;
}

// Unique kernel-level thread handle.

ACE_INLINE void
ACE_Thread_Descriptor::self (ACE_hthread_t &handle)
{
  ACE_TRACE ("ACE_Thread_Descriptor::self");
  handle = this->thr_handle_;
}

#if !defined(ACE_USE_ONE_SHOT_AT_THREAD_EXIT)
ACE_INLINE void
ACE_Thread_Descriptor::log_msg_cleanup (ACE_Log_Msg* log_msg)

{
  log_msg_ = log_msg;
}
#endif /* !ACE_USE_ONE_SHOT_AT_THREAD_EXIT */

// Set the <next_> pointer
ACE_INLINE void
ACE_Thread_Descriptor::set_next (ACE_Thread_Descriptor *td)
{
  ACE_TRACE ("ACE_Thread_Descriptor::set_next");
  this->next_ = td;
}

// Get the <next_> pointer
ACE_INLINE ACE_Thread_Descriptor *
ACE_Thread_Descriptor::get_next (void) const
{
  ACE_TRACE ("ACE_Thread_Descriptor::get_next");
  return ACE_static_cast (ACE_Thread_Descriptor * ACE_CAST_CONST,
                          this->next_);
}

// Reset this thread descriptor
ACE_INLINE void
ACE_Thread_Descriptor::reset (ACE_Thread_Manager *tm)
{
  ACE_TRACE ("ACE_Thread_Descriptor::reset");
  this->ACE_Thread_Descriptor_Base::reset ();
#if defined(ACE_USE_ONE_SHOT_AT_THREAD_EXIT)
  this->cleanup_info_.cleanup_hook_ = 0;
  this->cleanup_info_.object_ = 0;
  this->cleanup_info_.param_ = 0;
#else /* !ACE_USE_ONE_SHOT_AT_THREAD_EXIT */
  this->at_exit_list_ = 0;
    // Start the at_exit hook list.
  this->tm_ = tm;
    // Setup the Thread_Manager.
  this->log_msg_ = 0;
  this->terminated_ = 0;
#endif /* !ACE_USE_ONE_SHOT_AT_THREAD_EXIT */
}

ACE_INLINE ACE_Thread_Descriptor *
ACE_Thread_Manager::thread_desc_self (void)
{
  // This method must be called with lock held.

  // Try to get it from cache.
  ACE_Thread_Descriptor *desc = ACE_LOG_MSG->thr_desc ();

#if 1
  ACE_ASSERT (desc != 0);
  // Thread descriptor should always get cached.
#else
  if (desc == 0)
    {
      ACE_thread_t id = ACE_OS::thr_self ();

      desc = this->find_thread (id);

      // Thread descriptor adapter might not have been put into the
      // list yet.
      if (desc != 0)
        // Update the TSS cache.
        ACE_LOG_MSG->thr_desc (desc);
    }
#endif
  return desc;
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

#if !defined(ACE_USE_ONE_SHOT_AT_THREAD_EXIT)
ACE_INLINE int
ACE_Thread_Manager::at_exit (ACE_At_Thread_Exit* at)
{
  return this->thread_desc_self ()->at_exit (at);
}

ACE_INLINE int
ACE_Thread_Manager::at_exit (ACE_At_Thread_Exit& at)
{
  return this->thread_desc_self ()->at_exit (at);
}
#endif /* !ACE_USE_ONE_SHOT_AT_THREAD_EXIT */

ACE_INLINE int
ACE_Thread_Manager::at_exit (void *object,
                             ACE_CLEANUP_FUNC cleanup_hook,
                             void *param)
{
  return this->thread_desc_self ()->at_exit (object,
                                             cleanup_hook,
                                             param);
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

ACE_INLINE int
ACE_Thread_Manager::register_as_terminated (ACE_Thread_Descriptor *td)
{
#if defined (VXWORKS)
  ACE_UNUSED_ARG (td);
#else  /* ! VXWORKS */
  ACE_Thread_Descriptor_Base *tdb;
  ACE_NEW_RETURN (tdb, ACE_Thread_Descriptor_Base (*td), -1);
  this->terminated_thr_list_.insert_tail (tdb);
#endif /* ! VXWORKS */
  return 0;
}

ACE_INLINE int
ACE_Thread_Manager::count_threads (void) const
{
  return this->thr_list_.size ();
}
