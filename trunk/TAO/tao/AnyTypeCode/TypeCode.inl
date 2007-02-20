// -*- C++ -*-
//
// $Id$

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
CORBA::TypeCode::kind (void) const
{
  return this->kind_;
}

ACE_INLINE CORBA::TypeCode_ptr
CORBA::TypeCode::get_compact_typecode (void) const
{
  return this->get_compact_typecode_i ();
}

ACE_INLINE char const *
CORBA::TypeCode::id (void) const
{
  return this->id_i ();
}

ACE_INLINE char const *
CORBA::TypeCode::name (void) const
{
  return this->name_i ();
}

ACE_INLINE CORBA::ULong
CORBA::TypeCode::member_count (void) const
{
  return this->member_count_i ();
}

ACE_INLINE char const *
CORBA::TypeCode::member_name (ULong index) const
{
  return this->member_name_i (index);
}

ACE_INLINE CORBA::TypeCode_ptr
CORBA::TypeCode::member_type (ULong index) const
{
  return this->member_type_i (index);
}

ACE_INLINE CORBA::Any *
CORBA::TypeCode::member_label (ULong index) const
{
  return this->member_label_i (index);
}

ACE_INLINE CORBA::TypeCode_ptr
CORBA::TypeCode::discriminator_type (void) const
{
  return this->discriminator_type_i ();
}

ACE_INLINE CORBA::Long
CORBA::TypeCode::default_index (void) const
{
  return this->default_index_i ();
}

ACE_INLINE CORBA::ULong
CORBA::TypeCode::length (void) const
{
  return this->length_i ();
}

ACE_INLINE CORBA::TypeCode_ptr
CORBA::TypeCode::content_type (void) const
{
  return this->content_type_i ();
}

ACE_INLINE CORBA::UShort
CORBA::TypeCode::fixed_digits (void) const
{
  return this->fixed_digits_i ();
}

ACE_INLINE CORBA::UShort
CORBA::TypeCode::fixed_scale (void) const
{
  return this->fixed_scale_i ();
}

ACE_INLINE CORBA::Visibility
CORBA::TypeCode::member_visibility (ULong index) const
{
  return this->member_visibility_i (index);
}

ACE_INLINE CORBA::ValueModifier
CORBA::TypeCode::type_modifier (void) const
{
  return this->type_modifier_i ();
}

ACE_INLINE CORBA::TypeCode_ptr
CORBA::TypeCode::concrete_base_type (void) const
{
  return this->concrete_base_type_i ();
}

ACE_INLINE void
CORBA::TypeCode::_tao_any_destructor (void * x)
{
  ::CORBA::release (static_cast <CORBA::TypeCode_ptr> (x));
}


TAO_END_VERSIONED_NAMESPACE_DECL
