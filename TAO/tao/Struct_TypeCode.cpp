// $Id$

#ifndef TAO_STRUCT_TYPECODE_CPP
#define TAO_STRUCT_TYPECODE_CPP

#include "tao/Struct_TypeCode.h"
#include "tao/TypeCode_Struct_Field.h"
#include "tao/TypeCode_Traits.h"
#include "tao/ORB_Core.h"
#include "tao/TypeCodeFactory_Adapter.h"
#include "tao/CDR.h"


#ifndef __ACE_INLINE__
# include "tao/Struct_TypeCode.inl"
#endif  /* !__ACE_INLINE__ */

#include "ace/Dynamic_Service.h"


template <typename StringType,
          typename TypeCodeType,
          class FieldArrayType,
          class RefCountPolicy>
bool
TAO::TypeCode::Struct<StringType,
                      TypeCodeType,
                      FieldArrayType,
                      RefCountPolicy>::tao_marshal (TAO_OutputCDR & cdr) const
{
  // A tk_struct TypeCode has a "complex" parameter list type (see
  // Table 15-2 in Section 15.3.5.1 "TypeCode" in the CDR section of
  // the CORBA specification), meaning that it must be marshaled into
  // a CDR encapsulation.

  // Create a CDR encapsulation.
  bool const success =
    (cdr << TAO_OutputCDR::from_boolean (TAO_ENCAP_BYTE_ORDER))
    && (cdr << TAO_OutputCDR::from_string (this->base_attributes_.id (), 0))
    && (cdr << TAO_OutputCDR::from_string (this->base_attributes_.name (), 0))
    && (cdr << this->nfields_);

  if (!success)
    return false;

  Struct_Field<StringType, TypeCodeType> const * const begin =
    &this->fields_[0];
  Struct_Field<StringType, TypeCodeType> const * const end =
    begin + this->nfields_;

  for (Struct_Field<StringType, TypeCodeType> const * i = begin; i != end; ++i)
    {
      Struct_Field<StringType, TypeCodeType> const & field = *i;

      if (!(cdr << TAO_OutputCDR::from_string (
                       Traits<StringType>::get_string (field.name), 0))
          || !(cdr << Traits<StringType>::get_typecode (field.type)))
        return false;
    }

  return true;
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
CORBA::Boolean
TAO::TypeCode::Struct<StringType,
                      TypeCodeType,
                      FieldArrayType,
                      RefCountPolicy>::equal_i (
  CORBA::TypeCode_ptr tc
  ACE_ENV_ARG_DECL) const
{
  // This call shouldn't throw since CORBA::TypeCode::equal() verified
  // that the TCKind is the same as our's prior to invoking this
  // method, meaning that member_count() is supported.

  CORBA::ULong const tc_nfields =
    tc->member_count (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK_RETURN (0);

  if (tc_nfields != this->nfields_)
    return 0;

  for (CORBA::ULong i = 0; i < this->nfields_; ++i)
    {
      Struct_Field<StringType, TypeCodeType> const & lhs_field =
        this->fields_[i];

      char const * const lhs_name =
        Traits<StringType>::get_string (lhs_field.name);
      char const * const rhs_name = tc->member_name (i
                                                     ACE_ENV_ARG_PARAMETER);
      ACE_CHECK_RETURN (0);

      if (ACE_OS::strcmp (lhs_name, rhs_name) != 0)
        return 0;

      CORBA::TypeCode_ptr const lhs_tc =
        Traits<StringType>::get_typecode (lhs_field.type);
      CORBA::TypeCode_var const rhs_tc =
        tc->member_type (i
                         ACE_ENV_ARG_PARAMETER);
      ACE_CHECK_RETURN (0);

      CORBA::Boolean const equal_members =
        lhs_tc->equal (rhs_tc.in ()
                       ACE_ENV_ARG_PARAMETER);
      ACE_CHECK_RETURN (0);

      if (!equal_members)
        return 0;
    }

  return 1;
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
  CORBA::TypeCode_ptr tc
  ACE_ENV_ARG_DECL) const
{
  // We could refactor this code to the CORBA::TypeCode::equivalent()
  // method but doing so would force us to determine the unaliased
  // kind of this TypeCode.  Since we already know the unaliased kind
  // of this TypeCode, choose to optimize away the additional kind
  // unaliasing operation rather than save space.

  CORBA::TCKind const tc_kind =
    TAO::unaliased_kind (tc
                         ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (0);

  if (tc_kind != this->kind_)
    return 0;

  char const * const this_id = this->base_attributes_.id ();
  char const * const tc_id   = tc->id (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK_RETURN (0);

  if (ACE_OS::strlen (this_id) == 0
      || ACE_OS::strlen (tc_id) == 0)
    {
      // Perform a structural comparison, excluding the name() and
      // member_name() operations.

      CORBA::ULong const tc_nfields =
        tc->member_count (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_CHECK_RETURN (0);

      if (tc_nfields != this->nfields_)
        return 0;

      for (CORBA::ULong i = 0; i < this->nfields_; ++i)
        {
          CORBA::TypeCode_ptr const lhs =
            Traits<StringType>::get_typecode (this->fields_[i].type);
          CORBA::TypeCode_var const rhs =
            tc->member_type (i
                             ACE_ENV_ARG_PARAMETER);
          ACE_CHECK_RETURN (0);

          CORBA::Boolean const equiv_members =
            lhs->equivalent (rhs.in ()
                             ACE_ENV_ARG_PARAMETER);
          ACE_CHECK_RETURN (0);

          if (!equiv_members)
            return 0;
        }
    }
  else if (ACE_OS::strcmp (this_id, tc_id) != 0)
    {
      return 0;
    }

  return 1;
}

template <typename StringType,
          typename TypeCodeType,
          class FieldArrayType,
          class RefCountPolicy>
CORBA::TypeCode_ptr
TAO::TypeCode::Struct<StringType,
                      TypeCodeType,
                      FieldArrayType,
                      RefCountPolicy>::get_compact_typecode_i (
  ACE_ENV_SINGLE_ARG_DECL) const
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
              this->fields_[i].type)->get_compact_typecode (
                ACE_ENV_SINGLE_ARG_PARAMETER);
          ACE_CHECK_RETURN (CORBA::TypeCode::_nil ());
        }
    }

  TAO_TypeCodeFactory_Adapter * const adapter =
    ACE_Dynamic_Service<TAO_TypeCodeFactory_Adapter>::instance (
      TAO_ORB_Core::typecodefactory_adapter_name ());

  if (adapter == 0)
    {
      ACE_THROW_RETURN (CORBA::INTERNAL (),
                        CORBA::TypeCode::_nil ());
    }

  return
    adapter->create_struct_except_tc (this->kind_,
                                      this->base_attributes_.id (),
                                      ""  /* empty name */,
                                      tc_fields,
                                      this->nfields_
                                      ACE_ENV_ARG_PARAMETER);
}

template <typename StringType,
          typename TypeCodeType,
          class FieldArrayType,
          class RefCountPolicy>
char const *
TAO::TypeCode::Struct<StringType,
                      TypeCodeType,
                      FieldArrayType,
                      RefCountPolicy>::id_i (
  ACE_ENV_SINGLE_ARG_DECL_NOT_USED) const
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
                      RefCountPolicy>::name_i (
  ACE_ENV_SINGLE_ARG_DECL_NOT_USED) const
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
                      RefCountPolicy>::member_count_i (
  ACE_ENV_SINGLE_ARG_DECL_NOT_USED) const
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
  CORBA::ULong index
  ACE_ENV_ARG_DECL) const
{
  // Ownership is retained by the TypeCode, as required by the C++
  // mapping.
  if (index >= this->nfields_)
    ACE_THROW_RETURN (CORBA::TypeCode::Bounds (), 0);

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
  CORBA::ULong index
  ACE_ENV_ARG_DECL) const
{
  if (index >= this->nfields_)
    ACE_THROW_RETURN (CORBA::TypeCode::Bounds (),
                      CORBA::TypeCode::_nil ());

  return
    CORBA::TypeCode::_duplicate (
      Traits<StringType>::get_typecode (this->fields_[index].type));
}

#endif  /* TAO_STRUCT_TYPECODE_CPP */
