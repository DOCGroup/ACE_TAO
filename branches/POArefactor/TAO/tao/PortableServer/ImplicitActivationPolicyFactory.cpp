// $Id$

#include "ImplicitActivationPolicyFactory.h"
#include "ace/Dynamic_Service.h"
#include "ImplicitActivationPolicy.h"

ACE_RCSID (PortableServer,
           ImplicitActivationPolicyFactory,
           "$Id$")

#if (TAO_HAS_MINIMUM_POA == 0)

namespace TAO
{
  namespace Portable_Server
  {
    ImplicitActivationPolicyFactory::~ImplicitActivationPolicyFactory (void)
    {
    }

    ::PortableServer::ImplicitActivationPolicy_ptr
    ImplicitActivationPolicyFactory::create (
      ::PortableServer::ImplicitActivationPolicyValue value)
    {
      ImplicitActivationPolicy* policy = 0;

      ACE_NEW_RETURN (policy,
                      ImplicitActivationPolicy,
                      ::PortableServer::ImplicitActivationPolicy::_nil ());

      (void) policy->init (value);

      return policy;
    }

    ::PortableServer::ImplicitActivationPolicy_ptr
    ImplicitActivationPolicyFactory::create (
      const CORBA::Any &value
      ACE_ENV_ARG_DECL)
        ACE_THROW_SPEC ((CORBA::PolicyError))
    {
      ImplicitActivationPolicy* policy = 0;

      ACE_NEW_RETURN (policy,
                      ImplicitActivationPolicy,
                      ::PortableServer::ImplicitActivationPolicy::_nil ());

      (void) policy->init (value ACE_ENV_ARG_PARAMETER);
      ACE_CHECK_RETURN (::PortableServer::ImplicitActivationPolicy::_nil ());

      return policy;
    }

    TAO_Cached_Policy_Type
    ImplicitActivationPolicyFactory::_tao_cached_type (void) const
    {
      return TAO_CACHED_POLICY_IMPLICIT_ACTIVATION;
    }

    TAO_Policy_Scope
    ImplicitActivationPolicyFactory::_tao_scope (void) const
    {
      return TAO_POLICY_POA_SCOPE;
    }

    ACE_STATIC_SVC_DEFINE (
        ImplicitActivationPolicyFactory,
        ACE_TEXT ("ImplicitActivationPolicyFactory"),
        ACE_SVC_OBJ_T,
        &ACE_SVC_NAME (ImplicitActivationPolicyFactory),
        ACE_Service_Type::DELETE_THIS | ACE_Service_Type::DELETE_OBJ,
        0
      )

    ACE_FACTORY_DEFINE (TAO_PortableServer, ImplicitActivationPolicyFactory)

    #if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)

    template class ACE_Dynamic_Service<ImplicitActivationPolicyFactory>;

    #elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)

    #pragma instantiate ACE_Dynamic_Service<ImplicitActivationPolicyFactory>

    #endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */
  }
}

#endif /* TAO_HAS_MINIMUM_POA == 0 */

