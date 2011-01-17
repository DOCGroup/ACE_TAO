// -*- C++ -*-
// $Id$

#include "tao/AnyTypeCode/Union_TypeCode_Static.h"
#include "tao/AnyTypeCode/TypeCode_Case_Base_T.h"
#include "tao/AnyTypeCode/Any.h"
#include "tao/SystemException.h"

#ifndef __ACE_INLINE__
# include "tao/AnyTypeCode/Union_TypeCode_Static.inl"
#endif  /* !__ACE_INLINE__ */

#include "ace/Value_Ptr.h"
#include "ace/CORBA_macros.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

bool
TAO::TypeCode::Union<char const *,
                     CORBA::TypeCode_ptr const *,
                     TAO::TypeCode::Case<char const *,
                                         CORBA::TypeCode_ptr const *> const * const *,
                     TAO::Null_RefCount_Policy>::tao_marshal (
  TAO_OutputCDR & cdr,
  CORBA::ULong offset) const
{
  // A tk_union TypeCode has a "complex" parameter list type (see
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
    && marshal (enc,
                Traits<char const *>::get_typecode (this->discriminant_type_),
                offset + enc.total_length ())
    && (enc << this->default_index_)
    && (enc << this->ncases_);

  if (!success)
    {
      return false;
    }

  // Note that we handle the default case below, too.

  for (CORBA::ULong i = 0; i < this->ncases_; ++i)
    {
      case_type const & c = *this->cases_[i];

      if (!c.marshal (enc, offset))
        {
          return false;
        }
    }

  return
    cdr << static_cast<CORBA::ULong> (enc.total_length ())
    && cdr.write_octet_array_mb (enc.begin ());
}

void
TAO::TypeCode::Union<char const *,
                     CORBA::TypeCode_ptr const *,
                     TAO::TypeCode::Case<char const *,
                                         CORBA::TypeCode_ptr const *> const * const *,
                     TAO::Null_RefCount_Policy>::tao_duplicate (void)
{
}

void
TAO::TypeCode::Union<char const *,
                     CORBA::TypeCode_ptr const *,
                     TAO::TypeCode::Case<char const *,
                                         CORBA::TypeCode_ptr const *> const * const *,
                     TAO::Null_RefCount_Policy>::tao_release (void)
{
}

CORBA::Boolean
TAO::TypeCode::Union<char const *,
                     CORBA::TypeCode_ptr const *,
                     TAO::TypeCode::Case<char const *,
                                         CORBA::TypeCode_ptr const *> const * const *,
                     TAO::Null_RefCount_Policy>::equal_i (
  CORBA::TypeCode_ptr tc
  ) const
{
  // These calls shouldn't throw since CORBA::TypeCode::equal()
  // verified that the TCKind is the same as our's prior to invoking
  // this method, meaning that the CORBA::tk_union TypeCode methods
  // are supported.

  CORBA::ULong const tc_count = tc->member_count ();

  CORBA::Long tc_def = tc->default_index ();

  if (tc_count != this->ncases_ || tc_def != this->default_index_)
    return false;

  // Check the discriminator type.
  CORBA::TypeCode_var tc_discriminator = tc->discriminator_type ();

  CORBA::Boolean const equal_discriminators =
    Traits<char const *>::get_typecode (this->discriminant_type_)->equal (
      tc_discriminator.in ());

  if (!equal_discriminators)
    return false;

  for (CORBA::ULong i = 0; i < this->ncases_; ++i)
    {
      if (this->default_index_ > -1
          && static_cast<CORBA::ULong> (this->default_index_) == i)
        {
          // Don't bother checking equality of default case label.  It
          // will always be the zero octet (the CDR encoded value is
          // ignored).
          continue;
        }

      case_type const & lhs_case = *this->cases_[i];

      bool const equal_case = lhs_case.equal (i, tc);

      if (!equal_case)
        return false;
    }

  return true;
}

CORBA::Boolean
TAO::TypeCode::Union<char const *,
                     CORBA::TypeCode_ptr const *,
                     TAO::TypeCode::Case<char const *,
                                         CORBA::TypeCode_ptr const *> const * const *,
                     TAO::Null_RefCount_Policy>::equivalent_i (
  CORBA::TypeCode_ptr tc) const
{
  // Perform a structural comparison, excluding the name() and
  // member_name() operations.
  CORBA::ULong const tc_count = tc->member_count ();

  CORBA::Long tc_def = tc->default_index ();

  if (tc_count != this->ncases_ || tc_def != this->default_index_)
    return false;

  CORBA::TypeCode_var tc_discriminator = tc->discriminator_type ();

  CORBA::Boolean const equiv_discriminators =
    Traits<char const *>::get_typecode (this->discriminant_type_)->equivalent (
      tc_discriminator.in ());

  if (!equiv_discriminators)
    return false;

  for (CORBA::ULong i = 0; i < this->ncases_; ++i)
    {
      if (this->default_index_ > -1
          && static_cast<CORBA::ULong> (this->default_index_) == i)
        {
          // Don't bother checking equality/equivalence of default
          // case label.  It will always be the zero octet (the CDR
          // encoded value is ignored).
          continue;
        }

      case_type const & lhs_case = *this->cases_[i];

      bool const equivalent_case = lhs_case.equivalent (i, tc);

      if (!equivalent_case)
        return false;
    }

  return true;
}

CORBA::TypeCode_ptr
TAO::TypeCode::Union<char const *,
                     CORBA::TypeCode_ptr const *,
                     TAO::TypeCode::Case<char const *,
                                         CORBA::TypeCode_ptr const *> const * const *,
                     TAO::Null_RefCount_Policy>::get_compact_typecode_i (
  void) const
{
//   typedef ACE::Value_Ptr<TAO::TypeCode::Case<CORBA::String_var,
//                                              CORBA::TypeCode_var> > elem_type;

//   ACE_Array_Base<elem_type> tc_cases (this->ncases_);

//   if (this->ncases_ > 0)
//     {
//       // Dynamically construct a new array of cases stripped of
//       // member names.

//       static char const empty_name[] = "";

//       for (CORBA::ULong i = 0; i < this->ncases_; ++i)
//         {
//           // Member names will be stripped, i.e. not embedded within
//           // the compact TypeCode.
//           tc_cases[i].name = empty_name;
//           tc_cases[i].type =
//             this->cases_[i]->type ()->get_compact_typecode (
//              );
//         }
//     }

//   // Create the compact union TypeCode.
//   TAO_TypeCodeFactory_Adapter * adapter =
//     ACE_Dynamic_Service<TAO_TypeCodeFactory_Adapter>::instance (
//       TAO_ORB_Core::typecodefactory_adapter_name ());

//   if (adapter == 0)
//     {
//       throw ::CORBA::INTERNAL ();
//     }

//   return
//     adapter->create_union_tc (
//       this->base_attributes_.id (),
//       "",  /* empty name */
//       Traits<char const *>::get_typecode (this->discriminant_type_),
//       tc_cases,
//       this->ncases_,
//       this->default_index_,
//       "",
//       Traits<char const *>::get_typecode (this->default_case_.type)
//      );

  throw ::CORBA::NO_IMPLEMENT ();
}

char const *
TAO::TypeCode::Union<char const *,
                     CORBA::TypeCode_ptr const *,
                     TAO::TypeCode::Case<char const *,
                                         CORBA::TypeCode_ptr const *> const * const *,
                     TAO::Null_RefCount_Policy>::id_i (
  void) const
{
  // Ownership is retained by the TypeCode, as required by the C++
  // mapping.
  return this->base_attributes_.id ();
}

char const *
TAO::TypeCode::Union<char const *,
                     CORBA::TypeCode_ptr const *,
                     TAO::TypeCode::Case<char const *,
                                         CORBA::TypeCode_ptr const *> const * const *,
                     TAO::Null_RefCount_Policy>::name_i (void) const
{
  // Ownership is retained by the TypeCode, as required by the C++
  // mapping.
  return this->base_attributes_.name ();
}

CORBA::ULong
TAO::TypeCode::Union<char const *,
                     CORBA::TypeCode_ptr const *,
                     TAO::TypeCode::Case<char const *,
                                         CORBA::TypeCode_ptr const *> const * const *,
                     TAO::Null_RefCount_Policy>::member_count_i (void) const
{
  return this->ncases_;
}

char const *
TAO::TypeCode::Union<char const *,
                     CORBA::TypeCode_ptr const *,
                     TAO::TypeCode::Case<char const *,
                                         CORBA::TypeCode_ptr const *> const * const *,
                     TAO::Null_RefCount_Policy>::member_name_i (
  CORBA::ULong index) const
{
  // Ownership is retained by the TypeCode, as required by the C++
  // mapping.
  if (index >= this->ncases_)
    throw ::CORBA::TypeCode::Bounds ();

  return this->cases_[index]->name ();
}

CORBA::TypeCode_ptr
TAO::TypeCode::Union<char const *,
                     CORBA::TypeCode_ptr const *,
                     TAO::TypeCode::Case<char const *,
                                         CORBA::TypeCode_ptr const *> const * const *,
                     TAO::Null_RefCount_Policy>::member_type_i (
  CORBA::ULong index) const
{
  if (index >= this->ncases_)
    throw ::CORBA::TypeCode::Bounds ();

  return CORBA::TypeCode::_duplicate (this->cases_[index]->type ());
}

CORBA::Any *
TAO::TypeCode::Union<char const *,
                     CORBA::TypeCode_ptr const *,
                     TAO::TypeCode::Case<char const *,
                                         CORBA::TypeCode_ptr const *> const * const *,
                     TAO::Null_RefCount_Policy>::member_label_i (
  CORBA::ULong index) const
{
  if (index >= this->ncases_)
    throw ::CORBA::TypeCode::Bounds ();

  // Default case.
  if (this->default_index_ > -1
      && static_cast<CORBA::ULong> (this->default_index_) == index)
    {
      CORBA::Any * any = 0;
      ACE_NEW_THROW_EX (any,
                        CORBA::Any,
                        CORBA::NO_MEMORY ());

      CORBA::Any_var safe_any (any);

      // Default case's label is a zero octet.
      CORBA::Any::from_octet const zero_octet (0);

      // Default case/member has a zero octet label value.
      (*any) <<= zero_octet;

      return safe_any._retn ();
    }

  // Non-default cases.
  return this->cases_[index]->label ();
}

CORBA::TypeCode_ptr
TAO::TypeCode::Union<char const *,
                     CORBA::TypeCode_ptr const *,
                     TAO::TypeCode::Case<char const *,
                                         CORBA::TypeCode_ptr const *> const * const *,
                     TAO::Null_RefCount_Policy>::discriminator_type_i (void) const
{
  return
    CORBA::TypeCode::_duplicate (
      Traits<char const *>::get_typecode (this->discriminant_type_));
}

CORBA::Long
TAO::TypeCode::Union<char const *,
                     CORBA::TypeCode_ptr const *,
                     TAO::TypeCode::Case<char const *,
                                         CORBA::TypeCode_ptr const *> const * const *,
                     TAO::Null_RefCount_Policy>::default_index_i (void) const
{
  return this->default_index_;
}

TAO_END_VERSIONED_NAMESPACE_DECL
