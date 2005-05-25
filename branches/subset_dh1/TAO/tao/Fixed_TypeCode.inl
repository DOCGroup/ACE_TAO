// -*- C++ -*-
//
// $Id$

template <class RefCountPolicy>
ACE_INLINE
TAO::TypeCode::Fixed<RefCountPolicy>::Fixed (CORBA::UShort digits,
                                             CORBA::UShort scale)
  : CORBA::TypeCode (CORBA::tk_fixed)
  , RefCountPolicy ()
  , digits_ (digits)
  , scale_ (scale)
{
}
