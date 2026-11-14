// -*- C++ -*-
TAO_BEGIN_VERSIONED_NAMESPACE_DECL

ACE_INLINE
TAO::TypeCode::Indirected_Type::Indirected_Type (
  CORBA::TCKind kind,
  char const * id)
  : ::CORBA::TypeCode (kind)
  , id_ (id)
{}

ACE_INLINE
TAO::TypeCode::Indirected_Type::Indirected_Type (
  char const * id)
  : ::CORBA::TypeCode (CORBA::TAO_TC_KIND_COUNT)
  , id_ (id)
{}

TAO_END_VERSIONED_NAMESPACE_DECL
