// -*- C++ -*-
//
// $Id$

template <typename TypeCodeType, class RefCountPolicy>
ACE_INLINE
TAO::TypeCode::Sequence<TypeCodeType, RefCountPolicy>::Sequence (
  CORBA::TCKind kind,
  TypeCodeType const & content_type,
  CORBA::ULong length)
  : kind_ (kind)
  , content_type_ (content_type)
  , length_ (length)
{
  // ACE_ASSERT (kind == CORBA::tk_array || kind == CORBA::tk_sequence);
}
