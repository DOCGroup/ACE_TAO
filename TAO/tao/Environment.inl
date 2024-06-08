// -*- C++ -*-
#include "ace/OS_Memory.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

ACE_INLINE
CORBA::Exception *
CORBA::Environment::exception () const
{
  return this->exception_;
}

ACE_INLINE
CORBA::Environment *
CORBA::Environment::_duplicate (CORBA::Environment *x)
{
  if (!x)
    {
      return nullptr;
    }

  CORBA::Environment* ptr = nullptr;
  ACE_NEW_RETURN (ptr,
                  CORBA::Environment (*x),
                  nullptr);
  return ptr;
}

ACE_INLINE
CORBA::Environment_ptr
CORBA::Environment::_nil ()
{
  return nullptr;
}

TAO_END_VERSIONED_NAMESPACE_DECL
