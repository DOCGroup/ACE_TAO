// -*- C++ -*-
//
// $Id$

template <typename StringType,
          class FieldArrayType,
          CORBA::TCKind Kind,
          class RefCountPolicy>
ACE_INLINE
TAO::TypeCode::Struct<StringType, FieldArrayType, RefCountPolicy>::Struct (
  char const * id,
  char const * name,
  Field<StringType> const * fields,
  CORBA::ULong nfields)
  : base_attributes_ (id, name)
  , nfields_ (nfields)
  , fields_ (fields)
{
}

template <typename StringType,
          class FieldArrayType,
          CORBA::TCKind Kind,
          class RefCountPolicy>
ACE_INLINE TAO::TypeCode::Field<StringType> const *
TAO::TypeCode::Struct<StringType,
                      FieldArrayType>::fields (void) const
{
  return this->fields_;
}

// -------------------------------------------------------------
// Member specializations
// -------------------------------------------------------------

ACE_INLINE TAO::TypeCode::Field<CORBA::String_var> const *
TAO::TypeCode::Struct<CORBA::String_var,
                      ACE_Auto_Ptr_Array<Field<CORBA::String_var> const> >::fields (void) const
{
  return this->fields_.get ();
}
