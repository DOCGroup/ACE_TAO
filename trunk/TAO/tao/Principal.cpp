// $Id$

// Copyright 1994-1995 by Sun Microsystems Inc.
// All Rights Reserved
// ORB:		Principal identifier pseudo-objref

#include "tao/Principal.h"
#include "tao/CDR.h"

#if !defined (__ACE_INLINE__)
#include "tao/Principal.i"
#endif /* __ACE_INLINE__ */

ACE_RCSID(tao, Principal, "$Id$")

CORBA_Principal::CORBA_Principal (void)
{
}

CORBA_Principal::~CORBA_Principal (void)
{
}

void
CORBA::release (CORBA::Principal_ptr principal)
{
  if (principal)
    principal->_decr_refcnt ();
}

CORBA::Boolean
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
  return cdr.good_bit ();
}

CORBA::Boolean
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
      ACE_NEW_RETURN (x, CORBA::Principal, 0);
      x->id.length (length);
      cdr.read_octet_array (x->id.get_buffer (), length);
    }
  return cdr.good_bit ();
}
