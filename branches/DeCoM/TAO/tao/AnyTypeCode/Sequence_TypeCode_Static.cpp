// -*- C++ -*-
// $Id$

#include "tao/AnyTypeCode/Sequence_TypeCode_Static.h"
#include "tao/CDR.h"
#include "tao/AnyTypeCode/TypeCode_Traits.h"

#ifndef __ACE_INLINE__
# include "tao/AnyTypeCode/Sequence_TypeCode_Static.inl"
#endif  /* !__ACE_INLINE__ */

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

bool
TAO::TypeCode::Sequence<CORBA::TypeCode_ptr const *,
                        TAO::Null_RefCount_Policy>::tao_marshal (
  TAO_OutputCDR & cdr,
  CORBA::ULong offset) const
{
  // A tk_array or tk_sequence TypeCode has a "complex" parameter list
  // type (see Table 15-2 in Section 15.3.5.1 "TypeCode" in the CDR
  // section of the CORBA specification), meaning that it must be
  // marshaled into a CDR encapsulation.

  // Create a CDR encapsulation.
  TAO_OutputCDR enc;

  // Account for the encoded CDR encapsulation length and byte order.
  //
  // Aligning on an octet since the next value after the CDR
  // encapsulation length will always be the byte order octet/boolean
  // in this case.
  offset = ACE_align_binary (offset + 4,
                             ACE_CDR::OCTET_ALIGN);

  return
    enc << TAO_OutputCDR::from_boolean (TAO_ENCAP_BYTE_ORDER)
    && marshal (enc,
                Traits<CORBA::TypeCode_ptr const *>::get_typecode (
                  this->content_type_),
                offset + enc.total_length ())
    && enc << this->length_
    && cdr << static_cast<CORBA::ULong> (enc.total_length ())
    && cdr.write_octet_array_mb (enc.begin ());
}

void
TAO::TypeCode::Sequence<CORBA::TypeCode_ptr const *,
                        TAO::Null_RefCount_Policy>::tao_duplicate (void)
{
}

void
TAO::TypeCode::Sequence<CORBA::TypeCode_ptr const *,
                        TAO::Null_RefCount_Policy>::tao_release (void)
{
}

CORBA::Boolean
TAO::TypeCode::Sequence<CORBA::TypeCode_ptr const *,
                        TAO::Null_RefCount_Policy>::equal_i (
  CORBA::TypeCode_ptr tc
  ) const
{
  // The following calls won't throw since CORBA::TypeCode::equal()
  // has already established the kind of tc is the same as our kind.
  CORBA::ULong const tc_length = tc->length ();

  if (this->length_ != tc_length)
    return false;

  CORBA::TypeCode_var rhs_content_type =
    tc->content_type ();

  return
    Traits<CORBA::TypeCode_ptr const *>::get_typecode (this->content_type_)->equal (
      rhs_content_type.in ()
     );
}

CORBA::Boolean
TAO::TypeCode::Sequence<CORBA::TypeCode_ptr const *,
                        TAO::Null_RefCount_Policy>::equivalent_i (
  CORBA::TypeCode_ptr tc
  ) const
{
  // The following calls won't throw since CORBA::TypeCode::equal()
  // has already established the kind of tc is the same as our kind.
  CORBA::ULong const tc_length = tc->length ();

  if (this->length_ != tc_length)
    return 0;

  CORBA::TypeCode_var rhs_content_type =
    tc->content_type ();

  return
    Traits<CORBA::TypeCode_ptr const *>::get_typecode (this->content_type_)->equivalent (
      rhs_content_type.in ()
     );
}

CORBA::TypeCode_ptr
TAO::TypeCode::Sequence<CORBA::TypeCode_ptr const *,
                        TAO::Null_RefCount_Policy>::get_compact_typecode_i (
  void) const
{
  // Already compact since tk_sequence and tk_array TypeCodes have no
  // name or member names, meaning that we can simply call
  // _duplicate() on this TypeCode.

  // @@ There is a potential problem here if this TypeCode is a static
  //    and const since it may have been placed in read-only memory by
  //    the compiler.  A const_cast<> can return undefined results in
  //    that case.

  CORBA::TypeCode_ptr mutable_tc =
    const_cast<TAO::TypeCode::Sequence<CORBA::TypeCode_ptr const *,
    TAO::Null_RefCount_Policy> *> (this);

  return CORBA::TypeCode::_duplicate (mutable_tc);
}

CORBA::ULong
TAO::TypeCode::Sequence<CORBA::TypeCode_ptr const *,
                        TAO::Null_RefCount_Policy>::length_i (
  void) const
{
  return this->length_;
}

CORBA::TypeCode_ptr
TAO::TypeCode::Sequence<CORBA::TypeCode_ptr const *,
                        TAO::Null_RefCount_Policy>::content_type_i (
  void) const
{
  return
    CORBA::TypeCode::_duplicate (
      Traits<CORBA::TypeCode_ptr const *>::get_typecode (this->content_type_));
}

TAO_END_VERSIONED_NAMESPACE_DECL
