/* -*- C++ -*- */
// $Id$

// Thread_Manager.i

#if !defined(ACE_USE_ONE_SHOT_AT_THREAD_EXIT)
ACE_INLINE
ACE_At_Thread_Exit::ACE_At_Thread_Exit()
  : td_(0),
    was_applied_(0),
    is_owner_(1)

{
}

ACE_INLINE int
ACE_At_Thread_Exit::was_applied() const

{
   return was_applied_;
}

ACE_INLINE int
ACE_At_Thread_Exit::was_applied(int applied)

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
ACE_At_Thread_Exit::is_owner(int owner)

{
   is_owner_ = owner;
   return is_owner_;
}

ACE_INLINE void
ACE_At_Thread_Exit::do_apply()

{
   if (!this->was_applied_ && this->is_owner_)
    {
      td_->at_pop();
    }
}

ACE_INLINE
ACE_At_Thread_Exit::~ACE_At_Thread_Exit()

{
   this->do_apply();
}

ACE_INLINE
ACE_At_Thread_Exit_Func::ACE_At_Thread_Exit_Func (
  void* object,
  ACE_CLEANUP_FUNC func,
  void* param
)
  : object_(object),
    func_(func),
    param_(param)

{
}

ACE_INLINE
ACE_At_Thread_Exit_Func::~ACE_At_Thread_Exit_Func()

{
   this->do_apply();
}

ACE_INLINE void
ACE_At_Thread_Exit_Func::apply()

{
   func_(object_, param_);
}
#endif /* ! ACE_USE_ONE_SHOT_AT_THREAD_EXIT */

ACE_INLINE
ACE_Thread_Descriptor_Base::ACE_Thread_Descriptor_Base (void)
  : thr_id_ (ACE_OS::NULL_thread),
    thr_handle_ (ACE_OS::NULL_hthread)
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

#if !defined(ACE_USE_ONE_SHOT_AT_THREAD_EXIT)
ACE_INLINE void
ACE_Thread_Descriptor::log_msg_cleanup(ACE_Log_Msg* log_msg)

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
