// $Id$

#include "tao/DynamicAny/DynValueCommon_i.h"

ACE_RCSID (DynamicAny,
           DynValueCommon_i,
           "$Id$")

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

TAO_DynValueCommon_i::TAO_DynValueCommon_i (void)
{
}

TAO_DynValueCommon_i::~TAO_DynValueCommon_i (void)
{
}

CORBA::Boolean
TAO_DynValueCommon_i::is_null (
    void)
  ACE_THROW_SPEC ((
      CORBA::SystemException
    ))
{
  throw ::CORBA::NO_IMPLEMENT ();
}

void
TAO_DynValueCommon_i::set_to_null (
    void)
  ACE_THROW_SPEC ((
      CORBA::SystemException
    ))
{
  throw ::CORBA::NO_IMPLEMENT ();
}

void
TAO_DynValueCommon_i::set_to_value (
    void)
  ACE_THROW_SPEC ((
      CORBA::SystemException
    ))
{
  throw ::CORBA::NO_IMPLEMENT ();
}

TAO_END_VERSIONED_NAMESPACE_DECL

