// -*- C++ -*-
//
// $Id$

#include "tao/TypeCode_Value_Field.h"
#include "tao/True_RefCount_Policy.h"

#include "ace/Auto_Ptr.h"


template <typename StringType,
          class FieldArrayType,
          CORBA::TCKind Kind,
          class RefCountPolicy>
ACE_INLINE
TAO::TypeCode::Value<StringType, FieldArrayType, Kind, RefCountPolicy>::Value (
  char const * id,
  char const * name,
  CORBA::ValueModifier modifier,
  CORBA::TypeCode_ptr * concrete_base,
  Value_Field<StringType> const * fields,
  CORBA::ULong nfields)
  : base_attributes_ (id, name)
  , type_modifier_ (modifier)
  , concrete_base_ (concrete_base)
  , nfields_ (nfields)
  , fields_ (fields)
{
}

template <typename StringType,
          class FieldArrayType,
          CORBA::TCKind Kind,
          class RefCountPolicy>
ACE_INLINE TAO::TypeCode::Value_Field<StringType> const *
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

ACE_INLINE TAO::TypeCode::Value_Field<CORBA::String_var> const *
TAO::TypeCode::Value<
  CORBA::String_var,
  ACE_Auto_Array_Ptr<TAO::TypeCode::Value_Field<CORBA::String_var> const>,
  CORBA::tk_value,
  TAO::True_RefCount_Policy>::fields (void) const
{
  return this->fields_.get ();
}

ACE_INLINE TAO::TypeCode::Value_Field<CORBA::String_var> const *
TAO::TypeCode::Value<
  CORBA::String_var,
  ACE_Auto_Array_Ptr<TAO::TypeCode::Value_Field<CORBA::String_var> const>,
  CORBA::tk_event,
  TAO::True_RefCount_Policy>::fields (void) const
{
  return this->fields_.get ();
}
