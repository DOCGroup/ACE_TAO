// $Id$

#ifndef TAO_STRUCT_TYPECODE_CPP
#define TAO_STRUCT_TYPECODE_CPP

#include "tao/AnyTypeCode/Struct_TypeCode.h"
#include "tao/AnyTypeCode/TypeCode_Struct_Field.h"
#include "tao/AnyTypeCode/TypeCode_Traits.h"
#include "tao/ORB_Core.h"
#include "tao/TypeCodeFactory_Adapter.h"
#include "tao/CDR.h"
#include "tao/SystemException.h"


#ifndef __ACE_INLINE__
# include "tao/AnyTypeCode/Struct_TypeCode.inl"
#endif  /* !__ACE_INLINE__ */

#include "ace/Dynamic_Service.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

template <typename StringType,
          typename TypeCodeType,
          class FieldArrayType,
          class RefCountPolicy>
bool
TAO::TypeCode::Struct<StringType,
                      TypeCodeType,
                      FieldArrayType,
                      RefCountPolicy>::tao_marshal (TAO_OutputCDR & cdr,
                                                    CORBA::ULong offset) const
{
  // A tk_struct TypeCode has a "complex" parameter list type (see
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
    && (enc << this->nfields_);

  if (!success)
    return false;

  Struct_Field<StringType, TypeCodeType> const * const begin =
    &this->fields_[0];
  Struct_Field<StringType, TypeCodeType> const * const end =
    begin + this->nfields_;

  for (Struct_Field<StringType, TypeCodeType> const * i = begin; i != end; ++i)
    {
      Struct_Field<StringType, TypeCodeType> const & field = *i;

      if (!(enc << TAO_OutputCDR::from_string (
                       Traits<StringType>::get_string (field.name), 0))
          || !marshal (enc,
                       Traits<StringType>::get_typecode (field.type),
                       offset + enc.total_length ()))
        return false;
    }

  return
    cdr << static_cast<CORBA::ULong> (enc.total_length ())
    && cdr.write_octet_array_mb (enc.begin ());
}

template <typename StringType,
          typename TypeCodeType,
          class FieldArrayType,
          class RefCountPolicy>
void
TAO::TypeCode::Struct<StringType,
                      TypeCodeType,
                      FieldArrayType,
                      RefCountPolicy>::tao_duplicate (void)
{
  this->RefCountPolicy::add_ref ();
}

template <typename StringType,
          typename TypeCodeType,
          class FieldArrayType,
          class RefCountPolicy>
void
TAO::TypeCode::Struct<StringType,
                      TypeCodeType,
                      FieldArrayType,
                      RefCountPolicy>::tao_release (void)
{
  this->RefCountPolicy::remove_ref ();
}

template <typename StringType,
          typename TypeCodeType,
          class FieldArrayType,
          class RefCountPolicy>
const FieldArrayType&
TAO::TypeCode::Struct<StringType,
                      TypeCodeType,
                      FieldArrayType,
                      RefCountPolicy>::fields (void) const
{
  return fields_;
}

template <typename StringType,
          typename TypeCodeType,
          class FieldArrayType,
          class RefCountPolicy>
CORBA::Boolean
TAO::TypeCode::Struct<StringType,
                      TypeCodeType,
                      FieldArrayType,
                      RefCountPolicy>::equal_i (CORBA::TypeCode_ptr tc) const
{
  // This call shouldn't throw since CORBA::TypeCode::equal() verified
  // that the TCKind is the same as our's prior to invoking this
  // method, meaning that member_count() is supported.

  CORBA::ULong const tc_nfields =
    tc->member_count ();

  if (tc_nfields != this->nfields_)
    return false;

  for (CORBA::ULong i = 0; i < this->nfields_; ++i)
    {
      Struct_Field<StringType, TypeCodeType> const & lhs_field =
        this->fields_[i];

      char const * const lhs_name =
        Traits<StringType>::get_string (lhs_field.name);
      char const * const rhs_name = tc->member_name (i);

      if (ACE_OS::strcmp (lhs_name, rhs_name) != 0)
        return false;

      CORBA::TypeCode_ptr const lhs_tc =
        Traits<StringType>::get_typecode (lhs_field.type);
      CORBA::TypeCode_var const rhs_tc =
        tc->member_type (i);

      CORBA::Boolean const equal_members =
        lhs_tc->equal (rhs_tc.in ());

      if (!equal_members)
        return false;
    }

  return true;
}

template <typename StringType,
          typename TypeCodeType,
          class FieldArrayType,
          class RefCountPolicy>
CORBA::Boolean
TAO::TypeCode::Struct<StringType,
                      TypeCodeType,
                      FieldArrayType,
                      RefCountPolicy>::equivalent_i (
  CORBA::TypeCode_ptr tc) const
{
  // Perform a structural comparison, excluding the name() and
  // member_name() operations.

  CORBA::ULong const tc_nfields =
    tc->member_count ();

  if (tc_nfields != this->nfields_)
    return false;

  for (CORBA::ULong i = 0; i < this->nfields_; ++i)
    {
      CORBA::TypeCode_ptr const lhs =
        Traits<StringType>::get_typecode (this->fields_[i].type);
      CORBA::TypeCode_var const rhs =
        tc->member_type (i);

      CORBA::Boolean const equiv_members =
        lhs->equivalent (rhs.in ());

      if (!equiv_members)
        return false;
    }

  return true;
}

template <typename StringType,
          typename TypeCodeType,
          class FieldArrayType,
          class RefCountPolicy>
CORBA::TypeCode_ptr
TAO::TypeCode::Struct<StringType,
                      TypeCodeType,
                      FieldArrayType,
                      RefCountPolicy>::get_compact_typecode_i (void) const
{
  ACE_Array_Base<Struct_Field<CORBA::String_var,
                              CORBA::TypeCode_var> >
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
            Traits<StringType>::get_typecode (
              this->fields_[i].type)->get_compact_typecode ();
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
    adapter->create_struct_except_tc (this->kind_,
                                      this->base_attributes_.id (),
                                      ""  /* empty name */,
                                      tc_fields,
                                      this->nfields_);
}

template <typename StringType,
          typename TypeCodeType,
          class FieldArrayType,
          class RefCountPolicy>
char const *
TAO::TypeCode::Struct<StringType,
                      TypeCodeType,
                      FieldArrayType,
                      RefCountPolicy>::id_i (void) const
{
  // Ownership is retained by the TypeCode, as required by the C++
  // mapping.
  return this->base_attributes_.id ();
}

template <typename StringType,
          typename TypeCodeType,
          class FieldArrayType,
          class RefCountPolicy>
char const *
TAO::TypeCode::Struct<StringType,
                      TypeCodeType,
                      FieldArrayType,
                      RefCountPolicy>::name_i (void) const
{
  // Ownership is retained by the TypeCode, as required by the C++
  // mapping.
  return this->base_attributes_.name ();
}

template <typename StringType,
          typename TypeCodeType,
          class FieldArrayType,
          class RefCountPolicy>
CORBA::ULong
TAO::TypeCode::Struct<StringType,
                      TypeCodeType,
                      FieldArrayType,
                      RefCountPolicy>::member_count_i (void) const
{
  return this->nfields_;
}

template <typename StringType,
          typename TypeCodeType,
          class FieldArrayType,
          class RefCountPolicy>
char const *
TAO::TypeCode::Struct<StringType,
                      TypeCodeType,
                      FieldArrayType,
                      RefCountPolicy>::member_name_i (
  CORBA::ULong index) const
{
  // Ownership is retained by the TypeCode, as required by the C++
  // mapping.
  if (index >= this->nfields_)
    throw ::CORBA::TypeCode::Bounds ();

  return Traits<StringType>::get_string (this->fields_[index].name);
}

template <typename StringType,
          typename TypeCodeType,
          class FieldArrayType,
          class RefCountPolicy>
CORBA::TypeCode_ptr
TAO::TypeCode::Struct<StringType,
                      TypeCodeType,
                      FieldArrayType,
                      RefCountPolicy>::member_type_i (
  CORBA::ULong index) const
{
  if (index >= this->nfields_)
    throw ::CORBA::TypeCode::Bounds ();

  return
    CORBA::TypeCode::_duplicate (
      Traits<StringType>::get_typecode (this->fields_[index].type));
}

TAO_END_VERSIONED_NAMESPACE_DECL

#endif  /* TAO_STRUCT_TYPECODE_CPP */
