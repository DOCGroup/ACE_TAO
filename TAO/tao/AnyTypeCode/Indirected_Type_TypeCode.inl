// -*- C++ -*-
//
// $Id$

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

ACE_INLINE
TAO::TypeCode::Indirected_Type::Indirected_Type (
  CORBA::TCKind kind,
  char const * id)
  : ::CORBA::TypeCode (kind)
  , id_ (id)
  , recursive_tc_ (0)
  , refcount_ (1)
{
  // CORBA::tk_except is not allowed in the recursive TypeCode case.
  // ACE_ASSERT (kind != CORBA::tk_except);
}

ACE_INLINE
TAO::TypeCode::Indirected_Type::Indirected_Type (
  char const * id)
  : ::CORBA::TypeCode (CORBA::TAO_TC_KIND_COUNT)
  , id_ (id)
  , recursive_tc_ (0)
  , refcount_ (1)
{
  // CORBA::tk_except is not allowed in the recursive TypeCode case.
  // ACE_ASSERT (kind != CORBA::tk_except);
}

TAO_END_VERSIONED_NAMESPACE_DECL
