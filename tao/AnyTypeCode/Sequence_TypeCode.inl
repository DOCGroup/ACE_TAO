// -*- C++ -*-
//
// $Id$

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

template <typename TypeCodeType, class RefCountPolicy>
ACE_INLINE
TAO::TypeCode::Sequence<TypeCodeType, RefCountPolicy>::Sequence (
  CORBA::TCKind kind,
#if defined (__BORLANDC__) && (__BORLANDC__ < 0x572)
  // Borland C++ currently can't handle a reference to
  // const pointer to const CORBA::TypeCode_ptr
  TypeCodeType content_type,
#else
  TypeCodeType const & content_type,
#endif
  CORBA::ULong length)
  : ::CORBA::TypeCode (kind)
  , RefCountPolicy ()
  , content_type_ (content_type)
  , length_ (length)
{
  // ACE_ASSERT (kind == CORBA::tk_array || kind == CORBA::tk_sequence);
}

TAO_END_VERSIONED_NAMESPACE_DECL
