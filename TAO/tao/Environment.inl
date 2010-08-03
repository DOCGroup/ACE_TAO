// -*- C++ -*-
//
// $Id$

#include "ace/OS_Memory.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

ACE_INLINE
CORBA::Exception *
CORBA::Environment::exception (void) const
{
  return this->exception_;
}

ACE_INLINE
CORBA::Environment *
CORBA::Environment::_duplicate (CORBA::Environment *x)
{
  if (x == 0)
    {
      return 0;
    }

  CORBA::Environment* ptr = 0;
  ACE_NEW_RETURN (ptr,
                  CORBA::Environment (*x),
                  0);
  return ptr;
}

ACE_INLINE
CORBA::Environment_ptr
CORBA::Environment::_nil (void)
{
  return static_cast <CORBA::Environment_ptr> (0);
}

TAO_END_VERSIONED_NAMESPACE_DECL
