// -*- C++ -*-
// $Id$

#include "tao/DynamicAny/DynValueBox_i.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

TAO_DynValueBox_i::TAO_DynValueBox_i (void)
{
}

TAO_DynValueBox_i::~TAO_DynValueBox_i (void)
{
}

CORBA::Any *
TAO_DynValueBox_i::get_boxed_value (void)
{
  throw ::CORBA::NO_IMPLEMENT ();
}

void
TAO_DynValueBox_i::set_boxed_value (const CORBA::Any & /* boxed */)
{
  throw ::CORBA::NO_IMPLEMENT ();
}

DynamicAny::DynAny_ptr
TAO_DynValueBox_i::get_boxed_value_as_dyn_any (void)
{
  throw ::CORBA::NO_IMPLEMENT ();
}

void
TAO_DynValueBox_i::set_boxed_value_as_dyn_any (DynamicAny::DynAny_ptr /* boxed */)
{
  throw ::CORBA::NO_IMPLEMENT ();
}

TAO_END_VERSIONED_NAMESPACE_DECL

