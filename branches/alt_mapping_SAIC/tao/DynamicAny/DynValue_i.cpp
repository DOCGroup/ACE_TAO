// -*- C++ -*-
// $Id$

#include "tao/DynamicAny/DynValue_i.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

TAO_DynValue_i::TAO_DynValue_i (void)
{
}

TAO_DynValue_i::~TAO_DynValue_i (void)
{
}

char *
TAO_DynValue_i::current_member_name (void)
{
  throw ::CORBA::NO_IMPLEMENT ();
}

CORBA::TCKind
TAO_DynValue_i::current_member_kind (void)
{
  throw ::CORBA::NO_IMPLEMENT ();
}

DynamicAny::NameValuePairSeq *
TAO_DynValue_i::get_members (void)
{
  throw ::CORBA::NO_IMPLEMENT ();
}

void
TAO_DynValue_i::set_members (const DynamicAny::NameValuePairSeq & /* value */)
{
  throw ::CORBA::NO_IMPLEMENT ();
}

DynamicAny::NameDynAnyPairSeq *
TAO_DynValue_i::get_members_as_dyn_any (void)
{
  throw ::CORBA::NO_IMPLEMENT ();
}

void
TAO_DynValue_i::set_members_as_dyn_any (
    const DynamicAny::NameDynAnyPairSeq & /* value */)
{
  throw ::CORBA::NO_IMPLEMENT ();
}

TAO_END_VERSIONED_NAMESPACE_DECL

