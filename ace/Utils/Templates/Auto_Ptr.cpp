// Auto_Ptr.cpp
// $Id$

#if !defined (ACE_AUTO_PTR_C)
#define ACE_AUTO_PTR_C

#include "ace/Utils/Templates/Auto_Ptr.h"

#if !defined (__ACE_INLINE__)
#include "ace/Utils/Templates/Auto_Ptr.i"
#endif /* __ACE_INLINE__ */

ACE_RCSID(ace, Auto_Ptr, "$Id$")

ACE_ALLOC_HOOK_DEFINE(ACE_Auto_Basic_Ptr)

template<class X> void
ACE_Auto_Basic_Ptr<X>::dump (void) const
{
  ACE_TRACE ("ACE_Auto_Basic_Ptr<X>::dump");
}

ACE_ALLOC_HOOK_DEFINE(ACE_Auto_Basic_Array_Ptr)

template<class X> void
ACE_Auto_Basic_Array_Ptr<X>::dump (void) const
{
  ACE_TRACE ("ACE_Auto_Basic_Array_Ptr<X>::dump");
}

#if defined (__MINGW32__)
# if defined (ACE_HAS_STANDARD_CPP_LIBRARY) && \
             (ACE_HAS_STANDARD_CPP_LIBRARY != 0)
# if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)
template alloc;
#  elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)
#pragma instantiate alloc
#  endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */
# endif /* ACE_HAS_STANDARD_CPP_LIBRARY != 0 */
#endif /* __MINGW32__ */

#endif /* ACE_AUTO_PTR_C */
