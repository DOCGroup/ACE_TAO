// -*- C++ -*-
// $Id$

#include "tao/AnyTypeCode/Struct_TypeCode_Static.h"
#include "tao/AnyTypeCode/TypeCode_Struct_Field.h"
#include "tao/AnyTypeCode/TypeCode_Traits.h"
#include "tao/ORB_Core.h"
#include "tao/TypeCodeFactory_Adapter.h"
#include "tao/CDR.h"
#include "tao/SystemException.h"

#ifndef __ACE_INLINE__
# include "tao/AnyTypeCode/Struct_TypeCode_Static.inl"
#endif  /* !__ACE_INLINE__ */

#include "ace/Dynamic_Service.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

bool
TAO::TypeCode::Struct<char const *,
                      CORBA::TypeCode_ptr const *,
                      TAO::TypeCode::Struct_Field<char const *,
                                                  CORBA::TypeCode_ptr const *> const *,
                      TAO::Null_RefCount_Policy>::tao_marshal (
  TAO_OutputCDR & cdr,
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

  Struct_Field<char const *, CORBA::TypeCode_ptr const *> const * const begin =
    &this->fields_[0];
  Struct_Field<char const *, CORBA::TypeCode_ptr const *> const * const end =
    begin + this->nfields_;

  for (Struct_Field<char const *, CORBA::TypeCode_ptr const *> const * i =
         begin;
       i != end;
       ++i)
    {
      Struct_Field<char const *, CORBA::TypeCode_ptr const *> const & field =
        *i;

      if (!(enc << TAO_OutputCDR::from_string (
                       Traits<char const *>::get_string (field.name), 0))
          || !marshal (enc,
                       Traits<char const *>::get_typecode (field.type),
                       offset + enc.total_length ()))
        return false;
    }

  return
    cdr << static_cast<CORBA::ULong> (enc.total_length ())
    && cdr.write_octet_array_mb (enc.begin ());
}

void
TAO::TypeCode::Struct<char const *,
                      CORBA::TypeCode_ptr const *,
                      TAO::TypeCode::Struct_Field<char const *,
                                                  CORBA::TypeCode_ptr const *> const *,
                      TAO::Null_RefCount_Policy>::tao_duplicate (void)
{
}

void
TAO::TypeCode::Struct<char const *,
                      CORBA::TypeCode_ptr const *,
                      TAO::TypeCode::Struct_Field<char const *,
                                                  CORBA::TypeCode_ptr const *> const *,
                      TAO::Null_RefCount_Policy>::tao_release (void)
{
}

CORBA::Boolean
TAO::TypeCode::Struct<char const *,
                      CORBA::TypeCode_ptr const *,
                      TAO::TypeCode::Struct_Field<char const *,
                                                  CORBA::TypeCode_ptr const *> const *,
                      TAO::Null_RefCount_Policy>::equal_i (
  CORBA::TypeCode_ptr tc) const
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
      Struct_Field<char const *, CORBA::TypeCode_ptr const *> const &
        lhs_field = this->fields_[i];

      char const * const lhs_name =
        Traits<char const *>::get_string (lhs_field.name);
      char const * const rhs_name = tc->member_name (i
                                                    );

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
TAO::TypeCode::Struct<char const *,
                      CORBA::TypeCode_ptr const *,
                      TAO::TypeCode::Struct_Field<char const *,
                                                  CORBA::TypeCode_ptr const *> const *,
                      TAO::Null_RefCount_Policy>::equivalent_i (
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
        Traits<char const *>::get_typecode (this->fields_[i].type);
      CORBA::TypeCode_var const rhs =
        tc->member_type (i);

      CORBA::Boolean const equiv_members =
        lhs->equivalent (rhs.in ());

      if (!equiv_members)
        return false;
    }

  return true;
}

CORBA::TypeCode_ptr
TAO::TypeCode::Struct<char const *,
                      CORBA::TypeCode_ptr const *,
                      TAO::TypeCode::Struct_Field<char const *,
                                                  CORBA::TypeCode_ptr const *> const *,
                      TAO::Null_RefCount_Policy>::get_compact_typecode_i (
  void) const
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
            Traits<char const *>::get_typecode (
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

char const *
TAO::TypeCode::Struct<char const *,
                      CORBA::TypeCode_ptr const *,
                      TAO::TypeCode::Struct_Field<char const *,
                                                  CORBA::TypeCode_ptr const *> const *,
                      TAO::Null_RefCount_Policy>::id_i (
  void) const
{
  // Ownership is retained by the TypeCode, as required by the C++
  // mapping.
  return this->base_attributes_.id ();
}

char const *
TAO::TypeCode::Struct<char const *,
                      CORBA::TypeCode_ptr const *,
                      TAO::TypeCode::Struct_Field<char const *,
                                                  CORBA::TypeCode_ptr const *> const *,
                      TAO::Null_RefCount_Policy>::name_i (void) const
{
  // Ownership is retained by the TypeCode, as required by the C++
  // mapping.
  return this->base_attributes_.name ();
}

CORBA::ULong
TAO::TypeCode::Struct<char const *,
                      CORBA::TypeCode_ptr const *,
                      TAO::TypeCode::Struct_Field<char const *,
                                                  CORBA::TypeCode_ptr const *> const *,
                      TAO::Null_RefCount_Policy>::member_count_i (void) const
{
  return this->nfields_;
}

char const *
TAO::TypeCode::Struct<char const *,
                      CORBA::TypeCode_ptr const *,
                      TAO::TypeCode::Struct_Field<char const *,
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
TAO::TypeCode::Struct<char const *,
                      CORBA::TypeCode_ptr const *,
                      TAO::TypeCode::Struct_Field<char const *,
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

TAO_END_VERSIONED_NAMESPACE_DECL
