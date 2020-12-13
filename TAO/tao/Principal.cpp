// Copyright 1994-1995 by Sun Microsystems Inc.
// All Rights Reserved
// ORB: Principal identifier pseudo-objref

#include "Principal.h"
#include "CDR.h"

#if !defined (__ACE_INLINE__)
#include "Principal.inl"
#endif /* __ACE_INLINE__ */

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

CORBA::Principal::Principal ()
  : refcount_ (1)
{
}

CORBA::Principal::~Principal ()
{
}

CORBA::Boolean
operator<< (TAO_OutputCDR & cdr, CORBA::Principal * x)
{
  if (x != nullptr)
    {
      CORBA::ULong length  = x->id.length ();
      cdr.write_long (length);
      cdr.write_octet_array (x->id.get_buffer (), length);
    }
  else
    {
      cdr.write_ulong (0);
    }

  return (CORBA::Boolean) cdr.good_bit ();
}

CORBA::Boolean
operator>> (TAO_InputCDR & cdr, CORBA::Principal *& x)
{
  CORBA::ULong length;
  cdr.read_ulong (length);

  if (length == 0 || !cdr.good_bit ())
    {
      x = nullptr;
    }
  else
    {
      ACE_NEW_RETURN (x, CORBA::Principal, 0);
      x->id.length (length);
      cdr.read_octet_array (x->id.get_buffer (), length);
    }

  return (CORBA::Boolean) cdr.good_bit ();
}

TAO_END_VERSIONED_NAMESPACE_DECL
