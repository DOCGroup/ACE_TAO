// Shared_Object.cpp
// $Id$

#define ACE_BUILD_DLL
#include "ace/Shared_Object.h"
/* Provide the abstract base class used to access dynamic linking
   facilities */


#if !defined (__ACE_INLINE__)
#include "ace/Shared_Object.i"
#endif /* __ACE_INLINE__ */

// Initializes object when dynamic linking occurs.

int 
ACE_Shared_Object::init (int, char *[])
{
  ACE_TRACE ("ACE_Shared_Object::init");
  return -1;
}

// Terminates object when dynamic unlinking occurs.

int 
ACE_Shared_Object::fini (void)
{
  ACE_TRACE ("ACE_Shared_Object::fini");
  return -1;
}

// Returns information on active object.

int 
ACE_Shared_Object::info (char **, size_t) const
{
  ACE_TRACE ("ACE_Shared_Object::info");
  return -1;
}

// Need to give a default implementation.

ACE_Shared_Object::~ACE_Shared_Object (void)
{
  ACE_TRACE ("ACE_Shared_Object::~ACE_Shared_Object");
}
