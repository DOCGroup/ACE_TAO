// $Id$

#ifndef TAO_SEQUENCE_TYPECODE_CPP
#define TAO_SEQUENCE_TYPECODE_CPP

#include "tao/Sequence_TypeCode.h"

#ifndef __ACE_INLINE__
# include "tao/Sequence_TypeCode.inl"
#endif  /* !__ACE_INLINE__ */


template <typename TypeCodeType, class RefCountPolicy>
bool
TAO::TypeCode::Sequence<TypeCodeType, RefCountPolicy>::tao_marshal (
  TAO_OutputCDR & cdr) const
{
  // A tk_array or tk_sequence TypeCode has a "complex" parameter list
  // type (see Table 15-2 in Section 15.3.5.1 "TypeCode" in the CDR
  // section of the CORBA specification), meaning that it must be
  // marshaled into a CDR encapsulation.

  // Create a CDR encapsulation.
  return
    (cdr << TAO_OutputCDR::from_boolean (TAO_ENCAP_BYTE_ORDER))
    && (cdr << Traits<TypeCodeType>::get_typecode (this->content_type_))
    && (cdr << this->length_);
}

template <typename TypeCodeType, class RefCountPolicy>
void
TAO::TypeCode::Sequence<TypeCodeType, RefCountPolicy>::tao_duplicate (void)
{
  this->RefCountPolicy::add_ref ();
}

template <typename TypeCodeType, class RefCountPolicy>
void
TAO::TypeCode::Sequence<TypeCodeType, RefCountPolicy>::tao_release (void)
{
  this->RefCountPolicy::remove_ref ();
}

template <typename TypeCodeType, class RefCountPolicy>
CORBA::Boolean
TAO::TypeCode::Sequence<TypeCodeType,
                        RefCountPolicy>::equal_i (CORBA::TypeCode_ptr tc
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

  return
    Traits<TypeCodeType>::get_typecode (this->content_type_)->equal (
      rhs_content_type.in ()
      ACE_ENV_ARG_PARAMETER);
}

template <typename TypeCodeType, class RefCountPolicy>
CORBA::Boolean
TAO::TypeCode::Sequence<TypeCodeType,
                        RefCountPolicy>::equivalent_i (CORBA::TypeCode_ptr tc
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

  return
    Traits<TypeCodeType>::get_typecode (this->content_type_)->equivalent (
      rhs_content_type.in ()
      ACE_ENV_ARG_PARAMETER);
}

template <typename TypeCodeType, class RefCountPolicy>
CORBA::TypeCode_ptr
TAO::TypeCode::Sequence<TypeCodeType, RefCountPolicy>::get_compact_typecode_i (
  ACE_ENV_SINGLE_ARG_DECL_NOT_USED) const
{
  // Already compact since tk_sequence and tk_array TypeCodes have no
  // name or member names, meaning that we can simply call
  // _duplicate() on this TypeCode.

  // @@ There is a potential problem here if this TypeCode is a static
  //    and const since it may have been placed in read-only memory by
  //    the compiler.  A const_cast<> can return undefined results in
  //    that case.

  CORBA::TypeCode_ptr mutable_tc =
    const_cast<TAO::TypeCode::Sequence<TypeCodeType, RefCountPolicy> *> (this);

  return CORBA::TypeCode::_duplicate (mutable_tc);
}

template <typename TypeCodeType, class RefCountPolicy>
CORBA::ULong
TAO::TypeCode::Sequence<TypeCodeType, RefCountPolicy>::length_i (
  ACE_ENV_SINGLE_ARG_DECL_NOT_USED) const
{
  return this->length_;
}

template <typename TypeCodeType, class RefCountPolicy>
CORBA::TypeCode_ptr
TAO::TypeCode::Sequence<TypeCodeType, RefCountPolicy>::content_type_i (
  ACE_ENV_SINGLE_ARG_DECL_NOT_USED) const
{
  return
    CORBA::TypeCode::_duplicate (
      Traits<TypeCodeType>::get_typecode (this->content_type_));
}


#endif  /* TAO_SEQUENCE_TYPECODE_CPP */
