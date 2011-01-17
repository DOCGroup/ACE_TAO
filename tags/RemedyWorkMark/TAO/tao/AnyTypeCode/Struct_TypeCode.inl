// -*- C++ -*-
//
// $Id$

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

template <typename StringType,
          typename TypeCodeType,
          class FieldArrayType,
          class RefCountPolicy>
ACE_INLINE
TAO::TypeCode::Struct<StringType,
                      TypeCodeType,
                      FieldArrayType,
                      RefCountPolicy>::Struct (
  CORBA::TCKind kind,
  char const * id,
  char const * name,
  FieldArrayType const & fields,
  CORBA::ULong nfields)
  : ::CORBA::TypeCode (kind)
  , RefCountPolicy ()
  , base_attributes_ (id, name)
  , nfields_ (nfields)
  , fields_ (fields)
{
}

template <typename StringType,
          typename TypeCodeType,
          class FieldArrayType,
          class RefCountPolicy>
ACE_INLINE
TAO::TypeCode::Struct<StringType,
                      TypeCodeType,
                      FieldArrayType,
                      RefCountPolicy>::Struct (
  CORBA::TCKind kind,
  char const * id)
  : ::CORBA::TypeCode (kind)
  , RefCountPolicy ()
  , base_attributes_ (id)
  , nfields_ (0)
  , fields_ ()
{
  // CORBA::tk_except is not allowed in the recursive TypeCode case.
  // ACE_ASSERT (kind == CORBA::tk_struct);
}

TAO_END_VERSIONED_NAMESPACE_DECL
