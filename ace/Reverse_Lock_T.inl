/* -*- C++ -*- */
// $Id$

template <class ACE_LOCKING_MECHANISM> ACE_INLINE
ACE_Reverse_Lock<ACE_LOCKING_MECHANISM>::ACE_Reverse_Lock (ACE_LOCKING_MECHANISM &lock,
                                                           ACE_Acquire_Method::METHOD_TYPE acquire_method)
  : lock_ (lock),
    acquire_method_ (acquire_method)
{
}

// Explicitly destroy the lock.
template <class ACE_LOCKING_MECHANISM> ACE_INLINE int
ACE_Reverse_Lock<ACE_LOCKING_MECHANISM>::remove (void)
{
  return this->lock_.remove ();
}

// Release the lock.
template <class ACE_LOCKING_MECHANISM> ACE_INLINE int
ACE_Reverse_Lock<ACE_LOCKING_MECHANISM>::acquire (void)
{
  return this->lock_.release ();
}

// Release the lock.
template <class ACE_LOCKING_MECHANISM> ACE_INLINE int
ACE_Reverse_Lock<ACE_LOCKING_MECHANISM>::tryacquire (void)
{
  ACE_NOTSUP_RETURN (-1);
}

// Acquire the lock.
template <class ACE_LOCKING_MECHANISM> ACE_INLINE int
ACE_Reverse_Lock<ACE_LOCKING_MECHANISM>::release (void)
{
  if (this->acquire_method_ == ACE_Acquire_Method::ACE_READ)
    return this->lock_.acquire_read ();
  else if (this->acquire_method_ == ACE_Acquire_Method::ACE_WRITE)
    return this->lock_.acquire_write ();
  else
    return this->lock_.acquire ();
}

// Release the lock.
template <class ACE_LOCKING_MECHANISM> ACE_INLINE int
ACE_Reverse_Lock<ACE_LOCKING_MECHANISM>::acquire_read (void)
{
  ACE_NOTSUP_RETURN (-1);
}

// Release the lock.
template <class ACE_LOCKING_MECHANISM> ACE_INLINE int
ACE_Reverse_Lock<ACE_LOCKING_MECHANISM>::acquire_write (void)
{
  ACE_NOTSUP_RETURN (-1);
}

// Release the lock.
template <class ACE_LOCKING_MECHANISM> ACE_INLINE int
ACE_Reverse_Lock<ACE_LOCKING_MECHANISM>::tryacquire_read (void)
{
  ACE_NOTSUP_RETURN (-1);
}

// Release the lock.
template <class ACE_LOCKING_MECHANISM> ACE_INLINE int
ACE_Reverse_Lock<ACE_LOCKING_MECHANISM>::tryacquire_write (void)
{
  ACE_NOTSUP_RETURN (-1);
}

// Release the lock.
template <class ACE_LOCKING_MECHANISM> ACE_INLINE int
ACE_Reverse_Lock<ACE_LOCKING_MECHANISM>::tryacquire_write_upgrade (void)
{
  ACE_NOTSUP_RETURN (-1);
}
