/* -*- C++ -*- */
// $Id$

// SV_Semaphore_Complex.i

#ifdef ACE_SUBSET_0
#include "ace/Logging/Trace.h"
#endif

ASYS_INLINE int 
ACE_SV_Semaphore_Complex::acquire (u_short n, int flags) const
{

#ifdef ACE_SUBSET_0
  ACE_TRACE ("ACE_SV_Semaphore_Complex::acquire");
#endif
  return ACE_SV_Semaphore_Simple::acquire ((u_short) n + 2, flags);
}

ASYS_INLINE int 
ACE_SV_Semaphore_Complex::acquire_read (u_short n, int flags) const
{
#ifdef ACE_SUBSET_0
  ACE_TRACE ("ACE_SV_Semaphore_Complex::acquire_read");
#endif
  return this->acquire (n, flags);
}

ASYS_INLINE int 
ACE_SV_Semaphore_Complex::acquire_write (u_short n, int flags) const
{
#ifdef ACE_SUBSET_0
  ACE_TRACE ("ACE_SV_Semaphore_Complex::acquire_write");
#endif
  return this->acquire (n, flags);
}

ASYS_INLINE int 
ACE_SV_Semaphore_Complex::tryacquire (u_short n, int flags) const
{
#ifdef ACE_SUBSET_0
  ACE_TRACE ("ACE_SV_Semaphore_Complex::tryacquire");
#endif
  return ACE_SV_Semaphore_Simple::tryacquire ((u_short) n + 2, flags);
}

ASYS_INLINE int 
ACE_SV_Semaphore_Complex::tryacquire_read (u_short n, int flags) const
{
#ifdef ACE_SUBSET_0
  ACE_TRACE ("ACE_SV_Semaphore_Complex::tryacquire_read");
#endif
  return this->tryacquire (n, flags);
}

ASYS_INLINE int 
ACE_SV_Semaphore_Complex::tryacquire_write (u_short n, int flags) const
{
#ifdef ACE_SUBSET_0
  ACE_TRACE ("ACE_SV_Semaphore_Complex::tryacquire_write");
#endif
  return this->tryacquire (n, flags);
}

ASYS_INLINE int 
ACE_SV_Semaphore_Complex::release (u_short n, int flags) const
{
#ifdef ACE_SUBSET_0
  ACE_TRACE ("ACE_SV_Semaphore_Complex::release");
#endif
  return ACE_SV_Semaphore_Simple::release ((u_short) n + 2, flags);
}

ASYS_INLINE int 
ACE_SV_Semaphore_Complex::op (int val, u_short n, int flags) const
{
#ifdef ACE_SUBSET_0
  ACE_TRACE ("ACE_SV_Semaphore_Complex::op");
#endif
  return ACE_SV_Semaphore_Simple::op (val, (u_short) n + 2, flags);
}

ASYS_INLINE int 
ACE_SV_Semaphore_Complex::op (sembuf op_vec[], u_short n) const
{
#ifdef ACE_SUBSET_0
  ACE_TRACE ("ACE_SV_Semaphore_Complex::op");
#endif
  return ACE_SV_Semaphore_Simple::op (op_vec, (u_short) n + 2);
}

ASYS_INLINE int 
ACE_SV_Semaphore_Complex::control (int cmd, semun arg, u_short n) const
{
#ifdef ACE_SUBSET_0
  ACE_TRACE ("ACE_SV_Semaphore_Complex::control");
#endif
  return ACE_SV_Semaphore_Simple::control (cmd, arg, (u_short) n + 2);
}

ASYS_INLINE int 
ACE_SV_Semaphore_Complex::control (int cmd, int value, u_short n) const
{
#ifdef ACE_SUBSET_0
  ACE_TRACE ("ACE_SV_Semaphore_Complex::control");
#endif
  return ACE_SV_Semaphore_Simple::control (cmd, value, (u_short) n + 2);
}
