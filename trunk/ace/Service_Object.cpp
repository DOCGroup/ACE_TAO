// $Id$

// Service_Object.cpp

#define ACE_BUILD_DLL
#include "ace/Service_Object.h"

#if !defined (__ACE_INLINE__)
#include "ace/Service_Object.i"
#endif /* __ACE_INLINE__ */

ACE_ALLOC_HOOK_DEFINE(ACE_Service_Object)

ACE_ALLOC_HOOK_DEFINE(ACE_Service_Type)

void
ACE_Service_Type::dump (void) const
{
  ACE_TRACE ("ACE_Service_Type::dump");
}

ACE_Service_Type::ACE_Service_Type (const char *n,
					ACE_Service_Type_Impl *t,
					const ACE_SHLIB_HANDLE h,
					int active)
  : name_ (0),
    type_ (t),
    handle_ (h),
    active_ (active)
{
  ACE_TRACE ("ACE_Service_Type::ACE_Service_Type");
  this->name (n);
}

ACE_Service_Type::~ACE_Service_Type (void)
{
  ACE_TRACE ("ACE_Service_Type::~ACE_Service_Type");
  this->type_->fini ();
  if (this->handle_ != 0)
    ACE_OS::dlclose ((ACE_SHLIB_HANDLE) this->handle_);
  delete [] (char *) this->name_;
}

void
ACE_Service_Type::suspend (void) const
{
  ACE_TRACE ("ACE_Service_Type::suspend");
  ((ACE_Service_Type *) this)->active_ = 0;
  this->type_->suspend ();
}

void
ACE_Service_Type::resume (void) const
{
  ACE_TRACE ("ACE_Service_Type::resume");
  ((ACE_Service_Type *) this)->active_ = 1;
  this->type_->resume ();
}

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

