#include "tao/EndpointPolicy/EndpointPolicy_i.h"
#include "tao/SystemException.h"

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
  , value_ (rhs.value_)
{
}

CORBA::PolicyType
TAO_EndpointPolicy_i::policy_type ()
{
  return EndpointPolicy::ENDPOINT_POLICY_TYPE;
}

TAO_EndpointPolicy_i *
TAO_EndpointPolicy_i::clone () const
{
  TAO_EndpointPolicy_i *copy {};
  ACE_NEW_RETURN (copy,
                  TAO_EndpointPolicy_i (*this),
                  nullptr);
  return copy;
}

EndpointPolicy::EndpointList *
TAO_EndpointPolicy_i::value ()
{
  EndpointPolicy::EndpointList* list {};
  ACE_NEW_RETURN (list,
                  EndpointPolicy::EndpointList (this->value_),
                  nullptr);

  return list;
}

CORBA::Policy_ptr
TAO_EndpointPolicy_i::copy ()
{
  TAO_EndpointPolicy_i* servant {};
  ACE_NEW_THROW_EX (servant,
                    TAO_EndpointPolicy_i (*this),
                    CORBA::NO_MEMORY ());

  return servant;
}

void
TAO_EndpointPolicy_i::destroy ()
{
  this->value_.length (0);
}

TAO_Cached_Policy_Type
TAO_EndpointPolicy_i::_tao_cached_type () const
{
  return TAO_CACHED_POLICY_ENDPOINT;
}


TAO_END_VERSIONED_NAMESPACE_DECL
