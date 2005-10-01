// -*- C++ -*-
//
// $Id$


ACE_INLINE
TAO::TypeCode::Struct<char const *,
                      CORBA::TypeCode_ptr const *,
                      TAO::TypeCode::Struct_Field<char const *,
                                                  CORBA::TypeCode_ptr const *> const *,
                      TAO::Null_RefCount_Policy>::Struct (
  CORBA::TCKind kind,
  char const * id,
  char const * name,
  Struct_Field<char const *, CORBA::TypeCode_ptr const *> const * fields,
  CORBA::ULong nfields)
  : CORBA::TypeCode (kind)
  , ACE_NESTED_CLASS (TAO, Null_RefCount_Policy) ()
  , base_attributes_ (id, name)
  , nfields_ (nfields)
  , fields_ (fields)
{
}
