// $Id$

#ifndef ACE_AUTO_INCDEC_T_C
#define ACE_AUTO_INCDEC_T_C

#ifdef ACE_SUBSET_0
#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */
#endif

#include "ace/Utils/Templates/Auto_IncDec_T.h"

#ifdef ACE_SUBSET_0
#include "ace/Logging/Log_Msg.h"
#endif

ACE_RCSID(ace, Auto_IncDec_T, "Auto_IncDec_T.cpp, by Edan Ayal")

#if !defined (__ACE_INLINE__)
#include "ace/Utils/Templates/Auto_IncDec_T.i"
#endif /* __ACE_INLINE__ */

ACE_ALLOC_HOOK_DEFINE(ACE_Auto_IncDec)

template <class ACE_SAFELY_INCREMENTABLE_DECREMENTABLE> void
ACE_Auto_IncDec<ACE_SAFELY_INCREMENTABLE_DECREMENTABLE>::dump (void) const
{
// ACE_TRACE ("ACE_Auto_IncDec<ACE_SAFELY_INCREMENTABLE_DECREMENTABLE>::dump");

  ACE_DEBUG ((LM_DEBUG, ACE_BEGIN_DUMP, this));
  ACE_DEBUG ((LM_DEBUG, ACE_END_DUMP));
}

#endif /* ACE_AUTO_INCDEC_T_C */
