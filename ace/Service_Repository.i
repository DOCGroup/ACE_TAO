/* -*- C++ -*- */
// $Id$

// Service_Repository.i

// Returns a count of the number of currently valid entries (counting
// both resumed and suspended entries).

ACE_INLINE int
ACE_Service_Repository::current_size (void)
{
  ACE_TRACE ("ACE_Service_Repository::current_size");
  ACE_MT (ACE_GUARD_RETURN (ACE_Thread_Mutex, ace_mon, this->lock_, -1));
  return this->current_size_;
}

// Returns a count of the total number of possible entries in the
// table.

ACE_INLINE int
ACE_Service_Repository::total_size (void)
{
  ACE_TRACE ("ACE_Service_Repository::total_size");
  ACE_MT (ACE_GUARD_RETURN (ACE_Thread_Mutex, ace_mon, this->lock_, -1));
  return this->total_size_;
}

