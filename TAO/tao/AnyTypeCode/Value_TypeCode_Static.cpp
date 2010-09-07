// -*- C++ -*-
// $Id$

#include "tao/AnyTypeCode/Value_TypeCode_Static.h"
#include "tao/AnyTypeCode/TypeCode_Value_Field.h"
#include "tao/CDR.h"

#include "tao/ORB_Core.h"
#include "tao/TypeCodeFactory_Adapter.h"
#include "tao/SystemException.h"

#include "ace/Dynamic_Service.h"

#ifndef __ACE_INLINE__
# include "tao/AnyTypeCode/Value_TypeCode_Static.inl"
#endif  /* !__ACE_INLINE__ */

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

bool
TAO::TypeCode::Value<char const *,
                     CORBA::TypeCode_ptr const *,
                     TAO::TypeCode::Value_Field<char const *,
                                                CORBA::TypeCode_ptr const *> const *,
                     TAO::Null_RefCount_Policy>::tao_marshal (
  TAO_OutputCDR & cdr,
  CORBA::ULong offset) const
{
  // A tk_value TypeCode has a "complex" parameter list type (see
  // Table 15-2 in Section 15.3.5.1 "TypeCode" in the CDR section of
  // the CORBA specification), meaning that it must be marshaled into
  // a CDR encapsulation.

  // Create a CDR encapsulation.
  TAO_OutputCDR enc;

  // Account for the encoded CDR encapsulation length and byte order.
  //
  // Aligning on an octet since the next value after the CDR
  // encapsulation length will always be the byte order octet/boolean
  // in this case.
  offset = ACE_align_binary (offset + 4,
                             ACE_CDR::OCTET_ALIGN);

  bool const success =
    (enc << TAO_OutputCDR::from_boolean (TAO_ENCAP_BYTE_ORDER))
    && (enc << TAO_OutputCDR::from_string (this->base_attributes_.id (), 0))
    && (enc << TAO_OutputCDR::from_string (this->base_attributes_.name (), 0))
    && (enc << this->type_modifier_)
    && marshal (enc,
                Traits<char const *>::get_typecode (this->concrete_base_),
                offset + enc.total_length ())
    && (enc << this->nfields_);

  if (!success)
    return false;

  Value_Field<char const *, CORBA::TypeCode_ptr const *> const * const begin =
    &this->fields_[0];
  Value_Field<char const *, CORBA::TypeCode_ptr const *> const * const end =
    begin + this->nfields_;

  for (Value_Field<char const *, CORBA::TypeCode_ptr const *> const * i =
         begin;
       i != end;
       ++i)
    {
      Value_Field<char const *, CORBA::TypeCode_ptr const *> const & field = *i;

      if (!(enc << Traits<char const *>::get_string (field.name))
          || !marshal (enc,
                       Traits<char const *>::get_typecode (field.type),
                       offset + enc.total_length ())
          || !(enc << field.visibility))
        return false;
    }

  return
    cdr << static_cast<CORBA::ULong> (enc.total_length ())
    && cdr.write_octet_array_mb (enc.begin ());
}

void
TAO::TypeCode::Value<char const *,
                     CORBA::TypeCode_ptr const *,
                     TAO::TypeCode::Value_Field<char const *,
                                                CORBA::TypeCode_ptr const *> const *,
                     TAO::Null_RefCount_Policy>::tao_duplicate (void)
{
}

void
TAO::TypeCode::Value<char const *,
                     CORBA::TypeCode_ptr const *,
                     TAO::TypeCode::Value_Field<char const *,
                                                CORBA::TypeCode_ptr const *> const *,
                     TAO::Null_RefCount_Policy>::tao_release (void)
{
}

CORBA::Boolean
TAO::TypeCode::Value<char const *,
                     CORBA::TypeCode_ptr const *,
                     TAO::TypeCode::Value_Field<char const *,
                                                CORBA::TypeCode_ptr const *> const *,
                     TAO::Null_RefCount_Policy>::equal_i (
  CORBA::TypeCode_ptr tc) const
{
  // None of these calls should throw since CORBA::TypeCode::equal()
  // verified that the TCKind is the same as our's prior to invoking
  // this method.

  CORBA::ValueModifier const tc_type_modifier =
    tc->type_modifier ();

  if (tc_type_modifier != this->type_modifier_)
    return false;

  CORBA::TypeCode_var rhs_concrete_base_type = tc->concrete_base_type ();

  CORBA::Boolean const equal_concrete_base_types =
    this->equal (rhs_concrete_base_type.in ());

  if (!equal_concrete_base_types)
    return false;

  CORBA::ULong const tc_nfields = tc->member_count ();

  if (tc_nfields != this->nfields_)
    return false;

  for (CORBA::ULong i = 0; i < this->nfields_; ++i)
    {
      Value_Field<char const *, CORBA::TypeCode_ptr const *> const & lhs_field =
        this->fields_[i];

      CORBA::Visibility const lhs_visibility = lhs_field.visibility;
      CORBA::Visibility const rhs_visibility = tc->member_visibility (i);

      if (lhs_visibility != rhs_visibility)
        return false;

      char const * const lhs_name =
        Traits<char const *>::get_string (lhs_field.name);;
      char const * const rhs_name = tc->member_name (i);

      if (ACE_OS::strcmp (lhs_name, rhs_name) != 0)
        return false;

      CORBA::TypeCode_ptr const lhs_tc =
        Traits<char const *>::get_typecode (lhs_field.type);
      CORBA::TypeCode_var const rhs_tc =
        tc->member_type (i);

      CORBA::Boolean const equal_members =
        lhs_tc->equal (rhs_tc.in ());

      if (!equal_members)
        return false;
    }

  return true;
}

CORBA::Boolean
TAO::TypeCode::Value<char const *,
                     CORBA::TypeCode_ptr const *,
                     TAO::TypeCode::Value_Field<char const *,
                                                CORBA::TypeCode_ptr const *> const *,
                     TAO::Null_RefCount_Policy>::equivalent_i (
  CORBA::TypeCode_ptr tc) const
{
  CORBA::ValueModifier const tc_type_modifier = tc->type_modifier ();

  if (tc_type_modifier != this->type_modifier_)
    return false;

  CORBA::TypeCode_var rhs_concrete_base_type = tc->concrete_base_type ();

  CORBA::Boolean const equivalent_concrete_base_types =
    this->equivalent (rhs_concrete_base_type.in ());

  if (!equivalent_concrete_base_types)
    return false;

  // Perform a structural comparison, excluding the name() and
  // member_name() operations.

  CORBA::ULong const tc_nfields = tc->member_count ();

  if (tc_nfields != this->nfields_)
    return false;

  for (CORBA::ULong i = 0; i < this->nfields_; ++i)
    {
      Value_Field<char const *,
                  CORBA::TypeCode_ptr const *> const & lhs_field =
        this->fields_[i];

      CORBA::Visibility const lhs_visibility = lhs_field.visibility;
      CORBA::Visibility const rhs_visibility = tc->member_visibility (i);

      if (lhs_visibility != rhs_visibility)
        return false;

      CORBA::TypeCode_ptr const lhs_tc =
        Traits<char const *>::get_typecode (lhs_field.type);
      CORBA::TypeCode_var const rhs_tc = tc->member_type (i);

      CORBA::Boolean const equiv_types = lhs_tc->equivalent (rhs_tc.in ());

      if (!equiv_types)
        return false;
    }

  return true;
}

CORBA::TypeCode_ptr
TAO::TypeCode::Value<char const *,
                     CORBA::TypeCode_ptr const *,
                     TAO::TypeCode::Value_Field<char const *,
                                                CORBA::TypeCode_ptr const *> const *,
                     TAO::Null_RefCount_Policy>::get_compact_typecode_i (
  void) const
{
  ACE_Array_Base<Value_Field<CORBA::String_var, CORBA::TypeCode_var> >
    tc_fields (this->nfields_);

  if (this->nfields_ > 0)
    {
      // Dynamically construct a new array of fields stripped of
      // member names.

      static char const empty_name[] = "";

      for (CORBA::ULong i = 0; i < this->nfields_; ++i)
        {
          // Member names will be stripped, i.e. not embedded within
          // the compact TypeCode.

          tc_fields[i].name = empty_name;
          tc_fields[i].type =
            Traits<char const *>::get_typecode (
              this->fields_[i].type)->get_compact_typecode (
                );
          tc_fields[i].visibility = this->fields_[i].visibility;
        }
    }

  TAO_TypeCodeFactory_Adapter * const adapter =
    ACE_Dynamic_Service<TAO_TypeCodeFactory_Adapter>::instance (
      TAO_ORB_Core::typecodefactory_adapter_name ());

  if (adapter == 0)
    {
      throw ::CORBA::INTERNAL ();
    }

  return
    adapter->create_value_event_tc (
      this->kind_,
      this->base_attributes_.id (),
      "", // empty name
      this->type_modifier_,
      Traits<char const *>::get_typecode (this->concrete_base_),
      tc_fields,
      this->nfields_
     );
}

char const *
TAO::TypeCode::Value<char const *,
                     CORBA::TypeCode_ptr const *,
                     TAO::TypeCode::Value_Field<char const *,
                                                CORBA::TypeCode_ptr const *> const *,
                     TAO::Null_RefCount_Policy>::id_i (void) const
{
  // Ownership is retained by the TypeCode, as required by the C++
  // mapping.
  return this->base_attributes_.id ();
}

char const *
TAO::TypeCode::Value<char const *,
                     CORBA::TypeCode_ptr const *,
                     TAO::TypeCode::Value_Field<char const *,
                                                CORBA::TypeCode_ptr const *> const *,
                     TAO::Null_RefCount_Policy>::name_i (void) const
{
  // Ownership is retained by the TypeCode, as required by the C++
  // mapping.
  return this->base_attributes_.name ();
}

CORBA::ULong
TAO::TypeCode::Value<char const *,
                     CORBA::TypeCode_ptr const *,
                     TAO::TypeCode::Value_Field<char const *,
                                                CORBA::TypeCode_ptr const *> const *,
                     TAO::Null_RefCount_Policy>::member_count_i (void) const
{
  return this->nfields_;
}

char const *
TAO::TypeCode::Value<char const *,
                     CORBA::TypeCode_ptr const *,
                     TAO::TypeCode::Value_Field<char const *,
                                                CORBA::TypeCode_ptr const *> const *,
                     TAO::Null_RefCount_Policy>::member_name_i (
  CORBA::ULong index) const
{
  // Ownership is retained by the TypeCode, as required by the C++
  // mapping.
  if (index >= this->nfields_)
    throw ::CORBA::TypeCode::Bounds ();

  return Traits<char const *>::get_string (this->fields_[index].name);
}

CORBA::TypeCode_ptr
TAO::TypeCode::Value<char const *,
                     CORBA::TypeCode_ptr const *,
                     TAO::TypeCode::Value_Field<char const *,
                                                CORBA::TypeCode_ptr const *> const *,
                     TAO::Null_RefCount_Policy>::member_type_i (
  CORBA::ULong index) const
{
  if (index >= this->nfields_)
    throw ::CORBA::TypeCode::Bounds ();

  return
    CORBA::TypeCode::_duplicate (
      Traits<char const *>::get_typecode (this->fields_[index].type));
}

CORBA::Visibility
TAO::TypeCode::Value<char const *,
                     CORBA::TypeCode_ptr const *,
                     TAO::TypeCode::Value_Field<char const *,
                                                CORBA::TypeCode_ptr const *> const *,
                     TAO::Null_RefCount_Policy>::member_visibility_i (
  CORBA::ULong index) const
{
  if (index >= this->nfields_)
    throw ::CORBA::TypeCode::Bounds ();

  return this->fields_[index].visibility;
}

CORBA::ValueModifier
TAO::TypeCode::Value<char const *,
                     CORBA::TypeCode_ptr const *,
                     TAO::TypeCode::Value_Field<char const *,
                                                CORBA::TypeCode_ptr const *> const *,
                     TAO::Null_RefCount_Policy>::type_modifier_i (void) const
{
  return this->type_modifier_;
}

CORBA::TypeCode_ptr
TAO::TypeCode::Value<char const *,
                     CORBA::TypeCode_ptr const *,
                     TAO::TypeCode::Value_Field<char const *,
                                                CORBA::TypeCode_ptr const *> const *,
                     TAO::Null_RefCount_Policy>::concrete_base_type_i (
  void) const
{
  return
    CORBA::TypeCode::_duplicate (
      Traits<char const *>::get_typecode (this->concrete_base_));
}

TAO_END_VERSIONED_NAMESPACE_DECL
