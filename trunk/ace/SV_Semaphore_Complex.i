/* -*- C++ -*- */
// $Id$

// SV_Semaphore_Complex.i

#include "ace/Trace.h"

inline int 
ACE_SV_Semaphore_Complex::acquire (int n, int flags) const
{
  ACE_TRACE ("ACE_SV_Semaphore_Complex::acquire");
  return ACE_SV_Semaphore_Simple::acquire (n + 2, flags);
}

inline int 
ACE_SV_Semaphore_Complex::acquire_read (int n, int flags) const
{
  ACE_TRACE ("ACE_SV_Semaphore_Complex::acquire_read");
  return this->acquire (n, flags);
}

inline int 
ACE_SV_Semaphore_Complex::acquire_write (int n, int flags) const
{
  ACE_TRACE ("ACE_SV_Semaphore_Complex::acquire_write");
  return this->acquire (n, flags);
}

inline int 
ACE_SV_Semaphore_Complex::tryacquire (int n, int flags) const
{
  ACE_TRACE ("ACE_SV_Semaphore_Complex::tryacquire");
  return ACE_SV_Semaphore_Simple::tryacquire (n + 2, flags);
}

inline int 
ACE_SV_Semaphore_Complex::tryacquire_read (int n, int flags) const
{
  ACE_TRACE ("ACE_SV_Semaphore_Complex::tryacquire_read");
  return this->tryacquire (n, flags);
}

inline int 
ACE_SV_Semaphore_Complex::tryacquire_write (int n, int flags) const
{
  ACE_TRACE ("ACE_SV_Semaphore_Complex::tryacquire_write");
  return this->tryacquire (n, flags);
}

inline int 
ACE_SV_Semaphore_Complex::release (int n, int flags) const
{
  ACE_TRACE ("ACE_SV_Semaphore_Complex::release");
  return ACE_SV_Semaphore_Simple::release (n + 2, flags);
}

inline int 
ACE_SV_Semaphore_Complex::op (int val, int n, int flags) const
{
  ACE_TRACE ("ACE_SV_Semaphore_Complex::op");
  return ACE_SV_Semaphore_Simple::op (val, n + 2, flags);
}

inline int 
ACE_SV_Semaphore_Complex::op (sembuf op_vec[], int n) const
{
  ACE_TRACE ("ACE_SV_Semaphore_Complex::op");
  return ACE_SV_Semaphore_Simple::op (op_vec, n + 2);
}

inline int 
ACE_SV_Semaphore_Complex::control (int cmd, semun arg, int n) const
{
  ACE_TRACE ("ACE_SV_Semaphore_Complex::control");
  return ACE_SV_Semaphore_Simple::control (cmd, arg, n + 2);
}

inline int 
ACE_SV_Semaphore_Complex::control (int cmd, int value, int n) const
{
  ACE_TRACE ("ACE_SV_Semaphore_Complex::control");
  return ACE_SV_Semaphore_Simple::control (cmd, value, n + 2);
}
