// -*- C++ -*-
//
// $Id$

template <class ACE_LOCKING_MECHANISM>
ACE_INLINE
ACE_Lock_Adapter<ACE_LOCKING_MECHANISM>::ACE_Lock_Adapter (
  ACE_LOCKING_MECHANISM &lock)
  : lock_ (&lock),
    delete_lock_ (0)
{
}
