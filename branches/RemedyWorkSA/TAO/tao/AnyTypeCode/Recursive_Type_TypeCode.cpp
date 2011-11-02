// $Id$

#include "tao/CDR.h"

#ifndef __ACE_INLINE__
# include "tao/AnyTypeCode/Recursive_Type_TypeCode.inl"
#endif  /* __ACE_INLINE__ */

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

template <class TypeCodeBase, typename TypeCodeType, typename MemberArrayType>
bool
TAO::TypeCode::Recursive_Type<TypeCodeBase,
                              TypeCodeType,
                              MemberArrayType>::tao_marshal (
  TAO_OutputCDR & cdr,
  CORBA::ULong offset) const
{
  ACE_GUARD_RETURN (TAO_SYNCH_RECURSIVE_MUTEX,
                    guard,
                    this->lock_,
                    false);

  // Top-level TypeCode case.
  if (this->recursion_start_offset_ == 0)
    {
      // Starting offset should point to the CORBA::TCKind value.
      // Note that this doesn't need to take into account alignment
      // padding since CORBA::TCKind (encoded as a CORBA::ULong) is
      // already aligned on the appropriate boundary, and since the
      // CORBA::TCKind was the last thing marshaled into the CDR
      // stream before getting here.
      this->recursion_start_offset_ = offset - sizeof (CORBA::ULong);

      // Reset recursion flag to false in an exception-safe manner once
      // marshaling is done.
      //
      // Only reset the recursion flag at the top-level.
      Reset flag (this->recursion_start_offset_);

      return this->TypeCodeBase::tao_marshal (cdr, offset);
    }

  // Recursive/indirected TypeCode case.

//   ACE_ASSERT (offset > 4
//               && offset < static_cast<CORBA::ULong> (ACE_INT32_MAX));

  return (cdr << -static_cast<CORBA::Long> (offset - this->recursion_start_offset_));
}

template <class TypeCodeBase, typename TypeCodeType, typename MemberArrayType>
CORBA::Boolean
TAO::TypeCode::Recursive_Type<TypeCodeBase,
                              TypeCodeType,
                              MemberArrayType>::equal_i (
  CORBA::TypeCode_ptr tc) const
{
  ACE_GUARD_RETURN (TAO_SYNCH_RECURSIVE_MUTEX,
                    guard,
                    this->lock_,
                    false);

  // Top-level TypeCode case.
  if (this->recursion_start_offset_ == 0)
    {
      // no need for real start offset here
      // just set to non-zero to flag recursion
      this->recursion_start_offset_ = 1;

      // Reset recursion flag to false in an exception-safe manner once
      // equality determination is done.
      //
      // Only reset the recursion flag at the top-level.
      Reset flag (this->recursion_start_offset_);

      return this->TypeCodeBase::equal_i (tc);
    }

  // Nothing else to do.
  return true;
}

template <class TypeCodeBase, typename TypeCodeType, typename MemberArrayType>
CORBA::Boolean
TAO::TypeCode::Recursive_Type<TypeCodeBase,
                              TypeCodeType,
                              MemberArrayType>::equivalent_i (
  CORBA::TypeCode_ptr tc) const
{
  ACE_GUARD_RETURN (TAO_SYNCH_RECURSIVE_MUTEX,
                    guard,
                    this->lock_,
                    false);

  // Top-level TypeCode case.
  if (this->recursion_start_offset_ == 0)
    {
      // no need for real start offset here
      // just set to non-zero to flag recursion
      this->recursion_start_offset_ = 1;

      // Reset recursion flag to false in an exception-safe manner once
      // equivalence determination is done.
      //
      // Only reset the recursion flag at the top-level.
      Reset flag (this->recursion_start_offset_);

      return this->TypeCodeBase::equivalent_i (tc);
    }

  // Nothing else to do.
  return true;
}

template <class TypeCodeBase, typename TypeCodeType, typename MemberArrayType>
bool
TAO::TypeCode::Recursive_Type<TypeCodeBase,
                              TypeCodeType,
                              MemberArrayType>::tao_marshal_kind (
  TAO_OutputCDR & cdr) const
{
  ACE_GUARD_RETURN (TAO_SYNCH_RECURSIVE_MUTEX,
                    guard,
                    this->lock_,
                    false);

  // Top-level TypeCode case.
  if (this->recursion_start_offset_ == 0)
    return this->::CORBA::TypeCode::tao_marshal_kind (cdr);

  // Recursive/indirected TypeCode case.
  CORBA::ULong const indirection_kind = 0xffffffff;

  return (cdr << indirection_kind);
}

TAO_END_VERSIONED_NAMESPACE_DECL
