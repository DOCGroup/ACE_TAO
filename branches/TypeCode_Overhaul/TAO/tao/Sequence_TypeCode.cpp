// $Id$

#ifndef TAO_SEQUENCE_TYPECODE_CPP
#define TAO_SEQUENCE_TYPECODE_CPP

#include "Sequence_TypeCode.h"

#ifndef __ACE_INLINE__
# include "tao/Sequence_TypeCode.inl"
#endif  /* !__ACE_INLINE__ */

template <class RefCountPolicy>
TAO::TypeCode::Sequence<RefCountPolicy>::~Sequence (void)
{
  if (this->content_type_)
    CORBA::release (*this->content_type_);
}

template <class RefCountPolicy>
bool
TAO::TypeCode::Sequence<RefCountPolicy>::tao_marshal (
  TAO_OutputCDR & cdr) const
{
  // A tk_array or tk_sequence TypeCode has a "complex" parameter list
  // type (see Table 15-2 in Section 15.3.5.1 "TypeCode" in the CDR
  // section of the CORBA specification), meaning that it must be
  // marshaled into a CDR encapsulation.

  // Create a CDR encapsulation.
  return
    (cdr << TAO_OutputCDR::from_boolean (TAO_ENCAP_BYTE_ORDER))
    && (cdr << *(this->content_type_))
    && (cdr << this->length_);
}

template <class RefCountPolicy>
void
TAO::TypeCode::Sequence<RefCountPolicy>::tao_duplicate (void)
{
  this->RefCountPolicy::add_ref ();
}

template <class RefCountPolicy>
void
TAO::TypeCode::Sequence<RefCountPolicy>::tao_release (void)
{
  this->RefCountPolicy::remove_ref ();
}

template <class RefCountPolicy>
CORBA::Boolean
TAO::TypeCode::Sequence<RefCountPolicy>::equal_i (CORBA::TypeCode_ptr tc
                                               ACE_ENV_ARG_DECL) const
{
  // The following calls won't throw since CORBA::TypeCode::equal()
  // has already established the kind of tc is the same as our kind.
  CORBA::ULong const tc_length = tc->length (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK_RETURN (0);

  if (this->length_ != tc_length)
    return 0;

  CORBA::TypeCode_var rhs_content_type =
    tc->content_type (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK_RETURN (0);

  return *(this->content_type_)->equal (rhs_content_type.in ()
                                        ACE_ENV_ARG_PARAMETER);
}

template <class RefCountPolicy>
CORBA::Boolean
TAO::TypeCode::Sequence<RefCountPolicy>::equivalent_i (CORBA::TypeCode_ptr tc
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

  CORBA::TypeCode_var rhs_content_type =
    tc->content_type (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK_RETURN (0);

  return *(this->content_type_)->equivalent (rhs_content_type.in ()
                                             ACE_ENV_ARG_PARAMETER);
}

template <class RefCountPolicy>
CORBA::TCKind
TAO::TypeCode::Sequence<RefCountPolicy>::kind_i (
  ACE_ENV_SINGLE_ARG_DECL_NOT_USED) const
{
  return this->kind_;
}

template <class RefCountPolicy>
CORBA::TypeCode_ptr
TAO::TypeCode::Sequence<RefCountPolicy>::get_compact_typecode_i (
  ACE_ENV_SINGLE_ARG_DECL_NOT_USED) const
{
  // Already compact since tk_sequence and tk_array TypeCodes have no
  // name or member names, meaning that we can simply call
  // _duplicate() on this TypeCode.
  return CORBA::TypeCode::_duplicate (this);
}

template <class RefCountPolicy>
CORBA::TypeCode_ptr
TAO::TypeCode::Sequence<RefCountPolicy>::length_i (
  ACE_ENV_SINGLE_ARG_DECL_NOT_USED) const
{
  return this->length_;
}


#endif  /* TAO_SEQUENCE_TYPECODE_CPP */
