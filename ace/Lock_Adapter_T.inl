/* -*- C++ -*- */
// $Id$

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

// Conditionally try to upgrade a lock held for read to a write lock.
// If the locking mechanism doesn't support read locks then this just
// calls <acquire>. Returns 0 on success, -1 on failure.

template <class ACE_LOCKING_MECHANISM> ACE_INLINE int
ACE_Lock_Adapter<ACE_LOCKING_MECHANISM>::tryacquire_write_upgrade (void)
{
  return this->lock_->tryacquire_write_upgrade ();
}
