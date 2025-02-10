// -*- C++ -*-
ACE_BEGIN_VERSIONED_NAMESPACE_DECL

// Explicitly destroy the mutex.
ACE_INLINE int
ACE_RW_Process_Mutex::remove ()
{
  return this->lock_.remove ();
}

// Acquire lock ownership (wait on priority queue if necessary).
ACE_INLINE int
ACE_RW_Process_Mutex::acquire ()
{
  return this->lock_.acquire ();
}

// Conditionally acquire lock (i.e., don't wait on queue).
ACE_INLINE int
ACE_RW_Process_Mutex::tryacquire ()
{
  return this->lock_.tryacquire ();
}

// Release lock and unblock a thread at head of priority queue.
ACE_INLINE int
ACE_RW_Process_Mutex::release ()
{
  return this->lock_.release ();
}

// Acquire lock ownership (wait on priority queue if necessary).
ACE_INLINE int
ACE_RW_Process_Mutex::acquire_read ()
{
  return this->lock_.acquire_read ();
}

// Acquire lock ownership (wait on priority queue if necessary).
ACE_INLINE int
ACE_RW_Process_Mutex::acquire_write ()
{
  return this->lock_.acquire_write ();
}

// Conditionally acquire a lock (i.e., won't block).
ACE_INLINE int
ACE_RW_Process_Mutex::tryacquire_read ()
{
  return this->lock_.tryacquire_read ();
}

// Conditionally acquire a lock (i.e., won't block).
ACE_INLINE int
ACE_RW_Process_Mutex::tryacquire_write ()
{
  return this->lock_.tryacquire_write ();
}

// Conditionally upgrade a lock (i.e., won't block).
ACE_INLINE int
ACE_RW_Process_Mutex::tryacquire_write_upgrade ()
{
  return this->lock_.tryacquire_write_upgrade ();
}

ACE_INLINE const ACE_File_Lock &
ACE_RW_Process_Mutex::lock () const
{
// ACE_TRACE ("ACE_RW_Process_Mutex::lock");
  return this->lock_;
}

ACE_END_VERSIONED_NAMESPACE_DECL
