// $Id$

ACE_INLINE long
ACE_OS_Thread_Descriptor::flags (void) const
{
  ACE_OS_TRACE ("ACE_OS_Thread_Descriptor::flags");
  return flags_;
}


ACE_INLINE
ACE_OS_Thread_Descriptor::ACE_OS_Thread_Descriptor (long flags)
  : flags_ (flags)
{
}

ACE_INLINE void
ACE_Thread_Adapter::set_log_msg_hooks (ACE_INIT_LOG_MSG_HOOK init_hook,
                                       ACE_INHERIT_LOG_MSG_HOOK inherit_hook,
                                       ACE_CLOSE_LOG_MSG_HOOK close_hook,
                                       ACE_SYNC_LOG_MSG_HOOK sync_hook)
{
  ACE_Thread_Adapter::init_log_msg_hook_ = init_hook;
  ACE_Thread_Adapter::inherit_log_msg_hook_ = inherit_hook;
  ACE_Thread_Adapter::close_log_msg_hook_ = close_hook;
  ACE_Thread_Adapter::sync_log_msg_hook_ = sync_hook;
}

ACE_INLINE ACE_Thread_Manager *
ACE_Thread_Adapter::thr_mgr (void)
{
  return this->thr_mgr_;
}

ACE_INLINE
ACE_Thread_Adapter::~ACE_Thread_Adapter (void)
{
}

ACE_INLINE ACE_THR_C_FUNC
ACE_Thread_Adapter::entry_point (void)
{
  return this->entry_point_;
}
