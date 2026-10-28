// -*- C++ -*-
#include "tao/AnyTypeCode/Fixed_TypeCode_Static.h"
#include "tao/CDR.h"

#ifndef __ACE_INLINE__
# include "tao/AnyTypeCode/Fixed_TypeCode_Static.inl"
#endif  /* !__ACE_INLINE__ */

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

bool
TAO::TypeCode::Fixed<TAO::Null_RefCount_Policy>::tao_marshal (
  TAO_OutputCDR & cdr,
  ::CORBA::ULong) const
{
  return (cdr << this->digits_) && (cdr << this->scale_);
}

void
TAO::TypeCode::Fixed<TAO::Null_RefCount_Policy>::tao_duplicate ()
{
}

void
TAO::TypeCode::Fixed<TAO::Null_RefCount_Policy>::tao_release ()
{
}

::CORBA::Boolean
TAO::TypeCode::Fixed<TAO::Null_RefCount_Policy>::equal_i (
  ::CORBA::TypeCode_ptr tc) const
{
  return this->digits_ == tc->fixed_digits () && this->scale_ == tc->fixed_scale ();
}

::CORBA::Boolean
TAO::TypeCode::Fixed<TAO::Null_RefCount_Policy>::equivalent_i (
  ::CORBA::TypeCode_ptr tc) const
{
  return this->equal_i (tc);
}

::CORBA::TypeCode_ptr
TAO::TypeCode::Fixed<TAO::Null_RefCount_Policy>::get_compact_typecode_i () const
{
  ::CORBA::TypeCode_ptr mutable_tc =
    const_cast<TAO::TypeCode::Fixed<TAO::Null_RefCount_Policy> *> (this);

  return ::CORBA::TypeCode::_duplicate (mutable_tc);
}

::CORBA::UShort
TAO::TypeCode::Fixed<TAO::Null_RefCount_Policy>::fixed_digits_i () const
{
  return this->digits_;
}

::CORBA::UShort
TAO::TypeCode::Fixed<TAO::Null_RefCount_Policy>::fixed_scale_i () const
{
  return this->scale_;
}

TAO_END_VERSIONED_NAMESPACE_DECL
