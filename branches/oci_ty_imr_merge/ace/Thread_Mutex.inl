/* -*- C++ -*- */
// $Id$

ACE_INLINE const ACE_thread_mutex_t &
ACE_Thread_Mutex::lock (void) const
{
// ACE_TRACE ("ACE_Thread_Mutex::lock");
  return this->lock_;
}

ACE_INLINE int
ACE_Thread_Mutex::acquire_read (void)
{
// ACE_TRACE ("ACE_Thread_Mutex::acquire_read");
  return ACE_OS::thread_mutex_lock (&this->lock_);
}

ACE_INLINE int
ACE_Thread_Mutex::acquire_write (void)
{
// ACE_TRACE ("ACE_Thread_Mutex::acquire_write");
  return ACE_OS::thread_mutex_lock (&this->lock_);
}

ACE_INLINE int
ACE_Thread_Mutex::tryacquire_read (void)
{
// ACE_TRACE ("ACE_Thread_Mutex::tryacquire_read");
  return ACE_OS::thread_mutex_trylock (&this->lock_);
}

ACE_INLINE int
ACE_Thread_Mutex::tryacquire_write (void)
{
// ACE_TRACE ("ACE_Thread_Mutex::tryacquire_write");
  return ACE_OS::thread_mutex_trylock (&this->lock_);
}

ACE_INLINE int
ACE_Thread_Mutex::tryacquire_write_upgrade (void)
{
// ACE_TRACE ("ACE_Thread_Mutex::tryacquire_write_upgrade");
  return 0;
}

ACE_INLINE int
ACE_Thread_Mutex::acquire (void)
{
// ACE_TRACE ("ACE_Thread_Mutex::acquire");
  return ACE_OS::thread_mutex_lock (&this->lock_);
}

ACE_INLINE int
ACE_Thread_Mutex::acquire (ACE_Time_Value &tv)
{
  // ACE_TRACE ("ACE_Thread_Mutex::acquire");
  return ACE_OS::thread_mutex_lock (&this->lock_, tv);
}

ACE_INLINE int
ACE_Thread_Mutex::acquire (ACE_Time_Value *tv)
{
  // ACE_TRACE ("ACE_Thread_Mutex::acquire");
  return ACE_OS::thread_mutex_lock (&this->lock_, tv);
}

ACE_INLINE int
ACE_Thread_Mutex::tryacquire (void)
{
// ACE_TRACE ("ACE_Thread_Mutex::tryacquire");
  return ACE_OS::thread_mutex_trylock (&this->lock_);
}

ACE_INLINE int
ACE_Thread_Mutex::release (void)
{
// ACE_TRACE ("ACE_Thread_Mutex::release");
  return ACE_OS::thread_mutex_unlock (&this->lock_);
}

ACE_INLINE int
ACE_Thread_Mutex::remove (void)
{
// ACE_TRACE ("ACE_Thread_Mutex::remove");
  int result = 0;
  if (this->removed_ == 0)
    {
      this->removed_ = 1;
      result = ACE_OS::thread_mutex_destroy (&this->lock_);
    }
  return result;
}

#if defined (ACE_USES_OBSOLETE_GUARD_CLASSES)
ACE_INLINE int
ACE_Thread_Mutex_Guard::locked (void)
{
// ACE_TRACE ("ACE_Thread_Mutex_Guard::locked");
  return this->owner_ != -1;
}

// Explicitly acquire the lock.

ACE_INLINE int
ACE_Thread_Mutex_Guard::acquire (void)
{
// ACE_TRACE ("ACE_Thread_Mutex_Guard::acquire");
  return this->owner_ = this->lock_.acquire ();
}

// Conditionally acquire the lock (i.e., won't block).

ACE_INLINE int
ACE_Thread_Mutex_Guard::tryacquire (void)
{
// ACE_TRACE ("ACE_Thread_Mutex_Guard::tryacquire");
  return this->owner_ = this->lock_.tryacquire ();
}

// Implicitly and automatically acquire the lock.

ACE_INLINE
ACE_Thread_Mutex_Guard::ACE_Thread_Mutex_Guard (ACE_Thread_Mutex &m,
                                                int block)
  : lock_ (m)
{
// ACE_TRACE ("ACE_Thread_Mutex_Guard::ACE_Thread_Mutex_Guard");
  if (block)
    this->acquire ();
  else
    this->tryacquire ();
}

// Explicitly release the lock.

ACE_INLINE int
ACE_Thread_Mutex_Guard::release (void)
{
// ACE_TRACE ("ACE_Thread_Mutex_Guard::release");
  if (this->owner_ != -1)
    {
      this->owner_ = -1;
      return this->lock_.release ();
    }
  else
    return 0;
}

// Implicitly release the lock.

ACE_INLINE
ACE_Thread_Mutex_Guard::~ACE_Thread_Mutex_Guard (void)
{
// ACE_TRACE ("ACE_Thread_Mutex_Guard::~ACE_Thread_Mutex_Guard");
  this->release ();
}

// Explicitly release the lock.

ACE_INLINE int
ACE_Thread_Mutex_Guard::remove (void)
{
// ACE_TRACE ("ACE_Thread_Mutex_Guard::remove");
  this->owner_ = -1;
  return this->release ();
}
#endif /* ACE_USES_OBSOLETE_GUARD_CLASSES */
