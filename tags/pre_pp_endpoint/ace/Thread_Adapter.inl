// $Id$

ACE_INLINE long
ACE_OS_Thread_Descriptor::flags (void) const
{
  return flags_;
}


ACE_INLINE
ACE_OS_Thread_Descriptor::ACE_OS_Thread_Descriptor (long flags)
  : flags_ (flags)
{
}

ACE_INLINE ACE_Thread_Manager *
ACE_Thread_Adapter::thr_mgr (void)
{
  return this->thr_mgr_;
}
