// -*- C++ -*-
//
// $Id$

#include "tao/TypeCode_Struct_Field.h"
#include "tao/True_RefCount_Policy.h"

#include "ace/Auto_Ptr.h"


template <typename StringType,
          class FieldArrayType,
          CORBA::TCKind Kind,
          class RefCountPolicy>
ACE_INLINE
TAO::TypeCode::Struct<StringType,
                      FieldArrayType,
                      Kind,
                      RefCountPolicy>::Struct (
  char const * id,
  char const * name,
  Struct_Field<StringType> const * fields,
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
ACE_INLINE TAO::TypeCode::Struct_Field<StringType> const *
TAO::TypeCode::Struct<StringType,
                      FieldArrayType,
                      Kind,
                      RefCountPolicy>::fields (void) const
{
  return this->fields_;
}

// -------------------------------------------------------------
// Member specializations
// -------------------------------------------------------------

ACE_INLINE TAO::TypeCode::Struct_Field<CORBA::String_var> const *
TAO::TypeCode::Struct<
  CORBA::String_var,
  ACE_Auto_Array_Ptr<TAO::TypeCode::Struct_Field<CORBA::String_var> const>,
  CORBA::tk_struct,
  TAO::True_RefCount_Policy>::fields (void) const
{
  return this->fields_.get ();
}

ACE_INLINE TAO::TypeCode::Struct_Field<CORBA::String_var> const *
TAO::TypeCode::Struct<
  CORBA::String_var,
  ACE_Auto_Array_Ptr<TAO::TypeCode::Struct_Field<CORBA::String_var> const>,
  CORBA::tk_except,
  TAO::True_RefCount_Policy>::fields (void) const
{
  return this->fields_.get ();
}
