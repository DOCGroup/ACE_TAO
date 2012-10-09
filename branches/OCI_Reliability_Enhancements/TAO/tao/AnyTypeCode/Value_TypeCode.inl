// -*- C++ -*-
//
// $Id$

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

template <typename StringType,
          typename TypeCodeType,
          class FieldArrayType,
          class RefCountPolicy>
ACE_INLINE
TAO::TypeCode::Value<StringType,
                     TypeCodeType,
                     FieldArrayType,
                     RefCountPolicy>::Value (
  CORBA::TCKind kind,
  char const * id,
  char const * name,
  CORBA::ValueModifier modifier,
  TypeCodeType const & concrete_base,
  FieldArrayType const & fields,
  CORBA::ULong nfields)
  : ::CORBA::TypeCode (kind)
  , RefCountPolicy ()
  , base_attributes_ (id, name)
  , type_modifier_ (modifier)
  , concrete_base_ (concrete_base)
  , nfields_ (nfields)
  , fields_ (fields)
{
}

template <typename StringType,
          typename TypeCodeType,
          class FieldArrayType,
          class RefCountPolicy>
ACE_INLINE
TAO::TypeCode::Value<StringType,
                     TypeCodeType,
                     FieldArrayType,
                     RefCountPolicy>::Value (
  CORBA::TCKind kind,
  char const * id)
  : ::CORBA::TypeCode (kind)
  , RefCountPolicy ()
  , base_attributes_ (id)
  , type_modifier_ (CORBA::VM_NONE)
  , concrete_base_ ()
  , nfields_ (0)
  , fields_ ()
{
}

TAO_END_VERSIONED_NAMESPACE_DECL
