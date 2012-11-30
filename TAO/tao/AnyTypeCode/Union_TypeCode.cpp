// $Id$

#ifndef TAO_UNION_TYPECODE_CPP
#define TAO_UNION_TYPECODE_CPP

#include "tao/AnyTypeCode/Union_TypeCode.h"
#include "tao/AnyTypeCode/TypeCode_Case_Base_T.h"

#ifndef __ACE_INLINE__
# include "tao/AnyTypeCode/Union_TypeCode.inl"
#endif  /* !__ACE_INLINE__ */

#include "tao/AnyTypeCode/Any.h"

#include "ace/Value_Ptr.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

template <typename StringType,
          typename TypeCodeType,
          class CaseArrayType,
          class RefCountPolicy>
bool
TAO::TypeCode::Union<StringType,
                     TypeCodeType,
                     CaseArrayType,
                     RefCountPolicy>::tao_marshal (
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
                Traits<StringType>::get_typecode (this->discriminant_type_),
                offset + enc.total_length ())
    && (enc << this->default_index_)
    && (enc << this->ncases_);

  if (!success)
    {
      return false;
    }

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

template <typename StringType,
          typename TypeCodeType,
          class CaseArrayType,
          class RefCountPolicy>
void
TAO::TypeCode::Union<StringType,
                     TypeCodeType,
                     CaseArrayType,
                     RefCountPolicy>::tao_duplicate (void)
{
  this->RefCountPolicy::add_ref ();
}

template <typename StringType,
          typename TypeCodeType,
          class CaseArrayType,
          class RefCountPolicy>
void
TAO::TypeCode::Union<StringType,
                     TypeCodeType,
                     CaseArrayType,
                     RefCountPolicy>::tao_release (void)
{
  this->RefCountPolicy::remove_ref ();
}

template <typename StringType,
          typename TypeCodeType,
          class CaseArrayType,
          class RefCountPolicy>
const CaseArrayType &
TAO::TypeCode::Union<StringType,
                     TypeCodeType,
                     CaseArrayType,
                     RefCountPolicy>::cases (void) const
{
  return cases_;
}

template <typename StringType,
          typename TypeCodeType,
          class CaseArrayType,
          class RefCountPolicy>
CORBA::Boolean
TAO::TypeCode::Union<StringType,
                     TypeCodeType,
                     CaseArrayType,
                     RefCountPolicy>::equal_i (CORBA::TypeCode_ptr tc) const
{
  // These calls shouldn't throw since CORBA::TypeCode::equal()
  // verified that the TCKind is the same as our's prior to invoking
  // this method, meaning that the CORBA::tk_union TypeCode methods
  // are supported.

  CORBA::ULong const tc_count =
    tc->member_count ();

  CORBA::Long tc_def = tc->default_index ();

  if (tc_count != this->ncases_
      || tc_def != this->default_index_)
    return false;

  // Check the discriminator type.
  CORBA::TypeCode_var tc_discriminator =
    tc->discriminator_type ();

  CORBA::Boolean const equal_discriminators =
    Traits<StringType>::get_typecode (this->discriminant_type_)->equal (
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

template <typename StringType,
          typename TypeCodeType,
          class CaseArrayType,
          class RefCountPolicy>
CORBA::Boolean
TAO::TypeCode::Union<StringType,
                     TypeCodeType,
                     CaseArrayType,
                     RefCountPolicy>::equivalent_i (CORBA::TypeCode_ptr tc) const
{
  // Perform a structural comparison, excluding the name() and
  // member_name() operations.

  CORBA::ULong const tc_count =
    tc->member_count ();

  CORBA::Long tc_def = tc->default_index ();

  if (tc_count != this->ncases_
      || tc_def != this->default_index_)
    return false;

  CORBA::TypeCode_var tc_discriminator =
    tc->discriminator_type ();

  CORBA::Boolean const equiv_discriminators =
    Traits<StringType>::get_typecode (this->discriminant_type_)->equivalent (
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

      bool const equivalent_case =
        lhs_case.equivalent (i, tc);

      if (!equivalent_case)
        return false;
    }

  return true;
}

template <typename StringType,
          typename TypeCodeType,
          class CaseArrayType,
          class RefCountPolicy>
CORBA::TypeCode_ptr
TAO::TypeCode::Union<StringType,
                     TypeCodeType,
                     CaseArrayType,
                     RefCountPolicy>::get_compact_typecode_i (void) const
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
//       Traits<StringType>::get_typecode (this->discriminant_type_),
//       tc_cases,
//       this->ncases_,
//       this->default_index_,
//       "",
//       Traits<StringType>::get_typecode (this->default_case_.type)
//      );

  throw ::CORBA::NO_IMPLEMENT ();
}

template <typename StringType,
          typename TypeCodeType,
          class CaseArrayType,
          class RefCountPolicy>
char const *
TAO::TypeCode::Union<StringType,
                     TypeCodeType,
                     CaseArrayType,
                     RefCountPolicy>::id_i (void) const
{
  // Ownership is retained by the TypeCode, as required by the C++
  // mapping.
  return this->base_attributes_.id ();
}

template <typename StringType,
          typename TypeCodeType,
          class CaseArrayType,
          class RefCountPolicy>
char const *
TAO::TypeCode::Union<StringType,
                     TypeCodeType,
                     CaseArrayType,
                     RefCountPolicy>::name_i (void) const
{
  // Ownership is retained by the TypeCode, as required by the C++
  // mapping.
  return this->base_attributes_.name ();
}

template <typename StringType,
          typename TypeCodeType,
          class CaseArrayType,
          class RefCountPolicy>
CORBA::ULong
TAO::TypeCode::Union<StringType,
                     TypeCodeType,
                      CaseArrayType,
                      RefCountPolicy>::member_count_i (void) const
{
  return this->ncases_;
}

template <typename StringType,
          typename TypeCodeType,
          class CaseArrayType,
          class RefCountPolicy>
char const *
TAO::TypeCode::Union<StringType,
                     TypeCodeType,
                     CaseArrayType,
                     RefCountPolicy>::member_name_i (CORBA::ULong index) const
{
  // Ownership is retained by the TypeCode, as required by the C++
  // mapping.
  if (index >= this->ncases_)
    throw ::CORBA::TypeCode::Bounds ();

  return this->cases_[index]->name ();
}

template <typename StringType,
          typename TypeCodeType,
          class CaseArrayType,
          class RefCountPolicy>
CORBA::TypeCode_ptr
TAO::TypeCode::Union<StringType,
                     TypeCodeType,
                     CaseArrayType,
                     RefCountPolicy>::member_type_i (CORBA::ULong index) const
{
  if (index >= this->ncases_)
    throw ::CORBA::TypeCode::Bounds ();

  return CORBA::TypeCode::_duplicate (this->cases_[index]->type ());
}

template <typename StringType,
          typename TypeCodeType,
          class CaseArrayType,
          class RefCountPolicy>
CORBA::Any *
TAO::TypeCode::Union<StringType,
                     TypeCodeType,
                     CaseArrayType,
                     RefCountPolicy>::member_label_i (CORBA::ULong index) const
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

template <typename StringType,
          typename TypeCodeType,
          class CaseArrayType,
          class RefCountPolicy>
CORBA::TypeCode_ptr
TAO::TypeCode::Union<StringType,
                     TypeCodeType,
                     CaseArrayType,
                     RefCountPolicy>::discriminator_type_i (void) const
{
  return
    CORBA::TypeCode::_duplicate (
      Traits<StringType>::get_typecode (this->discriminant_type_));
}

template <typename StringType,
          typename TypeCodeType,
          class CaseArrayType,
          class RefCountPolicy>
CORBA::Long
TAO::TypeCode::Union<StringType,
                     TypeCodeType,
                     CaseArrayType,
                     RefCountPolicy>::default_index_i (void) const
{
  return this->default_index_;
}

TAO_END_VERSIONED_NAMESPACE_DECL

#endif  /* TAO_UNION_TYPECODE_CPP */
