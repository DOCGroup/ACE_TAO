// -*- C++ -*-
//
// $Id$


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
#ifdef __BORLANDC__
  // Borland C++ currently can't handle a reference to
  // const pointer to const CORBA::TypeCode_ptr
  TypeCodeType concrete_base,
#else
  TypeCodeType const & concrete_base,
#endif
  FieldArrayType const & fields,
  CORBA::ULong nfields)
  : CORBA::TypeCode (kind)
  , RefCountPolicy ()
  , base_attributes_ (id, name)
  , type_modifier_ (modifier)
  , concrete_base_ (concrete_base)
  , nfields_ (nfields)
  , fields_ (fields)
{
}
