// $Id$

// Copyright 1994-1995 by Sun Microsystems Inc.
// All Rights Reserved
// ORB:		Principal identifier pseudo-objref

#include "tao/Principal.h"
#include "tao/CDR.h"

#if !defined (__ACE_INLINE__)
#include "tao/Principal.i"
#endif /* __ACE_INLINE__ */


ACE_RCSID (tao,
           Principal,
           "$Id$")


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
      x = 0;
    }
  else
    {
      ACE_NEW_RETURN (x, CORBA::Principal, 0);
      x->id.length (length);
      cdr.read_octet_array (x->id.get_buffer (), length);
    }

  return (CORBA::Boolean) cdr.good_bit ();
}

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)

  template class TAO_Pseudo_Var_T<CORBA::Principal>;
  template class TAO_Pseudo_Out_T<CORBA::Principal, CORBA::Principal_var>;

#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)

# pragma instantiate TAO_Pseudo_Var_T<CORBA::Principal>
# pragma instantiate TAO_Pseudo_Out_T<CORBA::Principal, CORBA::Principal_var>

#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */

