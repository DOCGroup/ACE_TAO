// $Id$

#include "Loadable_Lifespan_Policy.h"
#include "ace/Dynamic_Service.h"
#include "POA_LifespanPolicy.h"

ACE_RCSID (PortableServer,
           Loadable_Lifespan_Policy,
           "$Id$")

namespace TAO
{
  Loadable_Lifespan_Policy::~Loadable_Lifespan_Policy (void)
  {
  }

  PortableServer::LifespanPolicy_ptr
  Loadable_Lifespan_Policy::create (
    PortableServer::LifespanPolicyValue value)
  {
    POA_LifespanPolicy* policy = 0;

    ACE_NEW_RETURN (policy,
                    POA_LifespanPolicy,
                    PortableServer::LifespanPolicy::_nil ());

    (void) policy->init (value);

    return policy;
  }

  PortableServer::LifespanPolicy_ptr
  Loadable_Lifespan_Policy::create (
    const CORBA::Any &value
    ACE_ENV_ARG_DECL)
      ACE_THROW_SPEC ((CORBA::PolicyError))
  {
    POA_LifespanPolicy* policy = 0;

    ACE_NEW_RETURN (policy,
                    POA_LifespanPolicy,
                    PortableServer::LifespanPolicy::_nil ());

    (void) policy->init (value ACE_ENV_ARG_PARAMETER);
    ACE_CHECK_RETURN (PortableServer::LifespanPolicy::_nil ());

    return policy;
  }

  TAO_Cached_Policy_Type
  Loadable_Lifespan_Policy::_tao_cached_type (void) const
  {
    return TAO_CACHED_POLICY_LIFESPAN;
  }

  TAO_Policy_Scope
  Loadable_Lifespan_Policy::_tao_scope (void) const
  {
    return TAO_POLICY_POA_SCOPE;
  }

  ACE_STATIC_SVC_DEFINE (
      Loadable_Lifespan_Policy,
      ACE_TEXT ("Loadable_Lifespan_Policy"),
      ACE_SVC_OBJ_T,
      &ACE_SVC_NAME (Loadable_Lifespan_Policy),
      ACE_Service_Type::DELETE_THIS | ACE_Service_Type::DELETE_OBJ,
      0
    )

  ACE_FACTORY_DEFINE (TAO_PortableServer, Loadable_Lifespan_Policy)

  #if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)

  template class ACE_Dynamic_Service<Loadable_Lifespan_Policy>;

  #elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)

  #pragma instantiate ACE_Dynamic_Service<Loadable_Lifespan_Policy>

  #endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */
}

