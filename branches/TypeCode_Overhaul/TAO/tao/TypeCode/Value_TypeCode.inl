// -*- C++ -*-
//
// $Id$

#include "TypeCode_Value_Field.h"


template <typename StringType,
          class FieldArrayType,
          CORBA::TCKind Kind,
          class RefCountPolicy>
ACE_INLINE
TAO::TypeCode::Value<StringType, FieldArrayType, Kind, RefCountPolicy>::Value (
  char const * id,
  char const * name,
  CORBA::ValueModifier type_modifier,
  CORBA::TypeCode_ptr * concrete_base_type,
  Field<StringType> const * fields,
  CORBA::ULong nfields)
  : base_attributes_ (id, name)
  , type_modifier_ (type_modifier)
  , concrete_base_ (concrete_base_type)
  , nfields_ (nfields)
  , fields_ (fields)
{
}

template <typename StringType,
          class FieldArrayType,
          CORBA::TCKind Kind,
          class RefCountPolicy>
ACE_INLINE TAO::TypeCode::Field<StringType> const *
TAO::TypeCode::Value<StringType,
                     FieldArrayType,
                     Kind,
                     RefCountPolicy>::fields (void) const
{
  return this->fields_;
}

// -------------------------------------------------------------
// Member specializations
// -------------------------------------------------------------

ACE_INLINE TAO::TypeCode::Field<CORBA::String_var> const *
TAO::TypeCode::Value<CORBA::String_var,
                     ACE_Auto_Ptr_Array<Field<CORBA::String_var> const>,
                     CORBA::tk_value,
                     TAO::True_RefCount_Policy>::fields (void) const
{
  return this->fields_.get ();
}

ACE_INLINE TAO::TypeCode::Field<CORBA::String_var> const *
TAO::TypeCode::Value<CORBA::String_var,
                     ACE_Auto_Ptr_Array<Field<CORBA::String_var> const>,
                     CORBA::tk_event,
                     TAO::True_RefCount_Policy>::fields (void) const
{
  return this->fields_.get ();
}
