/* -*- C++ -*- */
// $Id$

// SV_Semaphore_Complex.i

#include "ace/Trace.h"

ASYS_INLINE int 
ACE_SV_Semaphore_Complex::acquire (u_short n, int flags) const
{
  ACE_TRACE ("ACE_SV_Semaphore_Complex::acquire");
  return ACE_SV_Semaphore_Simple::acquire ((u_short) n + 2, flags);
}

ASYS_INLINE int 
ACE_SV_Semaphore_Complex::acquire_read (u_short n, int flags) const
{
  ACE_TRACE ("ACE_SV_Semaphore_Complex::acquire_read");
  return this->acquire (n, flags);
}

ASYS_INLINE int 
ACE_SV_Semaphore_Complex::acquire_write (u_short n, int flags) const
{
  ACE_TRACE ("ACE_SV_Semaphore_Complex::acquire_write");
  return this->acquire (n, flags);
}

ASYS_INLINE int 
ACE_SV_Semaphore_Complex::tryacquire (u_short n, int flags) const
{
  ACE_TRACE ("ACE_SV_Semaphore_Complex::tryacquire");
  return ACE_SV_Semaphore_Simple::tryacquire ((u_short) n + 2, flags);
}

ASYS_INLINE int 
ACE_SV_Semaphore_Complex::tryacquire_read (u_short n, int flags) const
{
  ACE_TRACE ("ACE_SV_Semaphore_Complex::tryacquire_read");
  return this->tryacquire (n, flags);
}

ASYS_INLINE int 
ACE_SV_Semaphore_Complex::tryacquire_write (u_short n, int flags) const
{
  ACE_TRACE ("ACE_SV_Semaphore_Complex::tryacquire_write");
  return this->tryacquire (n, flags);
}

ASYS_INLINE int 
ACE_SV_Semaphore_Complex::release (u_short n, int flags) const
{
  ACE_TRACE ("ACE_SV_Semaphore_Complex::release");
  return ACE_SV_Semaphore_Simple::release ((u_short) n + 2, flags);
}

ASYS_INLINE int 
ACE_SV_Semaphore_Complex::op (int val, u_short n, int flags) const
{
  ACE_TRACE ("ACE_SV_Semaphore_Complex::op");
  return ACE_SV_Semaphore_Simple::op (val, (u_short) n + 2, flags);
}

ASYS_INLINE int 
ACE_SV_Semaphore_Complex::op (sembuf op_vec[], u_short n) const
{
  ACE_TRACE ("ACE_SV_Semaphore_Complex::op");
  return ACE_SV_Semaphore_Simple::op (op_vec, (u_short) n + 2);
}

ASYS_INLINE int 
ACE_SV_Semaphore_Complex::control (int cmd, semun arg, u_short n) const
{
  ACE_TRACE ("ACE_SV_Semaphore_Complex::control");
  return ACE_SV_Semaphore_Simple::control (cmd, arg, (u_short) n + 2);
}

ASYS_INLINE int 
ACE_SV_Semaphore_Complex::control (int cmd, int value, u_short n) const
{
  ACE_TRACE ("ACE_SV_Semaphore_Complex::control");
  return ACE_SV_Semaphore_Simple::control (cmd, value, (u_short) n + 2);
}
