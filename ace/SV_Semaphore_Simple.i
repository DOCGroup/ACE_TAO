/* -*- C++ -*- */
// $Id$

// SV_Semaphore_Simple.i

#include "ace/SV_Semaphore_Simple.h"
#include "ace/Trace.h"

#undef LUSED
#define LUSED 4 // # of chars used from name

inline int
ACE_SV_Semaphore_Simple::control (int cmd, 
				  semun arg, 
				  int semnum) const
{
  ACE_TRACE ("ACE_SV_Semaphore_Simple::control");
  return this->internal_id_ == -1 ? 
    -1 : ACE_OS::semctl (this->internal_id_, semnum, cmd, arg);
}

// Remove all SV_Semaphores associated with a particular key.  This
// call is intended to be called from a server, for example, when it
// is being shut down, as we do an IPC_RMID on the ACE_SV_Semaphore,
// regardless of whether other processes may be using it or not.  Most
// other processes should use close() below.
     
inline int 
ACE_SV_Semaphore_Simple::remove (void) const
{
  ACE_TRACE ("ACE_SV_Semaphore_Simple::remove");
  int result = this->control (IPC_RMID);
  ((ACE_SV_Semaphore_Simple *) this)->init ();
  return result;
}

// Close a ACE_SV_Semaphore, marking it as invalid for subsequent
// operations...

inline int 
ACE_SV_Semaphore_Simple::close (void)
{
  ACE_TRACE ("ACE_SV_Semaphore_Simple::close");
  return this->init ();
}

// General ACE_SV_Semaphore operation on an array of SV_Semaphores. 
     
inline int 
ACE_SV_Semaphore_Simple::op (sembuf op_vec[], int n) const
{
  ACE_TRACE ("ACE_SV_Semaphore_Simple::op");
  return this->internal_id_ == -1 
    ? -1 : ACE_OS::semop (this->internal_id_, op_vec, n);
}

inline
ACE_SV_Semaphore_Simple::ACE_SV_Semaphore_Simple (void)
{
  ACE_TRACE ("ACE_SV_Semaphore_Simple::ACE_SV_Semaphore_Simple");
  this->init ();
}

// Wait until a ACE_SV_Semaphore's value is greater than 0, the
// decrement it by 1 and return. Dijkstra's P operation, Tannenbaums
// DOWN operation.

inline int 
ACE_SV_Semaphore_Simple::acquire (int n, int flags) const
{
  ACE_TRACE ("ACE_SV_Semaphore_Simple::acquire");
  return this->op (-1, n, flags);
}

inline int 
ACE_SV_Semaphore_Simple::acquire_read (int n, int flags) const
{
  ACE_TRACE ("ACE_SV_Semaphore_Simple::acquire_read");
  return this->acquire (n, flags);
}

inline int 
ACE_SV_Semaphore_Simple::acquire_write (int n, int flags) const
{
  ACE_TRACE ("ACE_SV_Semaphore_Simple::acquire_write");
  return this->acquire (n, flags);
}

// Non-blocking version of acquire(). 

inline int 
ACE_SV_Semaphore_Simple::tryacquire (int n, int flags) const
{
  ACE_TRACE ("ACE_SV_Semaphore_Simple::tryacquire");
  return this->op (-1, n, flags | IPC_NOWAIT);
}

// Non-blocking version of acquire(). 

inline int 
ACE_SV_Semaphore_Simple::tryacquire_read (int n, int flags) const
{
  ACE_TRACE ("ACE_SV_Semaphore_Simple::tryacquire_read");
  return this->tryacquire (n, flags);
}

// Non-blocking version of acquire(). 

inline int 
ACE_SV_Semaphore_Simple::tryacquire_write (int n, int flags) const
{
  ACE_TRACE ("ACE_SV_Semaphore_Simple::tryacquire_write");
  return this->tryacquire (n, flags);
}

// Increment ACE_SV_Semaphore by one. Dijkstra's V operation,
// Tannenbaums UP operation.

inline int 
ACE_SV_Semaphore_Simple::release (int n, int flags) const
{
  ACE_TRACE ("ACE_SV_Semaphore_Simple::release");
  return this->op (1, n, flags);
}

inline int
ACE_SV_Semaphore_Simple::get_id (void) const
{
  ACE_TRACE ("ACE_SV_Semaphore_Simple::get_id");
  return this->internal_id_;
}

