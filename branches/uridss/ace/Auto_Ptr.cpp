// $Id$

#ifndef ACE_AUTO_PTR_CPP
#define ACE_AUTO_PTR_CPP

#include "ace/Auto_Ptr.h"

#if !defined (__ACE_INLINE__)
#include "ace/Auto_Ptr.inl"
#endif /* __ACE_INLINE__ */

ACE_RCSID(ace, Auto_Ptr, "$Id$")

ACE_BEGIN_VERSIONED_NAMESPACE_DECL

ACE_ALLOC_HOOK_DEFINE(ACE_Auto_Basic_Ptr)
ACE_ALLOC_HOOK_DEFINE(ACE_Auto_Basic_Array_Ptr)

ACE_END_VERSIONED_NAMESPACE_DECL

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

#endif /* ACE_AUTO_PTR_CPP */
