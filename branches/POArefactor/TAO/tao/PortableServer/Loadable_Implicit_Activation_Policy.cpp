// $Id$

#include "Loadable_Implicit_Activation_Policy.h"
#include "ace/Dynamic_Service.h"
#include "POA_Implicit_ActivationPolicy.h"

ACE_RCSID (PortableServer,
           Loadable_Implicit_Activation_Policy,
           "$Id$")

#if (TAO_HAS_MINIMUM_POA == 0)

namespace TAO
{
  Loadable_Implicit_Activation_Policy::~Loadable_Implicit_Activation_Policy (void)
  {
  }

  PortableServer::ImplicitActivationPolicy_ptr
  Loadable_Implicit_Activation_Policy::create (
    PortableServer::ImplicitActivationPolicyValue value)
  {
    POA_Implicit_ActivationPolicy* policy = 0;

    ACE_NEW_RETURN (policy,
                    POA_Implicit_ActivationPolicy,
                    PortableServer::ImplicitActivationPolicy::_nil ());

    (void) policy->init (value);

    return policy;
  }

  PortableServer::ImplicitActivationPolicy_ptr
  Loadable_Implicit_Activation_Policy::create (
    const CORBA::Any &value
    ACE_ENV_ARG_DECL)
      ACE_THROW_SPEC ((CORBA::PolicyError))
  {
    POA_Implicit_ActivationPolicy* policy = 0;

    ACE_NEW_RETURN (policy,
                    POA_Implicit_ActivationPolicy,
                    PortableServer::ImplicitActivationPolicy::_nil ());

    (void) policy->init (value ACE_ENV_ARG_PARAMETER);
    ACE_CHECK_RETURN (PortableServer::ImplicitActivationPolicy::_nil ());

    return policy;
  }

  TAO_Cached_Policy_Type
  Loadable_Implicit_Activation_Policy::_tao_cached_type (void) const
  {
    return TAO_CACHED_POLICY_IMPLICIT_ACTIVATION;
  }

  TAO_Policy_Scope
  Loadable_Implicit_Activation_Policy::_tao_scope (void) const
  {
    return TAO_POLICY_POA_SCOPE;
  }

  ACE_STATIC_SVC_DEFINE (
      Loadable_Implicit_Activation_Policy,
      ACE_TEXT ("Loadable_Implicit_Activation_Policy"),
      ACE_SVC_OBJ_T,
      &ACE_SVC_NAME (Loadable_Implicit_Activation_Policy),
      ACE_Service_Type::DELETE_THIS | ACE_Service_Type::DELETE_OBJ,
      0
    )

  ACE_FACTORY_DEFINE (TAO_PortableServer, Loadable_Implicit_Activation_Policy)

  #if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)

  template class ACE_Dynamic_Service<Loadable_Implicit_Activation_Policy>;

  #elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)

  #pragma instantiate ACE_Dynamic_Service<Loadable_Implicit_Activation_Policy>

  #endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */
}

#endif /* TAO_HAS_MINIMUM_POA == 0 */

