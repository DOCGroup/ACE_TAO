#ifndef ACE_ATOMIC_OP_C
#define ACE_ATOMIC_OP_C

#include "ace/Atomic_Op.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#if !defined (__ACE_INLINE__)
// On non-Win32 platforms, this code will be treated as normal code.
#if !defined (ACE_WIN32)
#include "ace/Atomic_Op.i"
#endif /* !ACE_WIN32 */

ACE_RCSID(ace, Atomic_Op, "$Id$")



#endif /* __ACE_INLINE__ */

ACE_ALLOC_HOOK_DEFINE(ACE_Atomic_Op)

template <class ACE_LOCK, class TYPE> ACE_LOCK &
ACE_Atomic_Op<ACE_LOCK, TYPE>::mutex (void)
{
// ACE_TRACE ("ACE_Atomic_Op<ACE_LOCK, TYPE>::mutex");
  return this->mutex_;
}

template <class ACE_LOCK, class TYPE> void
ACE_Atomic_Op<ACE_LOCK, TYPE>::dump (void) const
{
// ACE_TRACE ("ACE_Atomic_Op<ACE_LOCK, TYPE>::dump");

  ACE_DEBUG ((LM_DEBUG, ACE_BEGIN_DUMP, this));
  this->mutex_.dump ();
  ACE_DEBUG ((LM_DEBUG, ACE_END_DUMP));
}

template <class ACE_LOCK, class TYPE>
ACE_Atomic_Op<ACE_LOCK, TYPE>::ACE_Atomic_Op (void)
  : value_ (0)
{
// ACE_TRACE ("ACE_Atomic_Op<ACE_LOCK, TYPE>::ACE_Atomic_Op");
}

template <class ACE_LOCK, class TYPE>
ACE_Atomic_Op<ACE_LOCK, TYPE>::ACE_Atomic_Op (const TYPE &c)
  : value_ (c)
{
// ACE_TRACE ("ACE_Atomic_Op<ACE_LOCK, TYPE>::ACE_Atomic_Op");
}


#endif /*ACE_ATOMIC_OP */
