/* -*- C++ -*- */
// $Id$

// SV_Semaphore_Simple.i

#ifdef ACE_SUBSET_0
#include "ace/Logging/Trace.h"
#endif

ASYS_INLINE int
ACE_SV_Semaphore_Simple::control (int cmd, 
				  semun arg, 
				  u_short n) const
{
#ifdef ACE_SUBSET_0
  ACE_TRACE ("ACE_SV_Semaphore_Simple::control");
#endif
  return this->internal_id_ == -1 ? 
    -1 : ACE_OS::semctl (this->internal_id_, n, cmd, arg);
}

// Close a ACE_SV_Semaphore, marking it as invalid for subsequent
// operations...

ASYS_INLINE int 
ACE_SV_Semaphore_Simple::close (void)
{
#ifdef ACE_SUBSET_0
  ACE_TRACE ("ACE_SV_Semaphore_Simple::close");
#endif
  return this->init ();
}

// General ACE_SV_Semaphore operation on an array of SV_Semaphores. 
     
ASYS_INLINE int 
ACE_SV_Semaphore_Simple::op (sembuf op_vec[], u_short n) const
{
#ifdef ACE_SUBSET_0
  ACE_TRACE ("ACE_SV_Semaphore_Simple::op");
#endif
  return this->internal_id_ == -1 
    ? -1 : ACE_OS::semop (this->internal_id_, op_vec, n);
}

// Wait until a ACE_SV_Semaphore's value is greater than 0, the
// decrement it by 1 and return. Dijkstra's P operation, Tannenbaums
// DOWN operation.

ASYS_INLINE int 
ACE_SV_Semaphore_Simple::acquire (u_short n, int flags) const
{
#ifdef ACE_SUBSET_0
  ACE_TRACE ("ACE_SV_Semaphore_Simple::acquire");
#endif
  return this->op (-1, n, flags);
}

ASYS_INLINE int 
ACE_SV_Semaphore_Simple::acquire_read (u_short n, int flags) const
{
#ifdef ACE_SUBSET_0
  ACE_TRACE ("ACE_SV_Semaphore_Simple::acquire_read");
#endif
  return this->acquire (n, flags);
}

ASYS_INLINE int 
ACE_SV_Semaphore_Simple::acquire_write (u_short n, int flags) const
{
#ifdef ACE_SUBSET_0
  ACE_TRACE ("ACE_SV_Semaphore_Simple::acquire_write");
#endif
  return this->acquire (n, flags);
}

// Non-blocking version of acquire(). 

ASYS_INLINE int 
ACE_SV_Semaphore_Simple::tryacquire (u_short n, int flags) const
{
#ifdef ACE_SUBSET_0
  ACE_TRACE ("ACE_SV_Semaphore_Simple::tryacquire");
#endif
  return this->op (-1, n, flags | IPC_NOWAIT);
}

// Non-blocking version of acquire(). 

ASYS_INLINE int 
ACE_SV_Semaphore_Simple::tryacquire_read (u_short n, int flags) const
{
#ifdef ACE_SUBSET_0
  ACE_TRACE ("ACE_SV_Semaphore_Simple::tryacquire_read");
#endif
  return this->tryacquire (n, flags);
}

// Non-blocking version of acquire(). 

ASYS_INLINE int 
ACE_SV_Semaphore_Simple::tryacquire_write (u_short n, int flags) const
{
#ifdef ACE_SUBSET_0
  ACE_TRACE ("ACE_SV_Semaphore_Simple::tryacquire_write");
#endif
  return this->tryacquire (n, flags);
}

// Increment ACE_SV_Semaphore by one. Dijkstra V operation,
// Tannenbaums UP operation.

ASYS_INLINE int 
ACE_SV_Semaphore_Simple::release (u_short n, int flags) const
{
#ifdef ACE_SUBSET_0
  ACE_TRACE ("ACE_SV_Semaphore_Simple::release");
#endif
  return this->op (1, n, flags);
}

ASYS_INLINE int
ACE_SV_Semaphore_Simple::get_id (void) const
{
#ifdef ACE_SUBSET_0
  ACE_TRACE ("ACE_SV_Semaphore_Simple::get_id");
#endif
  return this->internal_id_;
}

