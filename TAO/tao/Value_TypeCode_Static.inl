// -*- C++ -*-
//
// $Id$

ACE_INLINE
TAO::TypeCode::Value<char const *,
                     CORBA::TypeCode_ptr const *,
                     TAO::TypeCode::Value_Field<char const *,
                                                CORBA::TypeCode_ptr const *> const *,
                     TAO::Null_RefCount_Policy>::Value (
  CORBA::TCKind kind,
  char const * id,
  char const * name,
  CORBA::ValueModifier modifier,
  CORBA::TypeCode_ptr const * concrete_base,
  Value_Field<char const *, CORBA::TypeCode_ptr const *> const * fields,
  CORBA::ULong nfields)
  : CORBA::TypeCode (kind)
  , ACE_NESTED_CLASS (TAO, Null_RefCount_Policy) ()
  , base_attributes_ (id, name)
  , type_modifier_ (modifier)
  , concrete_base_ (concrete_base)
  , nfields_ (nfields)
  , fields_ (fields)
{
}
