// $Id$

#ifndef ACE_AUTO_INCDEC_T_C
#define ACE_AUTO_INCDEC_T_C

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "ace/Auto_IncDec_T.h"
#include "ace/Log_Msg.h"

ACE_RCSID(ace, Auto_IncDec_T, "Auto_IncDec_T.cpp, by Edan Ayal")

#if !defined (__ACE_INLINE__)
#include "ace/Auto_IncDec_T.i"
#endif /* __ACE_INLINE__ */

ACE_ALLOC_HOOK_DEFINE(ACE_Auto_IncDec)

template <class ACE_SAFELY_INCREMENTABLE_DECREMENTABLE> void
ACE_Auto_IncDec<ACE_SAFELY_INCREMENTABLE_DECREMENTABLE>::dump (void) const
{
#if defined (ACE_HAS_DUMP)
// ACE_TRACE ("ACE_Auto_IncDec<ACE_SAFELY_INCREMENTABLE_DECREMENTABLE>::dump");

  ACE_DEBUG ((LM_DEBUG, ACE_BEGIN_DUMP, this));
  ACE_DEBUG ((LM_DEBUG, ACE_END_DUMP));
#endif /* ACE_HAS_DUMP */
}

#endif /* ACE_AUTO_INCDEC_T_C */
