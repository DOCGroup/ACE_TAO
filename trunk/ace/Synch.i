/* -*- C++ -*- */
// $Id$

ACE_INLINE
ACE_Lock::ACE_Lock (void)
{
}

ACE_INLINE int
ACE_File_Lock::acquire_read (short whence, off_t start, off_t len)
{
// ACE_TRACE ("ACE_File_Lock::acquire_read");
  return ACE_OS::flock_rdlock (&this->lock_, whence, start, len);
}

ACE_INLINE int
ACE_File_Lock::tryacquire_read (short whence, off_t start, off_t len)
{
// ACE_TRACE ("ACE_File_Lock::tryacquire_read");
  return ACE_OS::flock_tryrdlock (&this->lock_, whence, start, len);
}

ACE_INLINE int
ACE_File_Lock::tryacquire_write (short whence, off_t start, off_t len)
{
// ACE_TRACE ("ACE_File_Lock::tryacquire_write");
  return ACE_OS::flock_trywrlock (&this->lock_, whence, start, len);
}

ACE_INLINE int
ACE_File_Lock::tryacquire (short whence, off_t start, off_t len)
{
// ACE_TRACE ("ACE_File_Lock::tryacquire");
  return this->tryacquire_write (whence, start, len);
}

ACE_INLINE int
ACE_File_Lock::acquire_write (short whence, off_t start, off_t len)
{
// ACE_TRACE ("ACE_File_Lock::acquire_write");
  return ACE_OS::flock_wrlock (&this->lock_, whence, start, len);
}

ACE_INLINE int
ACE_File_Lock::acquire (short whence, off_t start, off_t len)
{
// ACE_TRACE ("ACE_File_Lock::acquire");
  return this->acquire_write (whence, start, len);
}

ACE_INLINE int
ACE_File_Lock::release (short whence, off_t start, off_t len)
{
// ACE_TRACE ("ACE_File_Lock::release");
  return ACE_OS::flock_unlock (&this->lock_, whence, start, len);
}

ACE_INLINE int
ACE_File_Lock::remove (void)
{
// ACE_TRACE ("ACE_File_Lock::remove");

  int result = 0;

  if (this->removed_ == 0)
    {
      this->removed_ = 1;
      result = ACE_OS::flock_destroy (&this->lock_);
    }
  return result;
}

ACE_INLINE ACE_HANDLE
ACE_File_Lock::get_handle (void)
{
// ACE_TRACE ("ACE_File_Lock::get_handle");
  return this->lock_.handle_;
}

ACE_INLINE void
ACE_File_Lock::set_handle (ACE_HANDLE h)
{
// ACE_TRACE ("ACE_File_Lock::set_handle");
  this->lock_.handle_ = h;
}

ACE_INLINE const ACE_rwlock_t &
ACE_RW_Mutex::lock (void) const
{
// ACE_TRACE ("ACE_RW_Mutex::lock");
  return this->lock_;
}

ACE_INLINE int
ACE_RW_Mutex::remove (void)
{
// ACE_TRACE ("ACE_RW_Mutex::remove");
  int result = 0;
  if (this->removed_ == 0)
    {
      this->removed_ = 1;
      result = ACE_OS::rwlock_destroy (&this->lock_);
    }
  return result;
}

ACE_INLINE int
ACE_RW_Mutex::acquire_read (void)
{
// ACE_TRACE ("ACE_RW_Mutex::acquire_read");
  return ACE_OS::rw_rdlock (&this->lock_);
}

ACE_INLINE int
ACE_RW_Mutex::acquire_write (void)
{
// ACE_TRACE ("ACE_RW_Mutex::acquire_write");
  return ACE_OS::rw_wrlock (&this->lock_);
}

ACE_INLINE int
ACE_RW_Mutex::acquire (void)
{
// ACE_TRACE ("ACE_RW_Mutex::acquire");
  return ACE_OS::rw_wrlock (&this->lock_);
}

ACE_INLINE int
ACE_RW_Mutex::tryacquire_read (void)
{
// ACE_TRACE ("ACE_RW_Mutex::tryacquire_read");
  return ACE_OS::rw_tryrdlock (&this->lock_);
}

ACE_INLINE int
ACE_RW_Mutex::tryacquire_write (void)
{
// ACE_TRACE ("ACE_RW_Mutex::tryacquire_write");
  return ACE_OS::rw_trywrlock (&this->lock_);
}

ACE_INLINE int
ACE_RW_Mutex::tryacquire (void)
{
// ACE_TRACE ("ACE_RW_Mutex::tryacquire");
  return this->tryacquire_write ();
}

ACE_INLINE int
ACE_RW_Mutex::release (void)
{
// ACE_TRACE ("ACE_RW_Mutex::release");
  return ACE_OS::rw_unlock (&this->lock_);
}

#if defined (ACE_HAS_THREADS)
ACE_INLINE int
ACE_RW_Thread_Mutex::tryacquire_write_upgrade (void)
{
// ACE_TRACE ("ACE_RW_Thread_Mutex::tryacquire_write_upgrade");
  return ACE_OS::rw_trywrlock_upgrade (&this->lock_);
}
#endif /* ACE_HAS_THREADS */

ACE_INLINE int
ACE_Mutex::acquire_read (void)
{
// ACE_TRACE ("ACE_Mutex::acquire_read");
#if defined (CHORUS)
   if (this->process_lock_)
     return ACE_OS::mutex_lock (this->process_lock_);
#endif /* CHORUS */
  return ACE_OS::mutex_lock (&this->lock_);
}

ACE_INLINE int
ACE_Mutex::acquire_write (void)
{
// ACE_TRACE ("ACE_Mutex::acquire_write");
#if defined (CHORUS)
   if (this->process_lock_)
     return ACE_OS::mutex_lock (this->process_lock_);
#endif /* CHORUS */
  return ACE_OS::mutex_lock (&this->lock_);
}

ACE_INLINE int
ACE_Mutex::tryacquire_read (void)
{
// ACE_TRACE ("ACE_Mutex::tryacquire_read");
#if defined (CHORUS)
   if (this->process_lock_)
     return ACE_OS::mutex_trylock (this->process_lock_);
#endif /* CHORUS */
  return ACE_OS::mutex_trylock (&this->lock_);
}

ACE_INLINE const ACE_mutex_t &
ACE_Mutex::lock (void) const
{
// ACE_TRACE ("ACE_Mutex::lock");
#if defined (CHORUS)
  if (this->process_lock_)
    return *this->process_lock_;
#endif /* CHORUS */
  return this->lock_;
}

ACE_INLINE int
ACE_Mutex::tryacquire_write (void)
{
// ACE_TRACE ("ACE_Mutex::tryacquire_write");
#if defined (CHORUS)
   if (this->process_lock_)
     return ACE_OS::mutex_trylock (this->process_lock_);
#endif /* CHORUS */
  return ACE_OS::mutex_trylock (&this->lock_);
}

ACE_INLINE int
ACE_Mutex::acquire (void)
{
// ACE_TRACE ("ACE_Mutex::acquire");
#if defined (CHORUS)
   if (this->process_lock_)
     return ACE_OS::mutex_lock (this->process_lock_);
#endif /* CHORUS */
  return ACE_OS::mutex_lock (&this->lock_);
}

ACE_INLINE int
ACE_Mutex::tryacquire (void)
{
// ACE_TRACE ("ACE_Mutex::tryacquire");
#if defined (CHORUS)
   if (this->process_lock_)
     return ACE_OS::mutex_trylock (this->process_lock_);
#endif /* CHORUS */
  return ACE_OS::mutex_trylock (&this->lock_);
}

ACE_INLINE int
ACE_Mutex::release (void)
{
// ACE_TRACE ("ACE_Mutex::release");
#if defined (CHORUS)
   if (this->process_lock_)
     return ACE_OS::mutex_unlock (this->process_lock_);
#endif /* CHORUS */
  return ACE_OS::mutex_unlock (&this->lock_);
}

ACE_INLINE int
ACE_Mutex::remove (void)
{
// ACE_TRACE ("ACE_Mutex::remove");
#if defined (CHORUS)
   int result = 0;
   // Are we the owner?
   if (this->process_lock_ && this->lockname_)
     {
       if (this->removed_ == 0)
         {
           this->removed_ = 1;

           // Only destroy the lock if we're the ones who initialized
           // it.
           result = ACE_OS::mutex_destroy (this->process_lock_);
           ACE_OS::munmap (this->process_lock_,
                           sizeof (ACE_mutex_t));
           ACE_OS::shm_unlink (this->lockname_);
           ACE_OS::free (ACE_static_cast (void *,
                                          ACE_const_cast (LPTSTR,
                                                          this->lockname_)));
         }
     }
   else if (this->process_lock_)
     {
       ACE_OS::munmap (this->process_lock_,
                       sizeof (ACE_mutex_t));
       result = 0;
     }
   return result;
#else /* !CHORUS */
  int result = 0;
  if (this->removed_ == 0)
    {
      this->removed_ = 1;
      result = ACE_OS::mutex_destroy (&this->lock_);
    }
  return result;
#endif /* CHORUS */
}

ACE_INLINE const ACE_sema_t &
ACE_Semaphore::lock (void) const
{
// ACE_TRACE ("ACE_Semaphore::lock");
  return this->semaphore_;
}

ACE_INLINE int
ACE_Semaphore::remove (void)
{
// ACE_TRACE ("ACE_Semaphore::remove");
  int result = 0;
  if (this->removed_ == 0)
    {
      this->removed_ = 1;
      result = ACE_OS::sema_destroy (&this->semaphore_);
    }
  return result;
}

ACE_INLINE int
ACE_Semaphore::acquire (void)
{
// ACE_TRACE ("ACE_Semaphore::acquire");
  return ACE_OS::sema_wait (&this->semaphore_);
}

ACE_INLINE int
ACE_Semaphore::acquire (ACE_Time_Value &tv)
{
// ACE_TRACE ("ACE_Semaphore::acquire");
  return ACE_OS::sema_wait (&this->semaphore_, tv);
}

ACE_INLINE int
ACE_Semaphore::tryacquire (void)
{
// ACE_TRACE ("ACE_Semaphore::tryacquire");
  return ACE_OS::sema_trywait (&this->semaphore_);
}

ACE_INLINE int
ACE_Semaphore::release (void)
{
// ACE_TRACE ("ACE_Semaphore::release");
  return ACE_OS::sema_post (&this->semaphore_);
}

ACE_INLINE int
ACE_Semaphore::release (size_t release_count)
{
// ACE_TRACE ("ACE_Semaphore::release");
  return ACE_OS::sema_post (&this->semaphore_, release_count);
}

// Acquire semaphore ownership.  This calls <acquire> and is only
// here to make the <ACE_Semaphore> interface consistent with the
// other synchronization APIs.

ACE_INLINE int
ACE_Semaphore::acquire_read (void)
{
  return this->acquire ();
}

// Acquire semaphore ownership.  This calls <acquire> and is only
// here to make the <ACE_Semaphore> interface consistent with the
// other synchronization APIs.

ACE_INLINE int
ACE_Semaphore::acquire_write (void)
{
  return this->acquire ();
}

// Conditionally acquire semaphore (i.e., won't block).  This calls
// <tryacquire> and is only here to make the <ACE_Semaphore>
// interface consistent with the other synchronization APIs.

ACE_INLINE int
ACE_Semaphore::tryacquire_read (void)
{
  return this->tryacquire ();
}

// Conditionally acquire semaphore (i.e., won't block).  This calls
// <tryacquire> and is only here to make the <ACE_Semaphore>
// interface consistent with the other synchronization APIs.

ACE_INLINE int
ACE_Semaphore::tryacquire_write (void)
{
  return this->tryacquire ();
}

#if defined (ACE_WIN32) || defined (ACE_HAS_POSIX_SEM)
ACE_INLINE const ACE_mutex_t &
ACE_Process_Mutex::lock (void) const
{
// ACE_TRACE ("ACE_Process_Mutex::lock");
  return this->lock_.lock ();
}

ACE_INLINE const ACE_sema_t &
ACE_Process_Semaphore::lock (void) const
{
// ACE_TRACE ("ACE_Process_Semaphore::lock");
  return this->lock_.lock ();
}
#endif /* ACE_WIN32 */

// Acquire semaphore ownership.  This calls <acquire> and is only here
// to make the <ACE_Process_Semaphore> interface consistent with the
// other synchronization APIs.

ACE_INLINE int
ACE_Process_Semaphore::acquire_read (void)
{
  return this->acquire ();
}

// Acquire semaphore ownership.  This calls <acquire> and is only here
// to make the <ACE_Process_Semaphore> interface consistent with the
// other synchronization APIs.

ACE_INLINE int
ACE_Process_Semaphore::acquire_write (void)
{
  return this->acquire ();
}

// Conditionally acquire semaphore (i.e., won't block).  This calls
// <tryacquire> and is only here to make the <ACE_Process_Semaphore>
// interface consistent with the other synchronization APIs.

ACE_INLINE int
ACE_Process_Semaphore::tryacquire_read (void)
{
  return this->tryacquire ();
}

// Conditionally acquire semaphore (i.e., won't block).  This calls
// <tryacquire> and is only here to make the <ACE_Process_Semaphore>
// interface consistent with the other synchronization APIs.

ACE_INLINE int
ACE_Process_Semaphore::tryacquire_write (void)
{
  return this->tryacquire ();
}

#if defined (ACE_HAS_THREADS)

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
ACE_Thread_Mutex::acquire (void)
{
// ACE_TRACE ("ACE_Thread_Mutex::acquire");
  return ACE_OS::thread_mutex_lock (&this->lock_);
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

ACE_INLINE int
ACE_Condition_Thread_Mutex::remove (void)
{
// ACE_TRACE ("ACE_Condition_Thread_Mutex::remove");

  // <cond_destroy> is called in a loop if the condition variable is
  // BUSY.  This avoids a condition where a condition is signaled and
  // because of some timing problem, the thread that is to be signaled
  // has called the cond_wait routine after the signal call.  Since
  // the condition signal is not queued in any way, deadlock occurs.

  int result = 0;

  if (this->removed_ == 0)
    {
      this->removed_ = 1;

      while ((result = ACE_OS::cond_destroy (&this->cond_)) == -1
             && errno == EBUSY)
        {
          ACE_OS::cond_broadcast (&this->cond_);
          ACE_OS::thr_yield ();
        }
    }
  return result;
}

ACE_INLINE ACE_Thread_Mutex &
ACE_Condition_Thread_Mutex::mutex (void)
{
// ACE_TRACE ("ACE_Condition_Thread_Mutex::mutex");
  return this->mutex_;
}

ACE_INLINE
ACE_Recursive_Thread_Mutex::~ACE_Recursive_Thread_Mutex (void)
{
  // ACE_TRACE ("ACE_Recursive_Thread_Mutex::~ACE_Recursive_Thread_Mutex");
}

#if !defined (ACE_WIN32)
ACE_INLINE int
ACE_Recursive_Thread_Mutex::remove (void)
{
// ACE_TRACE ("ACE_Recursive_Thread_Mutex::remove");
  int result = 0;
  if (this->removed_ == 0)
    {
      this->removed_ = 1;
      this->nesting_mutex_.remove ();
      result = this->lock_available_.remove ();
    }
  return result;
}

ACE_INLINE void
ACE_Recursive_Thread_Mutex::set_thread_id (ACE_thread_t t)
{
// ACE_TRACE ("ACE_Recursive_Thread_Mutex::set_thread_id");
  this->owner_id_ = t;
}

ACE_INLINE int
ACE_Recursive_Thread_Mutex::acquire_read (void)
{
  return acquire ();
}

ACE_INLINE int
ACE_Recursive_Thread_Mutex::acquire_write (void)
{
  return acquire ();
}

ACE_INLINE int
ACE_Recursive_Thread_Mutex::tryacquire_read (void)
{
  return tryacquire ();
}

ACE_INLINE int
ACE_Recursive_Thread_Mutex::tryacquire_write (void)
{
  return tryacquire ();
}

#else /* ACE_WIN32 */
// The counter part of the following two functions for non-Win32 platforms
// are located in file Synch.cpp
ACE_INLINE ACE_thread_t
ACE_Recursive_Thread_Mutex::get_thread_id (void)
{
  // @@ The structure CriticalSection in Win32 doesn't hold
  // the thread handle of the thread that owns the lock.  However
  // it is still not clear at this point how to translate a
  // thread handle to its corresponding thread id.
  errno = ENOTSUP;
  return ACE_OS::NULL_thread;
}

ACE_INLINE int
ACE_Recursive_Thread_Mutex::get_nesting_level (void)
{
#if defined (ACE_HAS_WINCE)
  errno = ENOTSUP;
  return -1;                    // @@ Is this the right value to return?
#else
  return this->lock_.RecursionCount;
#endif /* ! ACE_HAS_WINCE */
}

#endif /* ACE_WIN32 */

#endif /* ACE_HAS_THREADS */

// Explicitly destroy the mutex.
ACE_INLINE int
ACE_Process_Mutex::remove (void)
{
  int retv = this->lock_.remove ();
  return retv;
}

// Acquire lock ownership (wait on priority queue if necessary).
ACE_INLINE int
ACE_Process_Mutex::acquire (void)
{
  return this->lock_.acquire ();
}

// Conditionally acquire lock (i.e., don't wait on queue).
ACE_INLINE int
ACE_Process_Mutex::tryacquire (void)
{
  return this->lock_.tryacquire ();
}

// Release lock and unblock a thread at head of priority queue.
ACE_INLINE int
ACE_Process_Mutex::release (void)
{
  return this->lock_.release ();
}

// Acquire lock ownership (wait on priority queue if necessary).
ACE_INLINE int
ACE_Process_Mutex::acquire_read (void)
{
  return this->lock_.acquire_read ();
}

// Acquire lock ownership (wait on priority queue if necessary).
ACE_INLINE int
ACE_Process_Mutex::acquire_write (void)
{
  return this->lock_.acquire_write ();
}

// Conditionally acquire a lock (i.e., won't block).
ACE_INLINE int
ACE_Process_Mutex::tryacquire_read (void)
{
  return this->lock_.tryacquire_read ();
}

// Conditionally acquire a lock (i.e., won't block).
ACE_INLINE int
ACE_Process_Mutex::tryacquire_write (void)
{
  return this->lock_.tryacquire_write ();
}

// Explicitly destroy the mutex.
ACE_INLINE int
ACE_RW_Process_Mutex::remove (void)
{
  return this->lock_.remove ();
}

// Acquire lock ownership (wait on priority queue if necessary).
ACE_INLINE int
ACE_RW_Process_Mutex::acquire (void)
{
  return this->lock_.acquire ();
}

// Conditionally acquire lock (i.e., don't wait on queue).
ACE_INLINE int
ACE_RW_Process_Mutex::tryacquire (void)
{
  return this->lock_.tryacquire ();
}

// Release lock and unblock a thread at head of priority queue.
ACE_INLINE int
ACE_RW_Process_Mutex::release (void)
{
  return this->lock_.release ();
}

// Acquire lock ownership (wait on priority queue if necessary).
ACE_INLINE int
ACE_RW_Process_Mutex::acquire_read (void)
{
  return this->lock_.acquire_read ();
}

// Acquire lock ownership (wait on priority queue if necessary).
ACE_INLINE int
ACE_RW_Process_Mutex::acquire_write (void)
{
  return this->lock_.acquire_write ();
}

// Conditionally acquire a lock (i.e., won't block).
ACE_INLINE int
ACE_RW_Process_Mutex::tryacquire_read (void)
{
  return this->lock_.tryacquire_read ();
}

// Conditionally acquire a lock (i.e., won't block).
ACE_INLINE int
ACE_RW_Process_Mutex::tryacquire_write (void)
{
  return this->lock_.tryacquire_write ();
}

ACE_INLINE const ACE_File_Lock &
ACE_RW_Process_Mutex::lock (void) const
{
// ACE_TRACE ("ACE_RW_Process_Mutex::lock");
  return this->lock_;
}

ACE_INLINE
ACE_Null_Barrier::ACE_Null_Barrier (u_int,
                                    const char *,
                                    void *)
{
}

ACE_INLINE
ACE_Null_Barrier::~ACE_Null_Barrier (void)
{
}

ACE_INLINE int
ACE_Null_Barrier::wait (void)
{
  return 0;
}

ACE_INLINE void
ACE_Null_Barrier::dump (void) const
{
}

ACE_INLINE
ACE_Null_Mutex::ACE_Null_Mutex (LPCTSTR)
{
}

ACE_INLINE
ACE_Null_Mutex::~ACE_Null_Mutex (void)
{
}

ACE_INLINE int
ACE_Null_Mutex::remove (void)
{
  return 0;
}

ACE_INLINE int
ACE_Null_Mutex::acquire (void)
{
  return 0;
}

ACE_INLINE int
ACE_Null_Mutex::tryacquire (void)
{
  return 0;
}

ACE_INLINE int
ACE_Null_Mutex::release (void)
{
  return 0;
}

ACE_INLINE int
ACE_Null_Mutex::acquire_write (void)
{
  return 0;
}

ACE_INLINE int
ACE_Null_Mutex::tryacquire_write (void)
{
  return 0;
}

ACE_INLINE int
ACE_Null_Mutex::acquire_read (void)
{
  return 0;
}

ACE_INLINE int
ACE_Null_Mutex::tryacquire_read (void)
{
  return 0;
}

ACE_INLINE void
ACE_Null_Mutex::dump (void) const
{
}

ACE_INLINE int
ACE_Noop_Token::renew (int, ACE_Time_Value *)
{
  return 0;
}

ACE_INLINE void
ACE_Noop_Token::dump (void) const
{
}


ACE_INLINE
ACE_Null_Condition::ACE_Null_Condition (const ACE_Null_Mutex &m,
                                        LPCTSTR,
                                        void*)
  : mutex_ ((ACE_Null_Mutex &) m)
{
}

ACE_INLINE ACE_Null_Condition::~ACE_Null_Condition (void)
{
}

ACE_INLINE int ACE_Null_Condition::remove (void)
{
  return 0;
}

ACE_INLINE int
ACE_Null_Condition::wait (ACE_Time_Value *)
{
  errno = ETIME;
  return -1;
}

ACE_INLINE int
ACE_Null_Condition::signal (void)
{
  return 0;
}

ACE_INLINE int
ACE_Null_Condition::broadcast (void)
{
  return 0;
}

ACE_INLINE ACE_Null_Mutex &
ACE_Null_Condition::mutex (void)
{
  return this->mutex_;
}

ACE_INLINE void
ACE_Null_Condition::dump (void) const
{
}

#if defined (ACE_USES_OBSOLETE_GUARD_CLASSES)
ACE_INLINE
ACE_Null_Mutex_Guard::ACE_Null_Mutex_Guard (ACE_Null_Mutex &)
{
}

ACE_INLINE
ACE_Null_Mutex_Guard::~ACE_Null_Mutex_Guard (void)
{
}

ACE_INLINE int
ACE_Null_Mutex_Guard::remove (void)
{
  return 0;
}

ACE_INLINE int
ACE_Null_Mutex_Guard::locked (void)
{
  return 1;
}

ACE_INLINE int
ACE_Null_Mutex_Guard::acquire (void)
{
  return 0;
}

ACE_INLINE int
ACE_Null_Mutex_Guard::tryacquire (void)
{
  return 0;
}

ACE_INLINE int
ACE_Null_Mutex_Guard::release (void)
{
  return 0;
}

ACE_INLINE void
ACE_Null_Mutex_Guard::dump (void) const
{
}
#endif /* ACE_USES_OBSOLETE_GUARD_CLASSES */

ACE_INLINE
ACE_TSS_Adapter::~ACE_TSS_Adapter (void)
{
}

ACE_INLINE
ACE_Manual_Event::~ACE_Manual_Event (void)
{
}

ACE_INLINE
ACE_Auto_Event::~ACE_Auto_Event (void)
{
}

#if defined (ACE_HAS_THREADS)
ACE_INLINE
ACE_RW_Thread_Mutex::~ACE_RW_Thread_Mutex (void)
{
}

ACE_INLINE
ACE_Thread_Semaphore::~ACE_Thread_Semaphore (void)
{
}

ACE_INLINE
ACE_Sub_Barrier::~ACE_Sub_Barrier (void)
{
}

ACE_INLINE
ACE_Barrier::~ACE_Barrier (void)
{
}

ACE_INLINE
ACE_Thread_Barrier::~ACE_Thread_Barrier (void)
{
}
#endif /* ACE_HAS_THREADS */
