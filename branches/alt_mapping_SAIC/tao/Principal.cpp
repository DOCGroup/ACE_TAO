// $Id$

// Copyright 1994-1995 by Sun Microsystems Inc.
// All Rights Reserved
// ORB: Principal identifier pseudo-objref

#include "tao/Principal.h"
#include "tao/CDR.h"

#if !defined (__ACE_INLINE__)
#include "tao/Principal.inl"
#endif /* __ACE_INLINE__ */

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

CORBA::Principal::Principal (void)
  : refcount_ (1)
{
}

CORBA::Principal::~Principal (void)
{
}

CORBA::Boolean
operator<< (TAO_OutputCDR & cdr, CORBA::Principal * x)
{
  if (x != 0)
    {
      return cdr << x->id;
    }
  else
    {
      cdr.write_ulong (0);
      return (CORBA::Boolean) cdr.good_bit ();
    }
}

CORBA::Boolean
operator>> (TAO_InputCDR & cdr, CORBA::Principal *& x)
{
  CORBA::ULong length;
  cdr.read_ulong (length);

  if (length == 0 || !cdr.good_bit ())
    {
      x = 0;
      return (CORBA::Boolean) cdr.good_bit ();
    }
  else
    {
      ACE_NEW_RETURN (x, CORBA::Principal, 0);
      return cdr >> x->id;
    }
}

TAO_END_VERSIONED_NAMESPACE_DECL
