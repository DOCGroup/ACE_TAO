// $Id$

// Copyright 1994-1995 by Sun Microsystems Inc.
// All Rights Reserved
// ORB:		Principal identifier pseudo-objref

#include "tao/corba.h"

#if !defined (__ACE_INLINE__)
#include "tao/Principal.i"
#endif /* __ACE_INLINE__ */

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

TAO_OutputCDR&
operator<< (TAO_OutputCDR& cdr, CORBA_Principal* x)
{
  if (x != 0)
    {
      CORBA::ULong length  = x->id.length ();
      cdr.write_long (length);
      cdr.write_octet_array (x->id.get_buffer (), length);
    }
  else
    {
      cdr.write_ulong (0);
    }
  return cdr;
}

TAO_InputCDR&
operator>> (TAO_InputCDR& cdr, CORBA_Principal*& x)
{
  CORBA::ULong length;
  cdr.read_ulong (length);
  if (length == 0 || !cdr.good_bit ())
    {
      x = 0;
    }
  else
    {
      ACE_NEW_RETURN (x, CORBA::Principal, cdr);
      x->id.length (length);
      cdr.read_octet_array (x->id.get_buffer (), length);
    }
  return cdr;
}
