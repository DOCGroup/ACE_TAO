// SV_Message.cpp
// $Id$

#define ACE_BUILD_DLL
#include "ace/SV_Message.h"

#if !defined (__ACE_INLINE__)
#include "ace/SV_Message.i"
#endif /* __ACE_INLINE__ */

ACE_ALLOC_HOOK_DEFINE(ACE_SV_Message)

void
ACE_SV_Message::dump (void) const
{
  ACE_TRACE ("ACE_SV_Message::dump");
}

