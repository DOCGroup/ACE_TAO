// FILE_Addr.cpp
// $Id$

#define ACE_BUILD_DLL
#include "ace/FILE_Addr.h"

#if !defined (__ACE_INLINE__)
#include "ace/FILE_Addr.i"
#endif /* __ACE_INLINE__ */

ACE_ALLOC_HOOK_DEFINE(ACE_FILE_Addr)

void
ACE_FILE_Addr::dump (void) const
{
  ACE_TRACE ("ACE_FILE_Addr::dump");

  ACE_DEBUG ((LM_DEBUG, ACE_BEGIN_DUMP, this));
  ACE_DEBUG ((LM_DEBUG, "filename_ = %s", this->filename_));
  ACE_DEBUG ((LM_DEBUG, ACE_END_DUMP));
}

