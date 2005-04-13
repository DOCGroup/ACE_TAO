// $Id$

#ifndef TAO_OBJREF_BASE_TYPECODE_CPP
#define TAO_OBJREF_BASE_TYPECODE_CPP

#include "tao/Objref_TypeCode_Base.h"

#ifndef __ACE_INLINE__
# include "tao/Objref_TypeCode_Base.inl"
#endif  /* !__ACE_INLINE__ */

#include "tao/CDR.h"


template <typename StringType, class RefCountPolicy>
bool
TAO::TypeCode::Objref_Base<StringType, RefCountPolicy>::tao_marshal (
  TAO_OutputCDR & cdr) const
{
  // A tk_objref TypeCode has a "complex" parameter list type (see
  // Table 15-2 in Section 15.3.5.1 "TypeCode" in the CDR section of
  // the CORBA specification), meaning that it must be marshaled into
  // a CDR encapsulation.

  // Create a CDR encapsulation.
  return
    (cdr << TAO_OutputCDR::from_boolean (TAO_ENCAP_BYTE_ORDER))
    && (cdr << TAO_OutputCDR::from_string (this->attributes_.id (), 0))
    && (cdr << TAO_OutputCDR::from_string (this->attributes_.name (), 0));
}

template <typename StringType, class RefCountPolicy>
void
TAO::TypeCode::Objref_Base<StringType, RefCountPolicy>::tao_duplicate (void)
{
  this->RefCountPolicy::add_ref ();
}

template <typename StringType, class RefCountPolicy>
void
TAO::TypeCode::Objref_Base<StringType, RefCountPolicy>::tao_release (void)
{
  this->RefCountPolicy::remove_ref ();
}

template <typename StringType, class RefCountPolicy>
CORBA::Boolean
TAO::TypeCode::Objref_Base<StringType, RefCountPolicy>::equal_i (
  CORBA::TypeCode_ptr /* tc */
  ACE_ENV_ARG_DECL_NOT_USED) const
{
  // Equality has already been established in the
  // CORBA::TypeCode base class.

  return 1;
}

template <typename StringType, class RefCountPolicy>
CORBA::Boolean
TAO::TypeCode::Objref_Base<StringType, RefCountPolicy>::equivalent_i (
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

  char const * const this_id = this->attributes_.id ();
  char const * const tc_id   = tc->id (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK_RETURN (0);

  if (ACE_OS::strcmp (this_id, tc_id) != 0)
    return 0;

  return 1;
}

template <typename StringType, class RefCountPolicy>
char const *
TAO::TypeCode::Objref_Base<StringType, RefCountPolicy>::id_i (
  ACE_ENV_SINGLE_ARG_DECL_NOT_USED) const
{
  // Ownership is retained by the TypeCode, as required by the C++
  // mapping.
  return this->attributes_.id ();
}

template <typename StringType, class RefCountPolicy>
char const *
TAO::TypeCode::Objref_Base<StringType, RefCountPolicy>::name_i (
  ACE_ENV_SINGLE_ARG_DECL_NOT_USED) const
{
  // Ownership is retained by the TypeCode, as required by the C++
  // mapping.
  return this->attributes_.name ();
}


#endif  /*  TAO_OBJREF_TYPECODE_BASE_CPP */
