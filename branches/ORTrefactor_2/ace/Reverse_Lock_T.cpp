// $Id$

#ifndef ACE_REVERSE_LOCK_T_C
#define ACE_REVERSE_LOCK_T_C

#include "ace/Reverse_Lock_T.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

ACE_RCSID(ace, Reverse_Lock_T, "$Id$")

#if !defined (__ACE_INLINE__)
#include "ace/Reverse_Lock_T.inl"
#endif /* __ACE_INLINE__ */

template <class ACE_LOCKING_MECHANISM>
ACE_Reverse_Lock<ACE_LOCKING_MECHANISM>::~ACE_Reverse_Lock (void)
{
}

#endif /* ACE_REVERSE_LOCK_T_C */
