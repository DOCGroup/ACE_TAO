// $Id$

#include "tao/TypeCodeFactory/Recursive_TypeCode.h"

#ifndef __ACE_INLINE__
# include "tao/TypeCodeFactory/Recursive_TypeCode.inl"
#endif  /* __ACE_INLINE__ */

#include "tao/SystemException.h"
#include "tao/ORB_Constants.h"


ACE_RCSID (TypeCodeFactory,
           Recursive_TypeCode,
           "$Id$")

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

bool
TAO::TypeCodeFactory::Recursive_TypeCode::tao_marshal (
  TAO_OutputCDR & cdr,
  CORBA::ULong offset) const
{
  ACE_DECLARE_NEW_CORBA_ENV;

  if (CORBA::is_nil (this->the_typecode_.in ()))
    {
      // Marshaling an incomplete recursive TypeCode placeholder is
      // not allowed.

      ACE_THROW_RETURN (CORBA::BAD_TYPECODE (CORBA::OMGVMCID | 1,
                                             CORBA::COMPLETED_NO),
                        false);
    }

  return this->the_typecode_->tao_marshal (cdr, offset);
}

bool
TAO::TypeCodeFactory::Recursive_TypeCode::tao_marshal_kind (
  TAO_OutputCDR & cdr) const
{
  ACE_DECLARE_NEW_CORBA_ENV;

  if (CORBA::is_nil (this->the_typecode_.in ()))
    {
      // Marshaling an incomplete recursive TypeCode placeholder is
      // not allowed.

      ACE_THROW_RETURN (CORBA::BAD_TYPECODE (CORBA::OMGVMCID | 1,
                                             CORBA::COMPLETED_NO),
                        false);
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
TAO::TypeCodeFactory::Recursive_TypeCode::equal_i (CORBA::TypeCode_ptr tc
                                                   ACE_ENV_ARG_DECL) const
{
  if (CORBA::is_nil (this->the_typecode_.in ()))
    {
      // Nothing to compare yet.

      ACE_THROW_RETURN (CORBA::BAD_TYPECODE (0,
                                             CORBA::COMPLETED_NO),
                        false);
    }

  return this->the_typecode_->equal (tc
                                     ACE_ENV_ARG_PARAMETER);
}

CORBA::Boolean
TAO::TypeCodeFactory::Recursive_TypeCode::equivalent_i (CORBA::TypeCode_ptr tc
                                                        ACE_ENV_ARG_DECL) const
{
  if (CORBA::is_nil (this->the_typecode_.in ()))
    {
      // Nothing to compare yet.

      ACE_THROW_RETURN (CORBA::BAD_TYPECODE (0,
                                             CORBA::COMPLETED_NO),
                        false);
    }

  return this->the_typecode_->equivalent (tc
                                          ACE_ENV_ARG_PARAMETER);
}

CORBA::TypeCode_ptr
TAO::TypeCodeFactory::Recursive_TypeCode::get_compact_typecode_i (
  ACE_ENV_SINGLE_ARG_DECL) const
{
  if (CORBA::is_nil (this->the_typecode_.in ()))
    {
      // Nothing to compare yet.

      ACE_THROW_RETURN (CORBA::BAD_TYPECODE (0,
                                             CORBA::COMPLETED_NO),
                        CORBA::TypeCode::_nil ());
    }

  return
    this->the_typecode_->get_compact_typecode (ACE_ENV_SINGLE_ARG_PARAMETER);
}

char const *
TAO::TypeCodeFactory::Recursive_TypeCode::id_i (
  ACE_ENV_SINGLE_ARG_DECL_NOT_USED) const
{
  // Ownership is retained by the TypeCode, as required by the C++
  // mapping.
  return this->id_.in ();
}

char const *
TAO::TypeCodeFactory::Recursive_TypeCode::name_i (
  ACE_ENV_SINGLE_ARG_DECL) const
{
  if (CORBA::is_nil (this->the_typecode_.in ()))
    {
      ACE_THROW_RETURN (CORBA::BAD_TYPECODE (0,
                                             CORBA::COMPLETED_NO),
                        0);
    }

  return this->the_typecode_->name (ACE_ENV_SINGLE_ARG_PARAMETER);
}

CORBA::ULong
TAO::TypeCodeFactory::Recursive_TypeCode::member_count_i (
  ACE_ENV_SINGLE_ARG_DECL) const
{
  if (CORBA::is_nil (this->the_typecode_.in ()))
    {
      ACE_THROW_RETURN (CORBA::BAD_TYPECODE (0,
                                             CORBA::COMPLETED_NO),
                        0);
    }

  return this->the_typecode_->member_count (ACE_ENV_SINGLE_ARG_PARAMETER);
}

char const *
TAO::TypeCodeFactory::Recursive_TypeCode::member_name_i (
  CORBA::ULong index
  ACE_ENV_ARG_DECL) const
{
  if (CORBA::is_nil (this->the_typecode_.in ()))
    {
      ACE_THROW_RETURN (CORBA::BAD_TYPECODE (0,
                                             CORBA::COMPLETED_NO),
                        0);
    }

  return this->the_typecode_->member_name (index
                                       ACE_ENV_ARG_PARAMETER);
}

CORBA::TypeCode_ptr
TAO::TypeCodeFactory::Recursive_TypeCode::member_type_i (
  CORBA::ULong index
  ACE_ENV_ARG_DECL) const
{
  if (CORBA::is_nil (this->the_typecode_.in ()))
    {
      ACE_THROW_RETURN (CORBA::BAD_TYPECODE (0,
                                             CORBA::COMPLETED_NO),
                        CORBA::TypeCode::_nil ());
    }

  return this->the_typecode_->member_type (index
                                           ACE_ENV_ARG_PARAMETER);
}

CORBA::Any *
TAO::TypeCodeFactory::Recursive_TypeCode::member_label_i (
  CORBA::ULong index
  ACE_ENV_ARG_DECL) const
{
  if (CORBA::is_nil (this->the_typecode_.in ()))
    {
      ACE_THROW_RETURN (CORBA::BAD_TYPECODE (0,
                                             CORBA::COMPLETED_NO),
                        0);
    }

  return
    this->the_typecode_->member_label (index
                                       ACE_ENV_ARG_PARAMETER);
}

CORBA::TypeCode_ptr
TAO::TypeCodeFactory::Recursive_TypeCode::discriminator_type_i (
  ACE_ENV_SINGLE_ARG_DECL) const
{
  if (CORBA::is_nil (this->the_typecode_.in ()))
    {
      ACE_THROW_RETURN (CORBA::BAD_TYPECODE (0,
                                             CORBA::COMPLETED_NO),
                        CORBA::TypeCode::_nil ());
    }

  return
    this->the_typecode_->discriminator_type (ACE_ENV_SINGLE_ARG_PARAMETER);
}

CORBA::Long
TAO::TypeCodeFactory::Recursive_TypeCode::default_index_i (
  ACE_ENV_SINGLE_ARG_DECL) const
{
  if (CORBA::is_nil (this->the_typecode_.in ()))
    {
      ACE_THROW_RETURN (CORBA::BAD_TYPECODE (0,
                                             CORBA::COMPLETED_NO),
                        0);
    }

  return this->the_typecode_->default_index (ACE_ENV_SINGLE_ARG_PARAMETER);
}

CORBA::Visibility
TAO::TypeCodeFactory::Recursive_TypeCode::member_visibility_i (
  CORBA::ULong index
  ACE_ENV_ARG_DECL) const
{
  if (CORBA::is_nil (this->the_typecode_.in ()))
    {
      ACE_THROW_RETURN (CORBA::BAD_TYPECODE (0,
                                             CORBA::COMPLETED_NO),
                        CORBA::PRIVATE_MEMBER);
    }

  return this->the_typecode_->member_visibility (index
                                                 ACE_ENV_ARG_PARAMETER);
}

CORBA::ValueModifier
TAO::TypeCodeFactory::Recursive_TypeCode::type_modifier_i (
  ACE_ENV_SINGLE_ARG_DECL) const
{
  if (CORBA::is_nil (this->the_typecode_.in ()))
    {
      ACE_THROW_RETURN (CORBA::BAD_TYPECODE (0,
                                             CORBA::COMPLETED_NO),
                        CORBA::VM_NONE);
    }

  return this->the_typecode_->type_modifier (ACE_ENV_SINGLE_ARG_PARAMETER);
}

CORBA::TypeCode_ptr
TAO::TypeCodeFactory::Recursive_TypeCode::concrete_base_type_i (
  ACE_ENV_SINGLE_ARG_DECL) const
{
  if (CORBA::is_nil (this->the_typecode_.in ()))
    {
      ACE_THROW_RETURN (CORBA::BAD_TYPECODE (0,
                                             CORBA::COMPLETED_NO),
                        CORBA::TypeCode::_nil ());
    }

  return
    this->the_typecode_->concrete_base_type (ACE_ENV_SINGLE_ARG_PARAMETER);
}

TAO_END_VERSIONED_NAMESPACE_DECL
