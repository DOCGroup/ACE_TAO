// -*- C++ -*-
//
// $Id$

#include "tao/Environment.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

ACE_INLINE CORBA::Boolean
CORBA::is_nil (CORBA::TypeCode_ptr obj)
{
  return obj == 0;
}

ACE_INLINE void
CORBA::release (CORBA::TypeCode_ptr obj)
{
  if (obj)
    obj->tao_release ();
}

// --------------------------------------------------------------

ACE_INLINE
CORBA::TypeCode::TypeCode (CORBA::TCKind k)
  : kind_ (k)
{
}

ACE_INLINE CORBA::TypeCode_ptr
CORBA::TypeCode::_duplicate (CORBA::TypeCode_ptr tc)
{
  if (!CORBA::is_nil (tc))
    tc->tao_duplicate ();

  return tc;
}

ACE_INLINE CORBA::TypeCode_ptr
CORBA::TypeCode::_nil (void)
{
  return static_cast<CORBA::TypeCode_ptr> (0);
}

ACE_INLINE CORBA::TCKind
CORBA::TypeCode::kind (ACE_ENV_SINGLE_ARG_DECL_NOT_USED) const
{
  return this->kind_;
}

ACE_INLINE CORBA::TypeCode_ptr
CORBA::TypeCode::get_compact_typecode (ACE_ENV_SINGLE_ARG_DECL) const
{
  return this->get_compact_typecode_i (ACE_ENV_SINGLE_ARG_PARAMETER);
}

ACE_INLINE char const *
CORBA::TypeCode::id (ACE_ENV_SINGLE_ARG_DECL) const
{
  return this->id_i (ACE_ENV_SINGLE_ARG_PARAMETER);
}

ACE_INLINE char const *
CORBA::TypeCode::name (ACE_ENV_SINGLE_ARG_DECL) const
{
  return this->name_i (ACE_ENV_SINGLE_ARG_PARAMETER);
}

ACE_INLINE CORBA::ULong
CORBA::TypeCode::member_count (ACE_ENV_SINGLE_ARG_DECL) const
{
  return this->member_count_i (ACE_ENV_SINGLE_ARG_PARAMETER);
}

ACE_INLINE char const *
CORBA::TypeCode::member_name (ULong index
                              ACE_ENV_ARG_DECL) const
{
  return this->member_name_i (index
                              ACE_ENV_ARG_PARAMETER);
}

ACE_INLINE CORBA::TypeCode_ptr
CORBA::TypeCode::member_type (ULong index
                              ACE_ENV_ARG_DECL) const
{
  return this->member_type_i (index
                              ACE_ENV_ARG_PARAMETER);
}

ACE_INLINE CORBA::Any *
CORBA::TypeCode::member_label (ULong index
                               ACE_ENV_ARG_DECL) const
{
  return this->member_label_i (index
                               ACE_ENV_ARG_PARAMETER);
}

ACE_INLINE CORBA::TypeCode_ptr
CORBA::TypeCode::discriminator_type (ACE_ENV_SINGLE_ARG_DECL) const
{
  return this->discriminator_type_i (ACE_ENV_SINGLE_ARG_PARAMETER);
}

ACE_INLINE CORBA::Long
CORBA::TypeCode::default_index (ACE_ENV_SINGLE_ARG_DECL) const
{
  return this->default_index_i (ACE_ENV_SINGLE_ARG_PARAMETER);
}

ACE_INLINE CORBA::ULong
CORBA::TypeCode::length (ACE_ENV_SINGLE_ARG_DECL) const
{
  return this->length_i (ACE_ENV_SINGLE_ARG_PARAMETER);
}

ACE_INLINE CORBA::TypeCode_ptr
CORBA::TypeCode::content_type (ACE_ENV_SINGLE_ARG_DECL) const
{
  return this->content_type_i (ACE_ENV_SINGLE_ARG_PARAMETER);
}

ACE_INLINE CORBA::UShort
CORBA::TypeCode::fixed_digits (ACE_ENV_SINGLE_ARG_DECL) const
{
  return this->fixed_digits_i (ACE_ENV_SINGLE_ARG_PARAMETER);
}

ACE_INLINE CORBA::UShort
CORBA::TypeCode::fixed_scale (ACE_ENV_SINGLE_ARG_DECL) const
{
  return this->fixed_scale_i (ACE_ENV_SINGLE_ARG_PARAMETER);
}

ACE_INLINE CORBA::Visibility
CORBA::TypeCode::member_visibility (ULong index
                                    ACE_ENV_ARG_DECL) const
{
  return this->member_visibility_i (index
                                    ACE_ENV_ARG_PARAMETER);
}

ACE_INLINE CORBA::ValueModifier
CORBA::TypeCode::type_modifier (ACE_ENV_SINGLE_ARG_DECL) const
{
  return this->type_modifier_i (ACE_ENV_SINGLE_ARG_PARAMETER);
}

ACE_INLINE CORBA::TypeCode_ptr
CORBA::TypeCode::concrete_base_type (ACE_ENV_SINGLE_ARG_DECL) const
{
  return this->concrete_base_type_i (ACE_ENV_SINGLE_ARG_PARAMETER);
}

ACE_INLINE void
CORBA::TypeCode::_tao_any_destructor (void * x)
{
  ::CORBA::release (static_cast <CORBA::TypeCode_ptr> (x));
}


TAO_END_VERSIONED_NAMESPACE_DECL
