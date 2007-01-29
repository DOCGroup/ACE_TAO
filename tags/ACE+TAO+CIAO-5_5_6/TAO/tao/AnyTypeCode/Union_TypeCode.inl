// -*- C++ -*-
//
// $Id$

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

template <typename StringType,
          typename TypeCodeType,
          class CaseArrayType,
          class RefCountPolicy>
ACE_INLINE
TAO::TypeCode::Union<StringType,
                     TypeCodeType,
                     CaseArrayType,
                     RefCountPolicy>::Union (
  char const * id,
  char const * name,
#if defined (__BORLANDC__) && (__BORLANDC__ < 0x572)
  // Borland C++ currently can't handle a reference to
  // const pointer to const CORBA::TypeCode_ptr
  TypeCodeType discriminant_type,
#else
  TypeCodeType const & discriminant_type,
#endif
  CaseArrayType const & cases,
  CORBA::ULong ncases,
  CORBA::Long default_index)
  : ::CORBA::TypeCode (CORBA::tk_union)
  , RefCountPolicy ()
  , base_attributes_ (id, name)
  , discriminant_type_ (discriminant_type)
  , default_index_ (default_index)
  , ncases_ (ncases)
  , cases_ (cases)
{
}

template <typename StringType,
          typename TypeCodeType,
          class CaseArrayType,
          class RefCountPolicy>
ACE_INLINE
TAO::TypeCode::Union<StringType,
                     TypeCodeType,
                     CaseArrayType,
                     RefCountPolicy>::Union (
  CORBA::TCKind,
  char const * id)
  : ::CORBA::TypeCode (CORBA::tk_union)
  , RefCountPolicy ()
  , base_attributes_ (id)
  , discriminant_type_ (0)
  , default_index_ (-1)
  , ncases_ (0)
  , cases_ ()
{
}

TAO_END_VERSIONED_NAMESPACE_DECL
