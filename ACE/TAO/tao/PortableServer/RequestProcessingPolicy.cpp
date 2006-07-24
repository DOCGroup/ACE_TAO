#include "tao/PortableServer/RequestProcessingPolicy.h"
#include "tao/PortableServer/PortableServer.h"

ACE_RCSID (PortableServer,
           RequestProcessingPolicy,
           "$Id$")

#if (TAO_HAS_MINIMUM_POA == 0) && !defined (CORBA_E_COMPACT) && !defined (CORBA_E_MICRO)

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

namespace TAO
{
  namespace Portable_Server
  {
    RequestProcessingPolicy::RequestProcessingPolicy (
      ::PortableServer::RequestProcessingPolicyValue value) :
        value_ (value)
    {
    }

    CORBA::Policy_ptr
    RequestProcessingPolicy::copy (ACE_ENV_SINGLE_ARG_DECL)
      ACE_THROW_SPEC ((CORBA::SystemException))
    {
      RequestProcessingPolicy *copy = 0;
      ACE_NEW_THROW_EX (copy,
                        RequestProcessingPolicy (this->value_),
                        CORBA::NO_MEMORY ());
      ACE_CHECK_RETURN (CORBA::Policy::_nil ());

      return copy;
    }

    void
    RequestProcessingPolicy::destroy (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
      ACE_THROW_SPEC ((CORBA::SystemException))
    {
    }

    ::PortableServer::RequestProcessingPolicyValue
    RequestProcessingPolicy::value (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
      ACE_THROW_SPEC ((CORBA::SystemException))
    {
      return value_;
    }

    CORBA::PolicyType
    RequestProcessingPolicy::policy_type (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
      ACE_THROW_SPEC ((CORBA::SystemException))
    {
      return ::PortableServer::REQUEST_PROCESSING_POLICY_ID;
    }

    TAO_Cached_Policy_Type
    RequestProcessingPolicy::_tao_cached_type (void) const
    {
      return TAO_CACHED_POLICY_REQUEST_PROCESSING;
    }

    TAO_Policy_Scope
    RequestProcessingPolicy::_tao_scope (void) const
    {
      return TAO_POLICY_POA_SCOPE;
    }


  }
}

TAO_END_VERSIONED_NAMESPACE_DECL

#endif /* TAO_HAS_MINIMUM_POA == 0 */
