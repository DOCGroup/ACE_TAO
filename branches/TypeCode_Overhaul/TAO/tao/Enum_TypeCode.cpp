// $Id$

#ifndef TAO_ENUM_TYPECODE_CPP
#define TAO_ENUM_TYPECODE_CPP

#include "tao/Enum_TypeCode.h"
#include "tao/TypeCode_Enumerators.h"

#ifndef __ACE_INLINE__
# include "tao/Enum_TypeCode.inl"
#endif  /* !__ACE_INLINE__ */


template <typename StringType, class EnumeratorArrayType, class RefCountPolicy>
bool
TAO::TypeCode::Enum<StringType,
                    EnumeratorArrayType,
                    RefCountPolicy>::tao_marshal (
  TAO_OutputCDR & cdr) const
{
  // A tk_enum TypeCode has a "complex" parameter list type (see
  // Table 15-2 in Section 15.3.5.1 "TypeCode" in the CDR section of
  // the CORBA specification), meaning that it must be marshaled into
  // a CDR encapsulation.

  // Create a CDR encapsulation.
  bool const success =
    (cdr << TAO_ENCAP_BYTE_ORDER)
    && (cdr << this->base_attributes_.id ())
    && (cdr << this->base_attributes_.name ())
    && (cdr << this->nenumerators_);

  if (!success)
    return false;

  Enumerator<StringType> const * const begin = this->enumerators ();
  Enumerator<StringType> const * const end   = begin + this->nenumerators_;

  for (Enumerator<StringType> const * i = begin; i != end; ++i)
    {
      Enumerator<StringType> const & enumerator = *i;

      if (!(cdr << enumerator.get_name ()))
        return false;
    }

  return true;
}

template <typename StringType, class EnumeratorArrayType, class RefCountPolicy>
void
TAO::TypeCode::Enum<StringType,
                    EnumeratorArrayType,
                    RefCountPolicy>::tao_duplicate (void)
{
  this->RefCountPolicy::add_ref ();
}

template <typename StringType, class EnumeratorArrayType, class RefCountPolicy>
void
TAO::TypeCode::Enum<StringType,
                    EnumeratorArrayType,
                    RefCountPolicy>::tao_release (void)
{
  this->RefCountPolicy::remove_ref ();
}

template <typename StringType, class EnumeratorArrayType, class RefCountPolicy>
CORBA::Boolean
TAO::TypeCode::Enum<StringType,
                    EnumeratorArrayType,
                    RefCountPolicy>::equal_i (
  CORBA::TypeCode_ptr tc
  ACE_ENV_ARG_DECL) const
{
  // This call shouldn't throw since CORBA::TypeCode::equal() verified
  // that the TCKind is the same as our's prior to invoking this
  // method, meaning that member_count() is supported.

  CORBA::ULong const tc_nenumerators =
    tc->member_count (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK_RETURN (0);

  if (tc_nenumerators != this->nenumerators_)
    return 0;

  for (CORBA::ULong i = 0; i < this->nenumerators_; ++i)
    {
      Enumerator<StringType> const & lhs_enumerator = this->enumerators_[i];

      char const * const lhs_name = lhs_enumerator.get_name ();
      char const * const rhs_name = tc->member_name (i
                                                     ACE_ENV_ARG_PARAMETER);
      ACE_CHECK_RETURN (0);

      if (ACE_OS::strcmp (lhs_name, rhs_name) != 0)
        return 0;
    }

  return 1;
}

template <typename StringType, class EnumeratorArrayType, class RefCountPolicy>
CORBA::Boolean
TAO::TypeCode::Enum<StringType,
                    EnumeratorArrayType,
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

  // Call kind_i() instead of using CORBA::tk_enum directly since a
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
      // Perform a enumural comparison, excluding the name() and
      // member_name() operations.

      CORBA::ULong const tc_nenumerators =
        tc->member_count (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_CHECK_RETURN (0);

      if (tc_nenumerators != this->nenumerators_)
        return 0;
    }
  else if (ACE_OS::strcmp (this_id, tc_id) != 0)
    {
      return 0;
    }

  return 1;
}

template <typename StringType, class EnumeratorArrayType, class RefCountPolicy>
CORBA::TCKind
TAO::TypeCode::Enum<StringType, EnumeratorArrayType, RefCountPolicy>::kind_i (
  ACE_ENV_SINGLE_ARG_DECL_NOT_USED) const
{
  return CORBA::tk_enum;
}

template <typename StringType, class EnumeratorArrayType, class RefCountPolicy>
CORBA::TypeCode_ptr
TAO::TypeCode::Enum<StringType,
                    EnumeratorArrayType,
                    RefCountPolicy>::get_compact_typecode_i (
  ACE_ENV_SINGLE_ARG_DECL) const
{
  Enumerator<StringType> * tc_enumerators = 0;

  ACE_Auto_Array_Ptr<Enumerator<StringType> > safe_enumerators;

  if (this->nenumerators_ > 0)
    {
      // Dynamically construct a new array of enumerators stripped of
      // member names.

      ACE_NEW_THROW_EX (tc_enumerators,
                        Enumerator<StringType> [this->nenumerators_],
                        CORBA::NO_MEMORY ());
      ACE_CHECK_RETURN (CORBA::TypeCode::_nil ());

      safe_enumerators.reset (enumerators);

      static char const * empty_name = "";

      for (CORBA::ULong i = 0; i < this->nenumerators_; ++i)
        {
          // Member names will be stripped, i.e. not embedded within
          // the compact TypeCode.

          tc_enumerators[i].name = empty_name;
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

  tc = adapter->_tao_create_enum_tc (this_kind,
                                     this->base_attributes_.id (),
                                     ""  /* empty name */,
                                     tc_enumerators,
                                     this->nenumerators_
                                     ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (CORBA::TypeCode::_nil ());

  (void) safe_enumerators.release ();

  return tc;
}

template <typename StringType, class EnumeratorArrayType, class RefCountPolicy>
char const *
TAO::TypeCode::Enum<StringType, EnumeratorArrayType, RefCountPolicy>::id_i (
  ACE_ENV_SINGLE_ARG_DECL_NOT_USED) const
{
  // Ownership is retained by the TypeCode, as required by the C++
  // mapping.
  return this->base_attributes_.id ();
}

template <typename StringType, class EnumeratorArrayType, class RefCountPolicy>
char const *
TAO::TypeCode::Enum<StringType, EnumeratorArrayType, RefCountPolicy>::name_i (
  ACE_ENV_SINGLE_ARG_DECL_NOT_USED) const
{
  // Ownership is retained by the TypeCode, as required by the C++
  // mapping.
  return this->base_attributes_.name ();
}

template <typename StringType, class EnumeratorArrayType, class RefCountPolicy>
CORBA::ULong
TAO::TypeCode::Enum<StringType,
                    EnumeratorArrayType,
                    RefCountPolicy>::member_count_i (
  ACE_ENV_SINGLE_ARG_DECL_NOT_USED) const
{
  return this->nenumerators_;
}

template <typename StringType, class EnumeratorArrayType, class RefCountPolicy>
char const *
TAO::TypeCode::Enum<StringType,
                    EnumeratorArrayType,
                    RefCountPolicy>::member_name_i (
  CORBA::ULong index
  ACE_ENV_ARG_DECL) const
{
  // Ownership is retained by the TypeCode, as required by the C++
  // mapping.
  if (index >= this->nenumerators_)
    ACE_THROW_RETURN (CORBA::TypeCode::Bounds (), 0);

  return this->enumerators_[index].get_name ();
}

#endif  /* TAO_ENUM_TYPECODE_CPP */
