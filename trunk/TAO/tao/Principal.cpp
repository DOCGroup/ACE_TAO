// $Id$

// Copyright 1994-1995 by Sun Microsystems Inc.
// All Rights Reserved
// ORB:		Principal identifier pseudo-objref

#include "tao/corba.h"

void
CORBA::release (CORBA::Principal_ptr principal)
{
  if (principal)
    principal->Release ();
}

CORBA::Boolean
CORBA::is_nil (CORBA::Principal_ptr principal)
{
  return (CORBA::Boolean) (principal == 0);
}

CORBA_Principal::CORBA_Principal (void)
{
}

CORBA_Principal::~CORBA_Principal (void)
{
  assert (refcount_ == 0);
}

CORBA::ULong
CORBA_Principal::AddRef (void)
{
  return ++refcount_;
}

CORBA::ULong
CORBA_Principal::Release (void)
{
  {
    if (--refcount_ != 0)
      return refcount_;
  }

  delete this;
  return 0;
}
