/* -*- C++ -*- */
// $Id$

// Synch.i

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
  
  return ACE_OS::flock_destroy (&this->lock_);
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
  return ACE_OS::rwlock_destroy (&this->lock_);
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

ACE_INLINE int
ACE_Mutex::acquire_read (void)
{
// ACE_TRACE ("ACE_Mutex::acquire_read");
  return ACE_OS::mutex_lock (&this->lock_);
}  

ACE_INLINE int 
ACE_Mutex::acquire_write (void)
{
// ACE_TRACE ("ACE_Mutex::acquire_write");
  return ACE_OS::mutex_lock (&this->lock_);
}

ACE_INLINE int 
ACE_Mutex::tryacquire_read (void)
{
// ACE_TRACE ("ACE_Mutex::tryacquire_read");
  return ACE_OS::mutex_trylock (&this->lock_);
}

ACE_INLINE const ACE_mutex_t &
ACE_Mutex::lock (void) const
{
// ACE_TRACE ("ACE_Mutex::lock");
  return this->lock_;
}

ACE_INLINE int 
ACE_Mutex::tryacquire_write (void)
{
// ACE_TRACE ("ACE_Mutex::tryacquire_write");
  return ACE_OS::mutex_trylock (&this->lock_);
}

ACE_INLINE int 
ACE_Mutex::acquire (void)
{
// ACE_TRACE ("ACE_Mutex::acquire");
  return ACE_OS::mutex_lock (&this->lock_);
}
 
ACE_INLINE int
ACE_Mutex::tryacquire (void)
{
// ACE_TRACE ("ACE_Mutex::tryacquire");
  return ACE_OS::mutex_trylock (&this->lock_);
}

ACE_INLINE int 
ACE_Mutex::release (void)
{
// ACE_TRACE ("ACE_Mutex::release");
  return ACE_OS::mutex_unlock (&this->lock_);
}

ACE_INLINE int
ACE_Mutex::remove (void)
{
// ACE_TRACE ("ACE_Mutex::remove");
  return ACE_OS::mutex_destroy (&this->lock_);
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
  return ACE_OS::sema_destroy (&this->semaphore_);
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
  return this->lock_->lock ();
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
  return ACE_OS::thread_mutex_destroy (&this->lock_);
}

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

ACE_INLINE int
ACE_Condition_Thread_Mutex::remove (void)
{
// ACE_TRACE ("ACE_Condition_Thread_Mutex::remove");

  // cond_destroy() is called in a loop if the condition variable is
  // BUSY.  This avoids a condition where a condition is signaled and
  // because of some timing problem, the thread that is to be signaled
  // has called the cond_wait routine after the signal call.  Since
  // the condition signal is not queued in any way, deadlock occurs.

  int result = 0;

  while ((result = ACE_OS::cond_destroy (&this->cond_)) == -1
	 && errno == EBUSY)
    {
      ACE_OS::cond_broadcast (&this->cond_);
      ACE_OS::thr_yield ();
    }

  return result;
}

ACE_INLINE ACE_Thread_Mutex &
ACE_Condition_Thread_Mutex::mutex (void)
{
// ACE_TRACE ("ACE_Condition_Thread_Mutex::mutex");
  return this->mutex_;
}

ACE_INLINE int
ACE_Recursive_Thread_Mutex::remove (void)
{
// ACE_TRACE ("ACE_Recursive_Thread_Mutex::remove");
  this->nesting_mutex_.remove ();
  return this->lock_available_.remove ();
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

#endif /* ACE_HAS_THREADS */

