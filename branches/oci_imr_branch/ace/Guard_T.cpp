// $Id$

#ifndef ACE_GUARD_T_C
#define ACE_GUARD_T_C

#include "ace/Guard_T.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

ACE_RCSID(ace, Guard_T, "$Id$")

#if !defined (__ACE_INLINE__)
#include "ace/Guard_T.inl"
#endif /* __ACE_INLINE__ */

// ****************************************************************
// ACE_ALLOC_HOOK_DEFINE(ACE_Guard)

template <class ACE_LOCK> void
ACE_Guard<ACE_LOCK>::dump (void) const
{
#if defined (ACE_HAS_DUMP)
// ACE_TRACE ("ACE_Guard<ACE_LOCK>::dump");

  ACE_DEBUG ((LM_DEBUG, ACE_BEGIN_DUMP, this));
  ACE_DEBUG ((LM_DEBUG, ACE_LIB_TEXT ("mutex_ = %x\n"), this->lock_));
  ACE_DEBUG ((LM_DEBUG, ACE_LIB_TEXT ("owner_ = %d\n"), this->owner_));
  ACE_DEBUG ((LM_DEBUG, ACE_END_DUMP));
#endif /* ACE_HAS_DUMP */
}

// ACE_ALLOC_HOOK_DEFINE(ACE_Write_Guard)

template <class ACE_LOCK> void
ACE_Write_Guard<ACE_LOCK>::dump (void) const
{
#if defined (ACE_HAS_DUMP)
// ACE_TRACE ("ACE_Write_Guard<ACE_LOCK>::dump");
  ACE_Guard<ACE_LOCK>::dump ();
#endif /* ACE_HAS_DUMP */
}

// ACE_ALLOC_HOOK_DEFINE(ACE_Read_Guard)

template <class ACE_LOCK> void
ACE_Read_Guard<ACE_LOCK>::dump (void) const
{
// ACE_TRACE ("ACE_Read_Guard<ACE_LOCK>::dump");
  ACE_Guard<ACE_LOCK>::dump ();
}

#endif /* ACE_GUARD_T_C */
