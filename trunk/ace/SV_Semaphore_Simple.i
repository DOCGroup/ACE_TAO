/* -*- C++ -*- */
// $Id$

// SV_Semaphore_Simple.i

#include "ace/SV_Semaphore_Simple.h"
#include "ace/Trace.h"

inline int
ACE_SV_Semaphore_Simple::control (int cmd, 
				  semun arg, 
				  u_short n) const
{
  ACE_TRACE ("ACE_SV_Semaphore_Simple::control");
  return this->internal_id_ == -1 ? 
    -1 : ACE_OS::semctl (this->internal_id_, n, cmd, arg);
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
ACE_SV_Semaphore_Simple::op (sembuf op_vec[], u_short n) const
{
  ACE_TRACE ("ACE_SV_Semaphore_Simple::op");
  return this->internal_id_ == -1 
    ? -1 : ACE_OS::semop (this->internal_id_, op_vec, n);
}

// Wait until a ACE_SV_Semaphore's value is greater than 0, the
// decrement it by 1 and return. Dijkstra's P operation, Tannenbaums
// DOWN operation.

inline int 
ACE_SV_Semaphore_Simple::acquire (u_short n, int flags) const
{
  ACE_TRACE ("ACE_SV_Semaphore_Simple::acquire");
  return this->op (-1, n, flags);
}

inline int 
ACE_SV_Semaphore_Simple::acquire_read (u_short n, int flags) const
{
  ACE_TRACE ("ACE_SV_Semaphore_Simple::acquire_read");
  return this->acquire (n, flags);
}

inline int 
ACE_SV_Semaphore_Simple::acquire_write (u_short n, int flags) const
{
  ACE_TRACE ("ACE_SV_Semaphore_Simple::acquire_write");
  return this->acquire (n, flags);
}

// Non-blocking version of acquire(). 

inline int 
ACE_SV_Semaphore_Simple::tryacquire (u_short n, int flags) const
{
  ACE_TRACE ("ACE_SV_Semaphore_Simple::tryacquire");
  return this->op (-1, n, flags | IPC_NOWAIT);
}

// Non-blocking version of acquire(). 

inline int 
ACE_SV_Semaphore_Simple::tryacquire_read (u_short n, int flags) const
{
  ACE_TRACE ("ACE_SV_Semaphore_Simple::tryacquire_read");
  return this->tryacquire (n, flags);
}

// Non-blocking version of acquire(). 

inline int 
ACE_SV_Semaphore_Simple::tryacquire_write (u_short n, int flags) const
{
  ACE_TRACE ("ACE_SV_Semaphore_Simple::tryacquire_write");
  return this->tryacquire (n, flags);
}

// Increment ACE_SV_Semaphore by one. Dijkstra's V operation,
// Tannenbaums UP operation.

inline int 
ACE_SV_Semaphore_Simple::release (u_short n, int flags) const
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

