#ifndef ACE_LOCK_ADAPTER_T_CPP
#define ACE_LOCK_ADAPTER_T_CPP

#include "ace/Lock_Adapter_T.h"
#include "ace/OS_Memory.h" // for ACE_NEW

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#if !defined (__ACE_INLINE__)
#include "ace/Lock_Adapter_T.inl"
#endif /* __ACE_INLINE__ */

ACE_BEGIN_VERSIONED_NAMESPACE_DECL

template <class ACE_LOCKING_MECHANISM>
ACE_Lock_Adapter<ACE_LOCKING_MECHANISM>::~ACE_Lock_Adapter ()
{
  if (this->delete_lock_)
    delete this->lock_;
}

// Explicitly destroy the lock.
template <class ACE_LOCKING_MECHANISM> int
ACE_Lock_Adapter<ACE_LOCKING_MECHANISM>::remove ()
{
  return this->lock_->remove ();
}

// Block the thread until the lock is acquired.
template <class ACE_LOCKING_MECHANISM> int
ACE_Lock_Adapter<ACE_LOCKING_MECHANISM>::acquire ()
{
  return this->lock_->acquire ();
}

// Conditionally acquire the lock (i.e., won't block).

template <class ACE_LOCKING_MECHANISM> int
ACE_Lock_Adapter<ACE_LOCKING_MECHANISM>::tryacquire ()
{
  return this->lock_->tryacquire ();
}

// Release the lock.

template <class ACE_LOCKING_MECHANISM> int
ACE_Lock_Adapter<ACE_LOCKING_MECHANISM>::release ()
{
  return this->lock_->release ();
}

// Block until the thread acquires a read lock.  If the locking
// mechanism doesn't support read locks then this just calls
// <acquire>.

template <class ACE_LOCKING_MECHANISM> int
ACE_Lock_Adapter<ACE_LOCKING_MECHANISM>::acquire_read ()
{
  return this->lock_->acquire_read ();
}

// Block until the thread acquires a write lock.  If the locking
// mechanism doesn't support read locks then this just calls
// <acquire>.

template <class ACE_LOCKING_MECHANISM> int
ACE_Lock_Adapter<ACE_LOCKING_MECHANISM>::acquire_write ()
{
  return this->lock_->acquire_write ();
}

// Conditionally acquire a read lock.  If the locking mechanism
// doesn't support read locks then this just calls <acquire>.

template <class ACE_LOCKING_MECHANISM> int
ACE_Lock_Adapter<ACE_LOCKING_MECHANISM>::tryacquire_read ()
{
  return this->lock_->tryacquire_read ();
}

// Conditionally acquire a write lock.  If the locking mechanism
// doesn't support write locks then this just calls <acquire>.

template <class ACE_LOCKING_MECHANISM> int
ACE_Lock_Adapter<ACE_LOCKING_MECHANISM>::tryacquire_write ()
{
  return this->lock_->tryacquire_write ();
}

// Conditionally try to upgrade a lock held for read to a write lock.
// If the locking mechanism doesn't support read locks then this just
// calls <acquire>. Returns 0 on success, -1 on failure.

template <class ACE_LOCKING_MECHANISM> int
ACE_Lock_Adapter<ACE_LOCKING_MECHANISM>::tryacquire_write_upgrade ()
{
  return this->lock_->tryacquire_write_upgrade ();
}

ACE_END_VERSIONED_NAMESPACE_DECL

#endif /* ACE_LOCK_ADAPTER_T_CPP */
