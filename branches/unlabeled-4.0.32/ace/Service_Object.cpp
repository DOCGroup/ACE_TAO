// Service_Object.cpp
// $Id$

#define ACE_BUILD_DLL
#include "ace/Service_Object.h"

#if !defined (__ACE_INLINE__)
#include "ace/Service_Object.i"
#endif /* __ACE_INLINE__ */

ACE_ALLOC_HOOK_DEFINE(ACE_Service_Object)

/* Provide the abstract base class common to all services */

ACE_Service_Object::ACE_Service_Object (void)
{
  ACE_TRACE ("ACE_Service_Object::ACE_Service_Object");
}

ACE_Service_Object::~ACE_Service_Object (void)
{
  ACE_TRACE ("ACE_Service_Object::~ACE_Service_Object");
}

int
ACE_Service_Object::suspend (void)
{
  ACE_TRACE ("ACE_Service_Object::suspend");
  return 0;
}

int
ACE_Service_Object::resume (void)
{
  ACE_TRACE ("ACE_Service_Object::resume");
  return 0;
}

ACE_ALLOC_HOOK_DEFINE(ACE_Service_Type)

void
ACE_Service_Type::dump (void) const
{
  ACE_TRACE ("ACE_Service_Type::dump");
}

ACE_Service_Type::ACE_Service_Type (const void *so, 
				    const char *s_name, 
				    unsigned int f)
  : obj_ (so), 
    flags_ (f)
{
  ACE_TRACE ("ACE_Service_Type::ACE_Service_Type");
  this->name (ACE_OS::strcpy (new char[::strlen (s_name) + 1], s_name));
}

int
ACE_Service_Type::fini (void) const
{
  ACE_TRACE ("ACE_Service_Type::fini");
  ACE_DEBUG ((LM_DEBUG, "destroying %s, flags = %d\n", 
	     this->name_, this->flags_));

  delete [] (char *) this->name_;
  if (ACE_BIT_ENABLED (this->flags_, ACE_Service_Type::DELETE_OBJ))
    delete (void *) this->object ();
  if (ACE_BIT_ENABLED (this->flags_, ACE_Service_Type::DELETE_THIS))
    delete (void *) this; // Prevent object's destructor from being called...
  return 0;
}

