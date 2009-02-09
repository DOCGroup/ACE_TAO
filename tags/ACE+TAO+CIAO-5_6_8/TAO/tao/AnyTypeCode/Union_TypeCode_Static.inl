// -*- C++ -*-
//
// $Id$

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

ACE_INLINE
TAO::TypeCode::Union<char const *,
                     CORBA::TypeCode_ptr const *,
                     TAO::TypeCode::Case<char const *,
                                         CORBA::TypeCode_ptr const *> const * const *,
                     TAO::Null_RefCount_Policy>::Union (
  char const * id,
  char const * name,
  CORBA::TypeCode_ptr const * discriminant_type,
  Case<char const *, CORBA::TypeCode_ptr const *> const * const * cases,
  CORBA::ULong ncases,
  CORBA::Long default_index)
  : ::CORBA::TypeCode (CORBA::tk_union)
  , ::TAO::Null_RefCount_Policy ()
  , base_attributes_ (id, name)
  , discriminant_type_ (discriminant_type)
  , default_index_ (default_index)
  , ncases_ (ncases)
  , cases_ (cases)
{
}

TAO_END_VERSIONED_NAMESPACE_DECL
