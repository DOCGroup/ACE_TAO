// Shared_Object.cpp
// $Id$

#include "ace/Svcconf/Shared_Object.h"
/* Provide the abstract base class used to access dynamic linking
   facilities */

#if !defined (__ACE_INLINE__)
#include "ace/Svcconf/Shared_Object.i"
#endif /* __ACE_INLINE__ */

ACE_RCSID(ace, Shared_Object, "$Id$")

// Initializes object when dynamic linking occurs.

int 
ACE_Shared_Object::init (int, ACE_TCHAR *[])
{
  ACE_TRACE ("ACE_Shared_Object::init");
  return 0;
}

// Terminates object when dynamic unlinking occurs.

int 
ACE_Shared_Object::fini (void)
{
  ACE_TRACE ("ACE_Shared_Object::fini");
  return 0;
}

// Returns information on active object.

int 
ACE_Shared_Object::info (ACE_TCHAR **, size_t) const
{
  ACE_TRACE ("ACE_Shared_Object::info");
  return 0;
}

// Need to give a default implementation.

ACE_Shared_Object::~ACE_Shared_Object (void)
{
  ACE_TRACE ("ACE_Shared_Object::~ACE_Shared_Object");
}
