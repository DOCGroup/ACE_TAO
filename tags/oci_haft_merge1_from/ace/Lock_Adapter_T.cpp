// $Id$

#ifndef ACE_LOCK_ADAPTER_T_C
#define ACE_LOCK_ADAPTER_T_C

#include "ace/Lock_Adapter_T.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

ACE_RCSID(ace, Lock_Adapter_T, "$Id$")

#if !defined (__ACE_INLINE__)
#include "ace/Lock_Adapter_T.inl"
#endif /* __ACE_INLINE__ */

// This constructor isn't inlined, because SunPRO C++ 4.2 + patch
// 104631-07 has trouble compiling TAO with it inline.
template <class ACE_LOCKING_MECHANISM>
ACE_Lock_Adapter<ACE_LOCKING_MECHANISM>::ACE_Lock_Adapter (void)
  : lock_ (0),
    delete_lock_ (1)
{
  ACE_NEW (this->lock_,
           ACE_LOCKING_MECHANISM);
}

#endif /* ACE_LOCK_ADAPTER_T_C */
