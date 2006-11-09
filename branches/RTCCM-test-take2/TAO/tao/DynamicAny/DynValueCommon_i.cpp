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
  return true;
}
  
void
TAO_DynValueCommon_i::set_to_null (
    ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((
      CORBA::SystemException
    ))
{
}
  
void
TAO_DynValueCommon_i::set_to_value (
    ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((
      CORBA::SystemException
    ))
{
}

TAO_END_VERSIONED_NAMESPACE_DECL

