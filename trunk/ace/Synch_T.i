/* -*- C++ -*- */
// $Id$

// Synch_T.i

#include "ace/Thread.h"

template <class ACE_LOCK> ACE_INLINE
ACE_Guard<ACE_LOCK>::ACE_Guard (ACE_LOCK &l)
  : lock_ (&l), owner_ (0)
{
  this->acquire ();
}

template <class ACE_LOCK> ACE_INLINE
ACE_Guard<ACE_LOCK>::ACE_Guard (ACE_LOCK &l, int block)
  : lock_ (&l), owner_ (0)
{
  if (block)
    this->acquire ();
  else
    this->tryacquire ();
}

// Implicitly and automatically acquire (or try to acquire) the
// lock.

template <class ACE_LOCK> ACE_INLINE
ACE_Guard<ACE_LOCK>::~ACE_Guard (void)
{
  this->release ();
}

// Implicitly release the lock.

template <class ACE_LOCK> ACE_INLINE int
ACE_Guard<ACE_LOCK>::acquire (void)
{
  return this->owner_ = this->lock_->acquire ();
}

// Explicitly acquire the lock.

template <class ACE_LOCK> ACE_INLINE int
ACE_Guard<ACE_LOCK>::tryacquire (void)
{
  return this->owner_ = this->lock_->tryacquire ();
}

// Conditionally acquire the lock (i.e., won't block).

template <class ACE_LOCK> ACE_INLINE int
ACE_Guard<ACE_LOCK>::release (void)
{
  if (this->owner_ != -1)
    {
      this->owner_ = -1;
      return this->lock_->release ();
    }
  else
    return 0;
}

// Explicitly release the lock, but only if it is held!

template <class ACE_LOCK> ACE_INLINE int
ACE_Guard<ACE_LOCK>::locked (void)
{
  return this->owner_ != -1;
}

// 1 if locked, 0 if couldn't acquire the lock
// (errno will contain the reason for this).

template <class ACE_LOCK> ACE_INLINE int
ACE_Guard<ACE_LOCK>::remove (void)
{
  return this->lock_->remove ();
}
// Explicitly remove the lock.

template <class ACE_LOCKING_MECHANISM> ACE_INLINE
ACE_Lock_Adapter<ACE_LOCKING_MECHANISM>::ACE_Lock_Adapter (ACE_LOCKING_MECHANISM &lock)
  : lock_ (&lock),
    delete_lock_ (0)
{
}

template <class ACE_LOCKING_MECHANISM> ACE_INLINE
ACE_Lock_Adapter<ACE_LOCKING_MECHANISM>::~ACE_Lock_Adapter (void)
{
  if (this->delete_lock_)
    delete this->lock_;
}

// Explicitly destroy the lock.
template <class ACE_LOCKING_MECHANISM> ACE_INLINE int
ACE_Lock_Adapter<ACE_LOCKING_MECHANISM>::remove (void)
{
  return this->lock_->remove ();
}

// Block the thread until the lock is acquired.
template <class ACE_LOCKING_MECHANISM> ACE_INLINE int
ACE_Lock_Adapter<ACE_LOCKING_MECHANISM>::acquire (void)
{
  return this->lock_->acquire ();
}

// Conditionally acquire the lock (i.e., won't block).

template <class ACE_LOCKING_MECHANISM> ACE_INLINE int
ACE_Lock_Adapter<ACE_LOCKING_MECHANISM>::tryacquire (void)
{
  return this->lock_->tryacquire ();
}

// Release the lock.

template <class ACE_LOCKING_MECHANISM> ACE_INLINE int
ACE_Lock_Adapter<ACE_LOCKING_MECHANISM>::release (void)
{
  return this->lock_->release ();
}

// Block until the thread acquires a read lock.  If the locking
// mechanism doesn't support read locks then this just calls
// <acquire>.

template <class ACE_LOCKING_MECHANISM> ACE_INLINE int
ACE_Lock_Adapter<ACE_LOCKING_MECHANISM>::acquire_read (void)
{
  return this->lock_->acquire_read ();
}

// Block until the thread acquires a write lock.  If the locking
// mechanism doesn't support read locks then this just calls
// <acquire>.

template <class ACE_LOCKING_MECHANISM> ACE_INLINE int
ACE_Lock_Adapter<ACE_LOCKING_MECHANISM>::acquire_write (void)
{
  return this->lock_->acquire_write ();
}

// Conditionally acquire a read lock.  If the locking mechanism
// doesn't support read locks then this just calls <acquire>.

template <class ACE_LOCKING_MECHANISM> ACE_INLINE int
ACE_Lock_Adapter<ACE_LOCKING_MECHANISM>::tryacquire_read (void)
{
  return this->lock_->tryacquire_read ();
}

// Conditionally acquire a write lock.  If the locking mechanism
// doesn't support write locks then this just calls <acquire>.

template <class ACE_LOCKING_MECHANISM> ACE_INLINE int
ACE_Lock_Adapter<ACE_LOCKING_MECHANISM>::tryacquire_write (void)
{
  return this->lock_->tryacquire_write ();
}

#if defined (ACE_HAS_THREADS)

template<class MUTEX> ACE_INLINE int
ACE_Condition<MUTEX>::remove (void)
{
  // ACE_TRACE ("ACE_Condition<MUTEX>::remove");

  // cond_destroy() is called in a loop if the condition variable is
  // BUSY.  This avoids a condition where a condition is signaled and
  // because of some timing problem, the thread that is to be signaled
  // has called the cond_wait routine after the signal call.  Since
  // the condition signal is not queued in any way, deadlock occurs.

  int result = 0;

#if defined (CHORUS)
  // Are we the owner?
  if (this->process_cond_ && this->condname_)
    {
      // Only destroy the condition if we're the ones who initialized
      // it.
      while ((result = ACE_OS::cond_destroy (this->process_cond_)) == -1
             && errno == EBUSY)
        {
          ACE_OS::cond_broadcast (this->process_cond_);
          ACE_OS::thr_yield ();
        }
      ACE_OS::munmap (this->process_cond_,
                      sizeof (ACE_cond_t));
      ACE_OS::shm_unlink (this->condname_);
      ACE_OS::free (ACE_static_cast (void *,
                                     ACE_const_cast (LPTSTR,
                                                     this->condname_)));
    }
  else if (this->process_cond_)
    {
      ACE_OS::munmap (this->process_cond_,
                      sizeof (ACE_cond_t));
      result = 0;
    }
  else
#endif /* CHORUS */

    while ((result = ACE_OS::cond_destroy (&this->cond_)) == -1
           && errno == EBUSY)
      {
        ACE_OS::cond_broadcast (&this->cond_);
        ACE_OS::thr_yield ();
      }

  return result;
}

template<class MUTEX> ACE_INLINE MUTEX &
ACE_Condition<MUTEX>::mutex (void)
{
  // ACE_TRACE ("ACE_Condition<MUTEX>::mutex");
  return this->mutex_;
}

template <class MUTEX> ACE_INLINE int
ACE_Condition<MUTEX>::signal (void)
{
// ACE_TRACE ("ACE_Condition<MUTEX>::signal");
#if defined (CHORUS)
  if (this->process_cond_ != 0)
    return ACE_OS::cond_signal (this->process_cond_);
#endif /* CHORUS */
  return ACE_OS::cond_signal (&this->cond_);
}

template <class MUTEX> ACE_INLINE int
ACE_Condition<MUTEX>::broadcast (void)
{
// ACE_TRACE ("ACE_Condition<MUTEX>::broadcast");
#if defined (CHORUS)
  if (this->process_cond_ != 0)
    return ACE_OS::cond_broadcast (this->process_cond_);
#endif /* CHORUS */
  return ACE_OS::cond_broadcast (&this->cond_);
}

#endif /* ACE_HAS_THREADS */

#if !(defined (ACE_HAS_THREADS) && (defined (ACE_HAS_THREAD_SPECIFIC_STORAGE) || defined (ACE_HAS_TSS_EMULATION)))
template <class TYPE> ACE_INLINE
ACE_TSS<TYPE>::ACE_TSS (TYPE *type)
  : type_ (type)
{
}

template <class TYPE> ACE_INLINE TYPE *
ACE_TSS<TYPE>::ts_object (void) const
{
  return this->type_;
}

template <class TYPE> ACE_INLINE TYPE *
ACE_TSS<TYPE>::ts_object (TYPE *type)
{
  this->type_ = type;
  return this->type_;
}

template <class TYPE> ACE_INLINE TYPE *
ACE_TSS<TYPE>::ts_get (void) const
{
  return this->type_;
}

#endif /* ! (defined (ACE_HAS_THREADS) && (defined (ACE_HAS_THREAD_SPECIFIC_STORAGE) || defined (ACE_HAS_TSS_EMULATION))) */
