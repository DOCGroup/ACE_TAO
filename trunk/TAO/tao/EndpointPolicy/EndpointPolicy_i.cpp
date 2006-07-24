// $Id$

#include "tao/EndpointPolicy/EndpointPolicy_i.h"

#include "tao/SystemException.h"

ACE_RCSID (EndpointPolicy,
           EndpointPolicy_i,
           "$Id$")


TAO_BEGIN_VERSIONED_NAMESPACE_DECL

TAO_EndpointPolicy_i::TAO_EndpointPolicy_i (const EndpointPolicy::EndpointList &value)
: value_ (value)
{
}

TAO_EndpointPolicy_i::TAO_EndpointPolicy_i (const TAO_EndpointPolicy_i &rhs)
  : ::CORBA::Object ()
  , ::CORBA::Policy ()
  , ::EndpointPolicy::Policy ()
  , ::CORBA::LocalObject ()
  , TAO_Local_RefCounted_Object ()
  , value_ (rhs.value_)
{
}

CORBA::PolicyType
TAO_EndpointPolicy_i::policy_type (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  return EndpointPolicy::ENDPOINT_POLICY_TYPE;
}

TAO_EndpointPolicy_i *
TAO_EndpointPolicy_i::clone (void) const
{
  TAO_EndpointPolicy_i *copy = 0;
  ACE_NEW_RETURN (copy,
                  TAO_EndpointPolicy_i (*this),
                  0);
  return copy;
}

EndpointPolicy::EndpointList *
TAO_EndpointPolicy_i::value (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  EndpointPolicy::EndpointList* list = 0;
  ACE_NEW_RETURN (list,
                  EndpointPolicy::EndpointList (this->value_),
                  0);

  return list;
}

CORBA::Policy_ptr
TAO_EndpointPolicy_i::copy (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  TAO_EndpointPolicy_i* servant = 0;
  ACE_NEW_THROW_EX (servant,
                    TAO_EndpointPolicy_i (*this),
                    CORBA::NO_MEMORY ());
  ACE_CHECK_RETURN (CORBA::Policy::_nil ());

  return servant;
}

void
TAO_EndpointPolicy_i::destroy (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
}

TAO_Cached_Policy_Type
TAO_EndpointPolicy_i::_tao_cached_type (void) const
{
  return TAO_CACHED_POLICY_ENDPOINT;
}


TAO_END_VERSIONED_NAMESPACE_DECL
