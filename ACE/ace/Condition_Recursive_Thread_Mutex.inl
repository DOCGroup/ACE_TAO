// -*- C++ -*-
ACE_BEGIN_VERSIONED_NAMESPACE_DECL

ACE_INLINE int
ACE_Condition<ACE_Recursive_Thread_Mutex>::remove (void)
{
  return ACE_OS::cond_destroy (&this->cond_);
}

ACE_INLINE ACE_Recursive_Thread_Mutex &
ACE_Condition<ACE_Recursive_Thread_Mutex>::mutex (void)
{
  return this->mutex_;
}

ACE_INLINE int
ACE_Condition<ACE_Recursive_Thread_Mutex>::signal (void)
{
  return ACE_OS::cond_signal (&this->cond_);
}

ACE_INLINE int
ACE_Condition<ACE_Recursive_Thread_Mutex>::broadcast (void)
{
  return ACE_OS::cond_broadcast (&this->cond_);
}

ACE_END_VERSIONED_NAMESPACE_DECL
