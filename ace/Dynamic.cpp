// Dynamic.cpp
// $Id$

#define ACE_BUILD_DLL
#include "ace/Dynamic.h"

#if !defined (__ACE_INLINE__)
#include "ace/Dynamic.i"
#endif /* __ACE_INLINE__ */

ACE_Dynamic::ACE_Dynamic (void)
  : is_dynamic_ (0) 
{
  ACE_TRACE ("ACE_Dynamic::ACE_Dynamic");
}

