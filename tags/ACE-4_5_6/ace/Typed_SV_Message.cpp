// Typed_SV_Message.cpp
// $Id$

#if !defined (ACE_TYPED_SV_MESSAGE_C)
#define ACE_TYPED_SV_MESSAGE_C
#define ACE_BUILD_DLL
#include "ace/Typed_SV_Message.h"

#if !defined (__ACE_INLINE__)
#include "ace/Typed_SV_Message.i"
#endif /* __ACE_INLINE__ */

ACE_ALLOC_HOOK_DEFINE(ACE_Typed_SV_Message)

template <class T> void
ACE_Typed_SV_Message<T>::dump (void) const
{
  ACE_TRACE ("ACE_Typed_SV_Message<T>::dump");
}

#endif /* ACE_TYPED_SV_MESSAGE_C */
