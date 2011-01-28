// -*- C++ -*-
// $Id$

#include "tao/AnyTypeCode/Enum_TypeCode_Static.h"
#include "tao/AnyTypeCode/TypeCode_Traits.h"
#include "tao/ORB_Core.h"
#include "tao/CDR.h"
#include "tao/TypeCodeFactory_Adapter.h"
#include "tao/SystemException.h"

#include "ace/Dynamic_Service.h"

#ifndef __ACE_INLINE__
# include "tao/AnyTypeCode/Enum_TypeCode_Static.inl"
#endif  /* !__ACE_INLINE__ */

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

bool
TAO::TypeCode::Enum<char const *,
                    char const * const *,
                    TAO::Null_RefCount_Policy>::tao_marshal (
  TAO_OutputCDR & cdr,
  CORBA::ULong) const
{
  // A tk_enum TypeCode has a "complex" parameter list type (see
  // Table 15-2 in Section 15.3.5.1 "TypeCode" in the CDR section of
  // the CORBA specification), meaning that it must be marshaled into
  // a CDR encapsulation.

  // Create a CDR encapsulation.
  TAO_OutputCDR enc;

  bool const success =
    (enc << TAO_OutputCDR::from_boolean (TAO_ENCAP_BYTE_ORDER))
    && (enc << TAO_OutputCDR::from_string (this->base_attributes_.id (), 0))
    && (enc << TAO_OutputCDR::from_string (this->base_attributes_.name (), 0))
    && (enc << this->nenumerators_);

  if (!success)
    return false;

  char const * const * const begin = &this->enumerators_[0];
  char const * const * const end   = begin + this->nenumerators_;

  for (char const * const * i = begin; i != end; ++i)
    {
      char const * const & enumerator = *i;

      if (!(enc << TAO_OutputCDR::from_string (
              Traits<char const *>::get_string (enumerator), 0)))
        return false;
    }

  return
    cdr << static_cast<CORBA::ULong> (enc.total_length ())
    && cdr.write_octet_array_mb (enc.begin ());
}

void
TAO::TypeCode::Enum<char const *,
                    char const * const *,
                    TAO::Null_RefCount_Policy>::tao_duplicate (void)
{
}

void
TAO::TypeCode::Enum<char const *,
                    char const * const *,
                    TAO::Null_RefCount_Policy>::tao_release (void)
{
}

CORBA::Boolean
TAO::TypeCode::Enum<char const *,
                    char const * const *,
                    TAO::Null_RefCount_Policy>::equal_i (
  CORBA::TypeCode_ptr tc
  ) const
{
  // This call shouldn't throw since CORBA::TypeCode::equal() verified
  // that the TCKind is the same as our's prior to invoking this
  // method, meaning that member_count() is supported.

  CORBA::ULong const tc_nenumerators =
    tc->member_count ();

  if (tc_nenumerators != this->nenumerators_)
    return false;

  for (CORBA::ULong i = 0; i < this->nenumerators_; ++i)
    {
      char const * const & lhs_enumerator = this->enumerators_[i];

      char const * const lhs_name =
        Traits<char const *>::get_string (lhs_enumerator);
      char const * const rhs_name = tc->member_name (i);

      if (ACE_OS::strcmp (lhs_name, rhs_name) != 0)
        return false;
    }

  return true;
}

CORBA::Boolean
TAO::TypeCode::Enum<char const *,
                    char const * const *,
                    TAO::Null_RefCount_Policy>::equivalent_i (
  CORBA::TypeCode_ptr tc
  ) const
{
  // Perform a structural comparison, excluding the name() and
  // member_name() operations.

  CORBA::ULong const tc_nenumerators =
    tc->member_count ();

  if (tc_nenumerators != this->nenumerators_)
    return false;

  return true;
}

CORBA::TypeCode_ptr
TAO::TypeCode::Enum<char const *,
                    char const * const *,
                    TAO::Null_RefCount_Policy>::get_compact_typecode_i (
  void) const
{
  ACE_Array_Base<CORBA::String_var> tc_enumerators (this->nenumerators_);

  // Dynamically construct a new array of enumerators stripped of
  // member names.

  static char const empty_name[] = "";

  for (CORBA::ULong i = 0; i < this->nenumerators_; ++i)
    {
      // Member names will be stripped, i.e. not embedded within
      // the compact TypeCode.

      tc_enumerators[i] = empty_name;
    }

  TAO_TypeCodeFactory_Adapter * adapter =
    ACE_Dynamic_Service<TAO_TypeCodeFactory_Adapter>::instance (
      TAO_ORB_Core::typecodefactory_adapter_name ());

  if (adapter == 0)
    {
      throw ::CORBA::INTERNAL ();
    }

  return
    adapter->create_enum_tc (this->base_attributes_.id (),
                             ""  /* empty name */,
                             tc_enumerators,
                             this->nenumerators_
                            );
}

char const *
TAO::TypeCode::Enum<char const *,
                    char const * const *,
                    TAO::Null_RefCount_Policy>::id_i (
  void) const
{
  // Ownership is retained by the TypeCode, as required by the C++
  // mapping.
  return this->base_attributes_.id ();
}

char const *
TAO::TypeCode::Enum<char const *,
                    char const * const *,
                    TAO::Null_RefCount_Policy>::name_i (
  void) const
{
  // Ownership is retained by the TypeCode, as required by the C++
  // mapping.
  return this->base_attributes_.name ();
}

CORBA::ULong
TAO::TypeCode::Enum<char const *,
                    char const * const *,
                    TAO::Null_RefCount_Policy>::member_count_i (
  void) const
{
  return this->nenumerators_;
}

char const *
TAO::TypeCode::Enum<char const *,
                    char const * const *,
                    TAO::Null_RefCount_Policy>::member_name_i (
  CORBA::ULong index
  ) const
{
  // Ownership is retained by the TypeCode, as required by the C++
  // mapping.
  if (index >= this->nenumerators_)
    throw ::CORBA::TypeCode::Bounds ();

  return Traits<char const *>::get_string (this->enumerators_[index]);
}

TAO_END_VERSIONED_NAMESPACE_DECL
