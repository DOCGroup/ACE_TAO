// $Id$

#include "Loadable_Servant_Retention_Policy.h"
#include "ace/Dynamic_Service.h"
#include "POA_ServantRetentionPolicy.h"

ACE_RCSID (PortableServer,
           Loadable_Servant_Retention_Policy,
           "$Id$")

#if (TAO_HAS_MINIMUM_POA == 0)

namespace TAO
{
  Loadable_Servant_Retention_Policy::~Loadable_Servant_Retention_Policy (void)
  {
  }

  PortableServer::ServantRetentionPolicy_ptr
  Loadable_Servant_Retention_Policy::create (
    PortableServer::ServantRetentionPolicyValue value)
  {
    POA_Servant_RetentionPolicy* policy = 0;

    ACE_NEW_RETURN (policy,
                    POA_Servant_RetentionPolicy,
                    PortableServer::ServantRetentionPolicy::_nil ());

    (void) policy->init (value);

    return policy;
  }

  PortableServer::ServantRetentionPolicy_ptr
  Loadable_Servant_Retention_Policy::create (
    const CORBA::Any &value
    ACE_ENV_ARG_DECL)
      ACE_THROW_SPEC ((CORBA::PolicyError))
  {
    POA_Servant_RetentionPolicy* policy = 0;

    ACE_NEW_RETURN (policy,
                    POA_Servant_RetentionPolicy,
                    PortableServer::ServantRetentionPolicy::_nil ());

    (void) policy->init (value ACE_ENV_ARG_PARAMETER);
    ACE_CHECK_RETURN (PortableServer::ServantRetentionPolicy::_nil ());

    return policy;
  }

  TAO_Cached_Policy_Type
  Loadable_Servant_Retention_Policy::_tao_cached_type (void) const
  {
    return TAO_CACHED_POLICY_SERVANT_RETENTION;
  }

  TAO_Policy_Scope
  Loadable_Servant_Retention_Policy::_tao_scope (void) const
  {
    return TAO_POLICY_POA_SCOPE;
  }

  ACE_STATIC_SVC_DEFINE (
      Loadable_Servant_Retention_Policy,
      ACE_TEXT ("Loadable_Servant_Retention_Policy"),
      ACE_SVC_OBJ_T,
      &ACE_SVC_NAME (Loadable_Servant_Retention_Policy),
      ACE_Service_Type::DELETE_THIS | ACE_Service_Type::DELETE_OBJ,
      0
    )

  ACE_FACTORY_DEFINE (TAO_PortableServer, Loadable_Servant_Retention_Policy)

  #if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)

  template class ACE_Dynamic_Service<Loadable_Servant_Retention_Policy>;

  #elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)

  #pragma instantiate ACE_Dynamic_Service<Loadable_Servant_Retention_Policy>

  #endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */
}

#endif /* TAO_HAS_MINIMUM_POA == 0 */

