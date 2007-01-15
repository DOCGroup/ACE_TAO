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
    ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((
      CORBA::SystemException
    ))
{
  ACE_THROW_RETURN (CORBA::NO_IMPLEMENT (), false);
}
  
void
TAO_DynValueCommon_i::set_to_null (
    ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((
      CORBA::SystemException
    ))
{
  ACE_THROW (CORBA::NO_IMPLEMENT ());
}
  
void
TAO_DynValueCommon_i::set_to_value (
    ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((
      CORBA::SystemException
    ))
{
  ACE_THROW (CORBA::NO_IMPLEMENT ());
}

TAO_END_VERSIONED_NAMESPACE_DECL

