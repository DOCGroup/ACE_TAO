// -*- C++ -*-
//
// $Id$

template <class RefCountPolicy>
ACE_INLINE
TAO::TypeCode::String<RefCountPolicy>::String (CORBA::TCKind kind,
                                               CORBA::ULong length)
  : kind_ (kind),
    length_ (length)
{
  // ACE_ASSERT (kind == CORBA::tk_string || kind == CORBA::tk_wstring);
}
