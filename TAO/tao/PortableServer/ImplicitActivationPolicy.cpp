#include "tao/PortableServer/ImplicitActivationPolicy.h"
#include "tao/PortableServer/PortableServer.h"

ACE_RCSID (PortableServer,
           ImplicitActivationPolicy,
           "$Id$")

#if (TAO_HAS_MINIMUM_POA == 0) && !defined (CORBA_E_COMPACT) && !defined (CORBA_E_MICRO)

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

namespace TAO
{
  namespace Portable_Server
  {
    ImplicitActivationPolicy::ImplicitActivationPolicy (
      ::PortableServer::ImplicitActivationPolicyValue value) :
        value_ (value)
    {
    }

    CORBA::Policy_ptr
    ImplicitActivationPolicy::copy (ACE_ENV_SINGLE_ARG_DECL)
      ACE_THROW_SPEC ((CORBA::SystemException))
    {
      ImplicitActivationPolicy *copy = 0;
      ACE_NEW_THROW_EX (copy,
                        ImplicitActivationPolicy (this->value_),
                        CORBA::NO_MEMORY ());
      ACE_CHECK_RETURN (CORBA::Policy::_nil ());

      return copy;
    }

    void
    ImplicitActivationPolicy::destroy (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
      ACE_THROW_SPEC ((CORBA::SystemException))
    {
    }

    ::PortableServer::ImplicitActivationPolicyValue
    ImplicitActivationPolicy::value (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
      ACE_THROW_SPEC ((CORBA::SystemException))
    {
      return value_;
    }

    CORBA::PolicyType
    ImplicitActivationPolicy::policy_type (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
      ACE_THROW_SPEC ((CORBA::SystemException))
    {
      return ::PortableServer::IMPLICIT_ACTIVATION_POLICY_ID;
    }

    TAO_Cached_Policy_Type
    ImplicitActivationPolicy::_tao_cached_type (void) const
    {
      return TAO_CACHED_POLICY_IMPLICIT_ACTIVATION;
    }

    TAO_Policy_Scope
    ImplicitActivationPolicy::_tao_scope (void) const
    {
      return TAO_POLICY_POA_SCOPE;
    }
  }
}

TAO_END_VERSIONED_NAMESPACE_DECL

#endif /* TAO_HAS_MINIMUM_POA == 0 && !CORBA_E_COMPACT && !CORBA_E_MICRO */
