// Auto_Ptr.cpp
// $Id$

#if !defined (ACE_AUTO_PTR_C)
#define ACE_AUTO_PTR_C

#define ACE_BUILD_DLL
#include "ace/Auto_Ptr.h"

#if !defined (__ACE_INLINE__)
#include "ace/Auto_Ptr.i"
#endif /* __ACE_INLINE__ */

#if defined (ACE_HAS_STANDARD_CPP_LIBRARY) 

ACE_ALLOC_HOOK_DEFINE(ACE_Auto_Basic_Ptr)

template<class X> void
ACE_Auto_Basic_Ptr<X>::dump (void) const
{
  ACE_TRACE ("ACE_Auto_Basic_Ptr<X>::dump");
}

template<class X> void
ACE_Auto_Basic_Ptr<X>::remove (X *& x)
{
  ACE_TRACE ("ACE_Auto_Basic_Ptr<X>::remove");

  X *tp = x;
  x = 0;
  delete tp;
}

#endif /* ACE_HAS_STANDARD_CPP_LIBRARY */

ACE_ALLOC_HOOK_DEFINE(ACE_Auto_Basic_Array_Ptr)

template<class X> void
ACE_Auto_Basic_Array_Ptr<X>::dump (void) const
{
  ACE_TRACE ("ACE_Auto_Basic_Array_Ptr<X>::dump");
}

template<class X> void
ACE_Auto_Basic_Array_Ptr<X>::remove (X *& x)
{
  ACE_TRACE ("ACE_Auto_Basic_Array_Ptr<X>::remove");

  X *tp = x;
  x = 0;
  delete [] tp;
}

#endif /* ACE_AUTO_PTR_C */
