/* -*- C++ -*- */
// $Id$

// Service_Repository.i

// Returns a count of the number of currently valid entries (counting
// both resumed and suspended entries).

#if defined (ACE_MT_SAFE) && (ACE_MT_SAFE != 0)
#include "ace/Guard_T.h"
#include "ace/Thread_Mutex.h"
#endif /* ACE_MT_SAFE */

ACE_INLINE int
ACE_Service_Repository::current_size (void) const
{
  ACE_TRACE ("ACE_Service_Repository::current_size");
  ACE_MT (ACE_GUARD_RETURN (ACE_Thread_Mutex, ace_mon, (ACE_Thread_Mutex &) this->lock_, -1));
  return this->current_size_;
}

// Returns a count of the total number of possible entries in the
// table.

ACE_INLINE int
ACE_Service_Repository::total_size (void) const
{
  ACE_TRACE ("ACE_Service_Repository::total_size");
  ACE_MT (ACE_GUARD_RETURN (ACE_Thread_Mutex, ace_mon, (ACE_Thread_Mutex &) this->lock_, -1));
  return this->total_size_;
}

ACE_INLINE
ACE_Service_Repository_Iterator::~ACE_Service_Repository_Iterator (void)
{
}
