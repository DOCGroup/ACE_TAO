// Auto_Ptr.cpp
// $Id$

#if !defined (ACE_AUTO_PTR_C)
#define ACE_AUTO_PTR_C

#define ACE_BUILD_DLL
#include "ace/Auto_Ptr.h"

#if !defined (__ACE_INLINE__)
#include "ace/Auto_Ptr.i"
#endif /* __ACE_INLINE__ */

ACE_ALLOC_HOOK_DEFINE(auto_basic_ptr)

template<class X> void
auto_basic_ptr<X>::dump (void) const
{
  ACE_TRACE ("auto_basic_ptr<X>::dump");
}

template<class X> void
auto_basic_ptr<X>::remove (X *& x)
{
  ACE_TRACE ("auto_basic_ptr<X>::remove");

  X *tp = x;
  x = 0;
  delete tp;
}

ACE_ALLOC_HOOK_DEFINE(auto_basic_array_ptr)

template<class X> void
auto_basic_array_ptr<X>::dump (void) const
{
  ACE_TRACE ("auto_basic_array_ptr<X>::dump");
}

template<class X> void
auto_basic_array_ptr<X>::remove (X *& x)
{
  ACE_TRACE ("auto_basic_array_ptr<X>::remove");

  X *tp = x;
  x = 0;
  delete [] tp;
}

#endif /* ACE_AUTO_PTR_C */
