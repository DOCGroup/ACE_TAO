// -*- C++ -*-
//
// $Id$

template <class RefCountPolicy>
ACE_INLINE
TAO::TypeCode::Sequence<RefCountPolicy>::Sequence (
  CORBA::TCKind kind,
  CORBA::TypeCode_ptr const * content_type,
  CORBA::ULong length)
  : kind_ (kind)
  , content_type_ (content_type)
  , length_ (length)
{
  // ACE_ASSERT (kind == CORBA::tk_array || kind == CORBA::tk_sequence);
}
