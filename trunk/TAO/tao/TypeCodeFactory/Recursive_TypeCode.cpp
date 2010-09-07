// $Id$

#include "tao/TypeCodeFactory/Recursive_TypeCode.h"

#ifndef __ACE_INLINE__
# include "tao/TypeCodeFactory/Recursive_TypeCode.inl"
#endif  /* __ACE_INLINE__ */

#include "tao/SystemException.h"
#include "tao/ORB_Constants.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

bool
TAO::TypeCodeFactory::Recursive_TypeCode::tao_marshal (
  TAO_OutputCDR & cdr,
  CORBA::ULong offset) const
{

  if (CORBA::is_nil (this->the_typecode_))
    {
      // Marshaling an incomplete recursive TypeCode placeholder is
      // not allowed.

      throw ::CORBA::BAD_TYPECODE (CORBA::OMGVMCID | 1, CORBA::COMPLETED_NO);
    }

  return this->the_typecode_->tao_marshal (cdr, offset);
}

bool
TAO::TypeCodeFactory::Recursive_TypeCode::tao_marshal_kind (
  TAO_OutputCDR & cdr) const
{

  if (CORBA::is_nil (this->the_typecode_))
    {
      // Marshaling an incomplete recursive TypeCode placeholder is
      // not allowed.

      throw ::CORBA::BAD_TYPECODE (CORBA::OMGVMCID | 1, CORBA::COMPLETED_NO);
    }

  return this->the_typecode_->tao_marshal_kind (cdr);
}

void
TAO::TypeCodeFactory::Recursive_TypeCode::tao_duplicate (void)
{
  this->add_ref ();
}

void
TAO::TypeCodeFactory::Recursive_TypeCode::tao_release (void)
{
  this->remove_ref ();
}

CORBA::Boolean
TAO::TypeCodeFactory::Recursive_TypeCode::equal_i (CORBA::TypeCode_ptr tc) const
{
  if (CORBA::is_nil (this->the_typecode_))
    {
      // Nothing to compare yet.

      throw ::CORBA::BAD_TYPECODE (0, CORBA::COMPLETED_NO);
    }

  return this->the_typecode_->equal (tc);
}

CORBA::Boolean
TAO::TypeCodeFactory::Recursive_TypeCode::equivalent_i (
  CORBA::TypeCode_ptr tc) const
{
  if (CORBA::is_nil (this->the_typecode_))
    {
      // Nothing to compare yet.

      throw ::CORBA::BAD_TYPECODE (0, CORBA::COMPLETED_NO);
    }

  return this->the_typecode_->equivalent (tc);
}

CORBA::TypeCode_ptr
TAO::TypeCodeFactory::Recursive_TypeCode::get_compact_typecode_i (
  void) const
{
  if (CORBA::is_nil (this->the_typecode_))
    {
      // Nothing to compare yet.

      throw ::CORBA::BAD_TYPECODE (0, CORBA::COMPLETED_NO);
    }

  return this->the_typecode_->get_compact_typecode ();
}

char const *
TAO::TypeCodeFactory::Recursive_TypeCode::id_i (void) const
{
  // Ownership is retained by the TypeCode, as required by the C++
  // mapping.
  return this->id_.in ();
}

char const *
TAO::TypeCodeFactory::Recursive_TypeCode::name_i (void) const
{
  if (CORBA::is_nil (this->the_typecode_))
    {
      throw ::CORBA::BAD_TYPECODE (0, CORBA::COMPLETED_NO);
    }

  return this->the_typecode_->name ();
}

CORBA::ULong
TAO::TypeCodeFactory::Recursive_TypeCode::member_count_i (void) const
{
  if (CORBA::is_nil (this->the_typecode_))
    {
      throw ::CORBA::BAD_TYPECODE (0, CORBA::COMPLETED_NO);
    }

  return this->the_typecode_->member_count ();
}

char const *
TAO::TypeCodeFactory::Recursive_TypeCode::member_name_i (
  CORBA::ULong index) const
{
  if (CORBA::is_nil (this->the_typecode_))
    {
      throw ::CORBA::BAD_TYPECODE (0, CORBA::COMPLETED_NO);
    }

  return this->the_typecode_->member_name (index);
}

CORBA::TypeCode_ptr
TAO::TypeCodeFactory::Recursive_TypeCode::member_type_i (
  CORBA::ULong index) const
{
  if (CORBA::is_nil (this->the_typecode_))
    {
      throw ::CORBA::BAD_TYPECODE (0, CORBA::COMPLETED_NO);
    }

  return this->the_typecode_->member_type (index);
}

CORBA::Any *
TAO::TypeCodeFactory::Recursive_TypeCode::member_label_i (
  CORBA::ULong index) const
{
  if (CORBA::is_nil (this->the_typecode_))
    {
      throw ::CORBA::BAD_TYPECODE (0, CORBA::COMPLETED_NO);
    }

  return this->the_typecode_->member_label (index);
}

CORBA::TypeCode_ptr
TAO::TypeCodeFactory::Recursive_TypeCode::discriminator_type_i (
  void) const
{
  if (CORBA::is_nil (this->the_typecode_))
    {
      throw ::CORBA::BAD_TYPECODE (0, CORBA::COMPLETED_NO);
    }

  return this->the_typecode_->discriminator_type ();
}

CORBA::Long
TAO::TypeCodeFactory::Recursive_TypeCode::default_index_i (
  void) const
{
  if (CORBA::is_nil (this->the_typecode_))
    {
      throw ::CORBA::BAD_TYPECODE (0, CORBA::COMPLETED_NO);
    }

  return this->the_typecode_->default_index ();
}

CORBA::Visibility
TAO::TypeCodeFactory::Recursive_TypeCode::member_visibility_i (
  CORBA::ULong index) const
{
  if (CORBA::is_nil (this->the_typecode_))
    {
      throw ::CORBA::BAD_TYPECODE (0, CORBA::COMPLETED_NO);
    }

  return this->the_typecode_->member_visibility (index);
}

CORBA::ValueModifier
TAO::TypeCodeFactory::Recursive_TypeCode::type_modifier_i (void) const
{
  if (CORBA::is_nil (this->the_typecode_))
    {
      throw ::CORBA::BAD_TYPECODE (0, CORBA::COMPLETED_NO);
    }

  return this->the_typecode_->type_modifier ();
}

CORBA::TypeCode_ptr
TAO::TypeCodeFactory::Recursive_TypeCode::concrete_base_type_i (void) const
{
  if (CORBA::is_nil (this->the_typecode_))
    {
      throw ::CORBA::BAD_TYPECODE (0, CORBA::COMPLETED_NO);
    }

  return this->the_typecode_->concrete_base_type ();
}

TAO_END_VERSIONED_NAMESPACE_DECL
