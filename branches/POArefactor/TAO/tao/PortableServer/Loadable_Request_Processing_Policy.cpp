// $Id$

#include "Loadable_Request_Processing_Policy.h"
#include "ace/Dynamic_Service.h"
#include "POA_Request_Processing_Policy.h"

ACE_RCSID (PortableServer,
           Loadable_Request_Processing_Policy,
           "$Id$")

#if (TAO_HAS_MINIMUM_POA == 0)

namespace TAO
{
  Loadable_Request_Processing_Policy::~Loadable_Request_Processing_Policy (void)
  {
  }

  PortableServer::RequestProcessingPolicy_ptr
  Loadable_Request_Processing_Policy::create (
    PortableServer::RequestProcessingPolicyValue value)
  {
    POA_Request_Processing_Policy* policy = 0;

    ACE_NEW_RETURN (policy,
                    POA_Request_Processing_Policy,
                    PortableServer::RequestProcessingPolicy::_nil ());

    (void) policy->init (value);

    return policy;
  }

  PortableServer::RequestProcessingPolicy_ptr
  Loadable_Request_Processing_Policy::create (
    const CORBA::Any &value
    ACE_ENV_ARG_DECL)
      ACE_THROW_SPEC ((CORBA::PolicyError))
  {
    POA_Request_Processing_Policy* policy = 0;

    ACE_NEW_RETURN (policy,
                    POA_Request_Processing_Policy,
                    PortableServer::RequestProcessingPolicy::_nil ());

    (void) policy->init (value ACE_ENV_ARG_PARAMETER);
    ACE_CHECK_RETURN (PortableServer::RequestProcessingPolicy::_nil ());

    return policy;
  }

  TAO_Cached_Policy_Type
  Loadable_Request_Processing_Policy::_tao_cached_type (void) const
  {
    return TAO_CACHED_POLICY_REQUEST_PROCESSING;
  }

  TAO_Policy_Scope
  Loadable_Request_Processing_Policy::_tao_scope (void) const
  {
    return TAO_POLICY_POA_SCOPE;
  }

  ACE_STATIC_SVC_DEFINE (
      Loadable_Request_Processing_Policy,
      ACE_TEXT ("Loadable_Request_Processing_Policy"),
      ACE_SVC_OBJ_T,
      &ACE_SVC_NAME (Loadable_Request_Processing_Policy),
      ACE_Service_Type::DELETE_THIS | ACE_Service_Type::DELETE_OBJ,
      0
    )

  ACE_FACTORY_DEFINE (TAO_PortableServer, Loadable_Request_Processing_Policy)

  #if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)

  template class ACE_Dynamic_Service<Loadable_Request_Processing_Policy>;

  #elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)

  #pragma instantiate ACE_Dynamic_Service<Loadable_Request_Processing_Policy>

  #endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */
}

#endif /* TAO_HAS_MINIMUM_POA == 0 */

