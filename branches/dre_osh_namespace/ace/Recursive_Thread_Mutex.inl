/* -*- C++ -*- */
// $Id$

ACE_INLINE ACE_recursive_thread_mutex_t &
ACE_Recursive_Thread_Mutex::mutex (void)
{
  return lock_;
}

ACE_INLINE ACE_thread_mutex_t &
ACE_Recursive_Thread_Mutex::get_nesting_mutex (void)
{
#if defined (ACE_HAS_RECURSIVE_MUTEXES)
  return ACE_static_cast (ACE_thread_mutex_t &,
                          lock_);
#else
  return lock_.nesting_mutex_;
#endif /* ACE_HAS_RECURSIVE_MUTEXES */
}

ACE_INLINE void
ACE_Recursive_Thread_Mutex::set_thread_id (ACE_thread_t t)
{
// ACE_TRACE ("ACE_Recursive_Thread_Mutex::set_thread_id");
#if defined (ACE_HAS_RECURSIVE_MUTEXES)
  ACE_UNUSED_ARG (t);
#else  /* ! ACE_HAS_RECURSIVE_MUTEXES */
  this->lock_.owner_id_ = t;
#endif /* ! ACE_HAS_RECURSIVE_MUTEXES */
}

ACE_INLINE int
ACE_Recursive_Thread_Mutex::acquire_read (void)
{
  return this->acquire ();
}

ACE_INLINE int
ACE_Recursive_Thread_Mutex::acquire_write (void)
{
  return this->acquire ();
}

ACE_INLINE int
ACE_Recursive_Thread_Mutex::tryacquire_read (void)
{
  return this->tryacquire ();
}

ACE_INLINE int
ACE_Recursive_Thread_Mutex::tryacquire_write (void)
{
  return this->tryacquire ();
}

ACE_INLINE int
ACE_Recursive_Thread_Mutex::tryacquire_write_upgrade (void)
{
  return 0;
}
