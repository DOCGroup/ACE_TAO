// $Id$

#ifndef TAO_UNION_TYPECODE_CPP
#define TAO_UNION_TYPECODE_CPP

#include "tao/Union_TypeCode.h"
#include "tao/TypeCode_Case.h"

#ifndef __ACE_INLINE__
# include "tao/Union_TypeCode.inl"
#endif  /* !__ACE_INLINE__ */


template <typename StringType, class CaseArrayType, class RefCountPolicy>
bool
TAO::TypeCode::Union<StringType,
                     CaseArrayType,
                     RefCountPolicy>::tao_marshal (
  TAO_OutputCDR & cdr) const
{
  // A tk_union TypeCode has a "complex" parameter list type (see
  // Table 15-2 in Section 15.3.5.1 "TypeCode" in the CDR section of
  // the CORBA specification), meaning that it must be marshaled into
  // a CDR encapsulation.

  // Create a CDR encapsulation.
  bool const success =
    (cdr << TAO_ENCAP_BYTE_ORDER)
    && (cdr << this->base_attributes_.id ())
    && (cdr << this->base_attributes_.name ())
    && (cdr << *(this->discriminant_type_))
    && (cdr << this->default_index_)
    && (cdr << this->ncases_);

  if (!success)
    return false;

  // Note that we handle the default case below, too.  The default
  // case handling is hidden behind the case_count() and case()
  // methods.

  CORBA::ULong const len = this->case_count ();

  for (unsigned int i = 0; i < len; ++i)
    {
      Case const & c = this->case (i);

      if (!c.marshal (cdr))
        return false;
    }

  return true;
}

template <typename StringType, class CaseArrayType, class RefCountPolicy>
void
TAO::TypeCode::Union<StringType,
                     CaseArrayType,
                     RefCountPolicy>::tao_duplicate (void)
{
  this->RefCountPolicy::add_ref (void);
}

template <typename StringType, class CaseArrayType, class RefCountPolicy>
void
TAO::TypeCode::Union<StringType,
                     CaseArrayType,
                     RefCountPolicy>::tao_release (void)
{
  this->RefCountPolicy::remove_ref (void);
}

template <typename StringType, class CaseArrayType, class RefCountPolicy>
CORBA::Boolean
TAO::TypeCode::Union<StringType,
                     CaseArrayType,
                     RefCountPolicy>::equal_i (
  CORBA::TypeCode_ptr tc
  ACE_ENV_ARG_DECL) const
{
  // This call shouldn't throw since CORBA::TypeCode::equal() verified
  // that the TCKind is the same as our's prior to invoking this
  // method, meaning that member_count() is supported.

  CORBA::ULong const tc_count =
    tc->member_count (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK_RETURN (0);

  CORBA::ULong const this_count = this->case_count ();

  if (tc_count != this_count)
    return 0;

  ............. ADD REMAINING ATTRIBUTE CHECKS ...........

  for (CORBA::ULong i = 0; i < this_count; ++i)
    {
      Case const & lhs_case = this->cases (i);

      char const * const lhs_name = lhs_case.name ();
      char const * const rhs_name = tc->member_name (i
                                                     ACE_ENV_ARG_PARAMETER);
      ACE_CHECK_RETURN (0);

      if (ACE_OS::strcmp (lhs_name, rhs_name) != 0)
        return 0;

      CORBA::TypeCode_ptr const lhs_tc = *(lhs_case.type);
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

template <typename StringType, class CaseArrayType, class RefCountPolicy>
CORBA::Boolean
TAO::TypeCode::Union<StringType,
                     CaseArrayType,
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

  // Call kind_i() instead of using CORBA::tk_union directly since a
  // subclass, such as Except_TypeCode, can use this equivalent_i()
  // implementation.
  CORBA::TCKind const this_kind =
    this->kind_i (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK_RETURN (0);

  if (tc_kind != this_kind)
    return 0;

  char const * const this_id = this->base_attributes_.id ();
  char const * const tc_id   = tc->id (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK_RETURN (0);

  if (ACE_OS::strlen (this_id) == 0
      || ACE_OS::strlen (tc_id) == 0)
    {
      // Perform a structural comparison, excluding the name() and
      // member_name() operations.

      CORBA::ULong const tc_count =
        tc->member_count (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_CHECK_RETURN (0);

      CORBA::Long tc_def = tc->default_index (ACE_ENV_ARG_PARAMETER);
      ACE_CHECK_RETURN (0);

      CORBA::ULong const this_count = this->case_count ();

      if (tc_count != this_count
          || tc_def != this->default_index_)
        return 0;

      CORBA::TypeCode_var tc_discriminator =
        tc->discriminator_type (ACE_ENV_ARG_PARAMETER);
      ACE_CHECK_RETURN (0);

      CORBA::Boolean const equiv_discriminators =
        (*this->discriminator_type_)->equivalent (tc_discriminator.in ()
                                                  ACE_ENV_ARG_PARAMETER);
      ACE_CHECK_RETURN (0);

      if (!equiv_discriminators)
        return 0;

      for (CORBA::ULong i = 0; i < this_count; ++i)
        {
          ....  CHECK EQUIVALENCE OF MEMBER LABELS ...

          CORBA::TypeCode_ptr const lhs = this->case (i).type ();
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

template <typename StringType, class CaseArrayType, class RefCountPolicy>
CORBA::TCKind
TAO::TypeCode::Union<StringType, CaseArrayType, RefCountPolicy>::kind_i (
  ACE_ENV_SINGLE_ARG_DECL_NOT_USED) const
{
  return CORBA::tk_union;
}

template <typename StringType, class CaseArrayType, class RefCountPolicy>
CORBA::TypeCode_ptr
TAO::TypeCode::Union<StringType,
                     CaseArrayType,
                     RefCountPolicy>::get_compact_typecode_i (
  ACE_ENV_SINGLE_ARG_DECL) const
{
  Case * tc_cases = 0;

  ACE_Auto_Array_Ptr<Case> safe_cases;

  CORBA::ULong const len = this->case_count ();

............. BUSTED ... NO DEFAULT CASE HANDLING ...

  if (len > 0)
    {
      // Dynamically construct a new array of cases stripped of
      // member names.

      ACE_NEW_THROW_EX (tc_cases,
                        Case[len],
                        CORBA::NO_MEMORY ());
      ACE_CHECK_RETURN (CORBA::TypeCode::_nil ());

      safe_cases.reset (cases);

      for (CORBA::ULong i = 0; i < len; ++i)
        {
          // Member names will be stripped, i.e. not embedded within
          // the compact TypeCode.

          tc_cases[i].type =
            &(this->cases (i).type ()->get_compact_typecode (
                  ACE_ENV_ARG_PARAMETER));
          ACE_CHECK_RETURN (CORBA::TypeCode::_nil ());
        }
    }

  TAO_TypeCodeFactory_Adapter * adapter =
    ACE_Dynamic_Service<TAO_TypeCodeFactory_Adapter>::instance (
      TAO_ORB_Core::typecodefactory_adapter_name ());

  if (adapter == 0)
    {
      ACE_THROW_RETURN (CORBA::INTERNAL (),
                        CORBA::TypeCode::_nil ());
    }

  CORBA::TCKind const this_kind =
    this->kind_i (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK_RETURN (CORBA::TypeCode::_nil ());

  tc = adapter->_tao_create_union_except_tc (........ FIX ME ....
                                             ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (CORBA::TypeCode::_nil ());

  (void) safe_cases.release ();

  return tc;
}

template <typename StringType, class CaseArrayType, class RefCountPolicy>
char const *
TAO::TypeCode::Union<StringType, CaseArrayType, RefCountPolicy>::id_i (
  ACE_ENV_SINGLE_ARG_DECL) const
{
  // Ownership is retained by the TypeCode, as required by the C++
  // mapping.
  return this->base_attributes_.id ();
}

template <typename StringType, class CaseArrayType, class RefCountPolicy>
char const *
TAO::TypeCode::Union<StringType, CaseArrayType, RefCountPolicy>::name_i (
  ACE_ENV_SINGLE_ARG_DECL) const
{
  // Ownership is retained by the TypeCode, as required by the C++
  // mapping.
  return this->base_attributes_.name ();
}

template <typename StringType, class CaseArrayType, class RefCountPolicy>
CORBA::ULong
TAO::TypeCode::Union<StringType,
                      CaseArrayType,
                      RefCountPolicy>::member_count_i (
  ACE_ENV_SINGLE_ARG_DECL_NOT_USED) const
{
  return this->case_count ();
}

template <typename StringType, class CaseArrayType, class RefCountPolicy>
char const *
TAO::TypeCode::Union<StringType,
                     CaseArrayType,
                     RefCountPolicy>::member_name_i (
  CORBA::ULong index
  ACE_ENV_ARG_DECL) const
{
  // Ownership is retained by the TypeCode, as required by the C++
  // mapping.
  if (index >= this->case_count ())
    ACE_THROW_RETURN (CORBA::TypeCode::Bounds (), 0);

  return this->case (index).name ();
}

template <typename StringType, class CaseArrayType, class RefCountPolicy>
CORBA::TypeCode_ptr
TAO::TypeCode::Union<StringType,
                     CaseArrayType,
                     RefCountPolicy>::member_type_i (
  CORBA::ULong index
  ACE_ENV_ARG_DECL) const
{
  if (index >= this->case_count ())
    ACE_THROW_RETURN (CORBA::TypeCode::Bounds (),
                      CORBA::TypeCode::_nil ());

  return CORBA::TypeCode::_duplicate (this->case (index).type ());
}

template <typename StringType, class CaseArrayType, class RefCountPolicy>
CORBA::Any *
TAO::TypeCode::Union<StringType,
                     CaseArrayType,
                     RefCountPolicy>::member_label_i (ULong index
                                                      ACE_ENV_ARG_DECL) const
{
  if (index >= this->case_count ())
    ACE_THROW_RETURN (CORBA::TypeCode::Bounds (),
                      0);

  return this->case (index).label (ACE_ENV_ARG_PARAMETER);
}

template <typename StringType, class CaseArrayType, class RefCountPolicy>
CORBA::TypeCode_ptr
TAO::TypeCode::Union<StringType,
                     CaseArrayType,
                     RefCountPolicy>::discriminator_type_i (
  ACE_ENV_SINGLE_ARG_DECL) const
{
  return CORBA::TypeCode::_duplicate (*this->discriminator_type_);
}

template <typename StringType, class CaseArrayType, class RefCountPolicy>
CORBA::Long
TAO::TypeCode::Union<StringType,
                     CaseArrayType,
                     RefCountPolicy>::default_index_i (
  ACE_ENV_SINGLE_ARG_DECL) const
{
  return this->default_index_;
}


#endif  /* TAO_UNION_TYPECODE_CPP */
