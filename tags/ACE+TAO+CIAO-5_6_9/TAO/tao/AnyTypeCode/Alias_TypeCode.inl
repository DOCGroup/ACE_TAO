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
  TypeCodeType const & tc)
  : ::CORBA::TypeCode (kind)
  , RefCountPolicy ()
  , attributes_ (id, name)
  , content_type_ (tc)
{
}

TAO_END_VERSIONED_NAMESPACE_DECL
