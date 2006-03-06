// -*- C++ -*-
//
// $Id$

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

template <class RefCountPolicy>
ACE_INLINE
TAO::TypeCode::String<RefCountPolicy>::String (CORBA::TCKind kind,
                                               CORBA::ULong length)
  : ::CORBA::TypeCode (kind)
  , RefCountPolicy ()
  , length_ (length)
{
  // ACE_ASSERT (kind == CORBA::tk_string || kind == CORBA::tk_wstring);
}

TAO_END_VERSIONED_NAMESPACE_DECL
