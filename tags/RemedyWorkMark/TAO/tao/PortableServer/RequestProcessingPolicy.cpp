// -*- C++ -*-
// $Id$

#include "tao/orbconf.h"

#if (TAO_HAS_MINIMUM_POA == 0) && !defined (CORBA_E_COMPACT) && !defined (CORBA_E_MICRO)

#include "tao/PortableServer/RequestProcessingPolicy.h"
#include "tao/PortableServer/PortableServer.h"

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
    RequestProcessingPolicy::copy (void)
    {
      RequestProcessingPolicy *copy = 0;
      ACE_NEW_THROW_EX (copy,
                        RequestProcessingPolicy (this->value_),
                        CORBA::NO_MEMORY ());

      return copy;
    }

    void
    RequestProcessingPolicy::destroy (void)
    {
    }

    ::PortableServer::RequestProcessingPolicyValue
    RequestProcessingPolicy::value (void)
    {
      return value_;
    }

    CORBA::PolicyType
    RequestProcessingPolicy::policy_type (void)
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
