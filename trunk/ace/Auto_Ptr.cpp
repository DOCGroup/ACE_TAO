// Auto_Ptr.cpp
// $Id$

#if !defined (ACE_AUTO_PTR_C)
#define ACE_AUTO_PTR_C

#define ACE_BUILD_DLL
#include "ace/Auto_Ptr.h"

#if !defined (__ACE_INLINE__)
#include "ace/Auto_Ptr.i"
#endif /* __ACE_INLINE__ */

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

#endif /* ACE_AUTO_PTR_C */
