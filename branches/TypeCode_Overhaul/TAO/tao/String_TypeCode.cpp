// $Id$

#ifndef TAO_STRING_TYPECODE_CPP
#define TAO_STRING_TYPECODE_CPP

#include "String_TypeCode.h"

#ifndef __ACE_INLINE__
# include "tao/String_TypeCode.inl"
#endif  /* !__ACE_INLINE__ */


template <class RefCountPolicy>
bool
TAO::TypeCode::String<RefCountPolicy>::tao_marshal (TAO_OutputCDR & cdr) const
{
  // A tk_string TypeCode has a "simple" parameter list type (see
  // Table 15-2 in Section 15.3.5.1 "TypeCode" in the CDR section of
  // the CORBA specification), meaning that its parameter(s) must be
  // marshaled immediately following the TCKind.  No CDR encapsulation
  // is to be created.

  return (cdr << this->length_);
}

template <class RefCountPolicy>
void
TAO::TypeCode::String<RefCountPolicy>::tao_duplicate (void)
{
  this->RefCountPolicy::add_ref (void);
}

template <class RefCountPolicy>
void
TAO::TypeCode::String<RefCountPolicy>::tao_release (void)
{
  this->RefCountPolicy::remove_ref (void);
}

template <class RefCountPolicy>
CORBA::Boolean
TAO::TypeCode::String<RefCountPolicy>::equal_i (CORBA::TypeCode_ptr tc
                                                ACE_ENV_ARG_DECL) const
{
  // The following call won't throw since CORBA::TypeCode::equal() has
  // already established the kind of tc is the same as our kind.
  CORBA::ULong const tc_length = tc->length (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK_RETURN (0);

  return (this->length_ == tc_length);
}

template <class RefCountPolicy>
CORBA::Boolean
TAO::TypeCode::String<RefCountPolicy>::equivalent_i (CORBA::TypeCode_ptr tc
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

  // Call kind_i() instead of using CORBA::tk_string directly since a
  // subclass, such as WString_TypeCode, can use this equivalent_i()
  // implementation.
  CORBA::TCKind const this_kind =
    this->kind_i (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK_RETURN (0);

  if (tc_kind != this_kind)
    return 0;

  // Since TCKind comparisons must be performed before equal_i() is
  // called, we can also call it to determine equivalence of
  // tk_string-based TypeCodes.
  return this->equal_i (ACE_ENV_SINGLE_ARG_PARAMETER);
}

template <class RefCountPolicy>
CORBA::TCKind
TAO::TypeCode::String<RefCountPolicy>::kind_i (
  ACE_ENV_SINGLE_ARG_DECL_NOT_USED) const
{
  return this->kind_;
}

template <class RefCountPolicy>
CORBA::TypeCode_ptr
TAO::TypeCode::String<RefCountPolicy>::get_compact_typecode_i (
  ACE_ENV_SINGLE_ARG_DECL_NOT_USED) const
{
  // Already compact since tk_string TypeCodes have no name or member
  // names, meaning that we can simply call _duplicate() on this
  // TypeCode.
  return CORBA::TypeCode::_duplicate (this);
}

template <class RefCountPolicy>
CORBA::ULong
TAO::TypeCode::String<RefCountPolicy>::length_i (
  ACE_ENV_SINGLE_ARG_DECL_NOT_USED) const
{
  return this->length_;
}


#endif  /* TAO_STRING_TYPECODE_CPP */
