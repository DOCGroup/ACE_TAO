// -*- C++ -*-
#include "ace/OS_Memory.h"

ACE_BEGIN_VERSIONED_NAMESPACE_DECL

template <class ACE_LOCKING_MECHANISM>
ACE_INLINE
ACE_Lock_Adapter<ACE_LOCKING_MECHANISM>::ACE_Lock_Adapter (
  ACE_LOCKING_MECHANISM &lock)
  : lock_ (&lock)
{
}

template <class ACE_LOCKING_MECHANISM>
ACE_INLINE
ACE_Lock_Adapter<ACE_LOCKING_MECHANISM>::ACE_Lock_Adapter ()
{
  ACE_LOCKING_MECHANISM *lock = nullptr;
  ACE_NEW (lock, ACE_LOCKING_MECHANISM);
  this->owned_lock_.reset (lock);
  this->lock_ = this->owned_lock_.get ();
}

ACE_END_VERSIONED_NAMESPACE_DECL
