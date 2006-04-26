// -*- C++ -*-
//
// $Id$

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

template<typename StringType,
         typename TypeCodeType,
         class RefCountPolicy>
ACE_INLINE
TAO::TypeCode::Alias<StringType,
                     TypeCodeType,
                     RefCountPolicy>::Alias (
  CORBA::TCKind kind,
  char const * id,
  char const * name,
#if defined (__BORLANDC__) && (__BORLANDC__ < 0x572)
  // Borland C++ currently can't handle a reference to
  // const pointer to const CORBA::TypeCode_ptr
  TypeCodeType tc)
#else
  TypeCodeType const & tc)
#endif
  : ::CORBA::TypeCode (kind)
  , RefCountPolicy ()
  , attributes_ (id, name)
  , content_type_ (tc)
{
}

TAO_END_VERSIONED_NAMESPACE_DECL
