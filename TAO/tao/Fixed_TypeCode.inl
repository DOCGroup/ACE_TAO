// -*- C++ -*-
//
// $Id$

template <class RefCountPolicy>
ACE_INLINE
TAO::TypeCode::Fixed<RefCountPolicy::Fixed (CORBA::UShort digits,
                                            CORBA::UShort scale)
  : digits_ (digits),
    scale_ (scale)
{
}
