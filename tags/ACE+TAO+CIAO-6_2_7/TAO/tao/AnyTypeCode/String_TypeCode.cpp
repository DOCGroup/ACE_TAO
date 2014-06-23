// $Id$

#ifndef TAO_STRING_TYPECODE_CPP
#define TAO_STRING_TYPECODE_CPP

#include "tao/AnyTypeCode/String_TypeCode.h"
#include "tao/CDR.h"

#ifndef __ACE_INLINE__
# include "tao/AnyTypeCode/String_TypeCode.inl"
#endif  /* !__ACE_INLINE__ */

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

template <class RefCountPolicy>
bool
TAO::TypeCode::String<RefCountPolicy>::tao_marshal (TAO_OutputCDR & cdr,
                                                    CORBA::ULong) const
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
  this->RefCountPolicy::add_ref ();
}

template <class RefCountPolicy>
void
TAO::TypeCode::String<RefCountPolicy>::tao_release (void)
{
  this->RefCountPolicy::remove_ref ();
}

template <class RefCountPolicy>
CORBA::Boolean
TAO::TypeCode::String<RefCountPolicy>::equal_i (CORBA::TypeCode_ptr tc) const
{
  // The following call won't throw since CORBA::TypeCode::equal() has
  // already established the kind of tc is the same as our kind.
  CORBA::ULong const tc_length = tc->length ();

  return (this->length_ == tc_length);
}

template <class RefCountPolicy>
CORBA::Boolean
TAO::TypeCode::String<RefCountPolicy>::equivalent_i (CORBA::TypeCode_ptr tc) const
{
  // Since TCKind comparisons must be performed before equal_i() is
  // called, we can also call it to determine equivalence of
  // tk_string-based TypeCodes.
  return this->equal_i (tc);
}

template <class RefCountPolicy>
CORBA::TypeCode_ptr
TAO::TypeCode::String<RefCountPolicy>::get_compact_typecode_i (void) const
{
  // Already compact since tk_string and tk_wstring TypeCodes have no
  // name or member names, meaning that we can simply call
  // _duplicate() on this TypeCode.

  CORBA::TypeCode_ptr mutable_tc =
    const_cast<TAO::TypeCode::String<RefCountPolicy> *> (this);

  return CORBA::TypeCode::_duplicate (mutable_tc);
}

template <class RefCountPolicy>
CORBA::ULong
TAO::TypeCode::String<RefCountPolicy>::length_i (void) const
{
  return this->length_;
}

TAO_END_VERSIONED_NAMESPACE_DECL

#endif  /* TAO_STRING_TYPECODE_CPP */
