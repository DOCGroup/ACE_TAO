// $Id$

#include "tao/Recursive_Type_TypeCode.h"

#ifndef __ACE_INLINE__
# include "tao/Recursive_Type_TypeCode.inl"
#endif  /* __ACE_INLINE__ */


template <class TypeCodeBase, typename TypeCodeType, typename MemberArrayType>
bool
TAO::TypeCode::Recursive_Type<TypeCodeBase,
                              TypeCodeType,
                              MemberArrayType>::tao_marshal (
  TAO_Output & cdr) const
{
  ACE_GUARD_RETURN (TAO_SYNCH_RECURSIVE_MUTEX,
                    guard,
                    this->lock_,
                    false);
  
  // Top-level TypeCode case.
  if (this->starting_offset_ == 0)
    {
      // Starting offset should point to the CORBA::TCKind value.
      //
      // Note that this subtraction doesn't need to take into account
      // alignment padding since CORBA::TCKind (encoded as a
      // CORBA::ULong) is already aligned on the appropriate boundary,
      // and since the CORBA::TCKind was the last thing marshaled into
      // the CDR stream before getting here.
      this->starting_offset_ = cdr.total_length () - sizeof (CORBA::TCKind);

      // ACE_ASSERT (this->starting_offset_ != 0);

      // Reset offset to zero in an exception-safe manner once
      // marshaling is done.
      class Offset_Reset
      {
      public:
        Offset_Reset (CORBA::Long & offset) : offset_ (offset) {}
        ~Offset_Reset (void) { this->offset_ = 0; }
      private:
        CORBA::Long & offset_;
      };

      // Only reset the offset at the top-level.

      Offset_Reset (this->starting_offset_);

      return this->TypeCodeBase::tao_marshal (cdr);
    }

  // Recursive/indirected TypeCode case.
  CORBA::ULong const indirection_kind = 0xffffffff;

  if (!(cdr << indirection_kind
        && cdr << (static_cast<CORBA::Long> (cdr.total_length ())
                   - this->starting_offset_)))
    {
      return false;
    }

  return true;
}

template <class TypeCodeBase, typename TypeCodeType, typename MemberArrayType>
CORBA::Boolean
TAO::TypeCode::Recursive_Type<TypeCodeBase,
                              TypeCodeType,
                              MemberArrayType>::equal_i (
  CORBA::TypeCode_ptr tc
  ACE_ENV_ARG_DECL) const
{
  ACE_GUARD_RETURN (TAO_SYNCH_RECURSIVE_MUTEX,
                    guard,
                    this->lock_,
                    false);
  
  // Top-level TypeCode case.
  if (!(this->in_recursion_))
    {
      this->in_recursion_ = true;

      // Reset recursion flag to false in an exception-safe manner once
      // equivalence determination is done.
      class Reset
      {
      public:
        Reset (bool & flag) : flag_ (flag) {}
        ~Reset (void) { this->flag_ = false; }
      private:
        bool & flag_;
      };

      // Only reset the recursion flag at the top-level.

      Reset flag (this->in_recursion_);

      return this->TypeCodeBase::equal_i (tc
                                          ACE_ENV_ARG_PARAMETER);
    }

  // Nothing else to do.
  return true;
}

template <class TypeCodeBase, typename TypeCodeType, typename MemberArrayType>
CORBA::Boolean
TAO::TypeCode::Recursive_Type<TypeCodeBase,
                              TypeCodeType,
                              MemberArrayType>::equivalent_i (
  CORBA::TypeCode_ptr tc
  ACE_ENV_ARG_DECL) const
{
  ACE_GUARD_RETURN (TAO_SYNCH_RECURSIVE_MUTEX,
                    guard,
                    this->lock_,
                    false);
  
  // Top-level TypeCode case.
  if (!(this->in_recursion_))
    {
      this->in_recursion_ = true;

      // Reset recursion flag to false in an exception-safe manner once
      // equality determination is done.
      class Reset
      {
      public:
        Reset (bool & flag) : flag_ (flag) {}
        ~Reset (void) { this->flag_ = false; }
      private:
        bool & flag_;
      };

      // Only reset the recursion flag at the top-level.

      Reset flag (this->in_recursion_);

      return this->TypeCodeBase::equivalent_i (tc
                                               ACE_ENV_ARG_PARAMETER);
    }

  // Nothing else to do.
  return true;
}
