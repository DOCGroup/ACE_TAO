// $Id$

#include "Loadable_Thread_Policy.h"
#include "ace/Dynamic_Service.h"
#include "POA_ThreadPolicy.h"

ACE_RCSID (PortableServer,
           Loadable_Thread_Policy,
           "$Id$")

namespace TAO
{
  Loadable_Thread_Policy::~Loadable_Thread_Policy (void)
  {
  }

  PortableServer::ThreadPolicy_ptr
  Loadable_Thread_Policy::create (
    PortableServer::ThreadPolicyValue value)
  {
    POA_ThreadPolicy* policy = 0;

    ACE_NEW_RETURN (policy,
                    POA_ThreadPolicy,
                    PortableServer::ThreadPolicy::_nil ());

    (void) policy->init (value);

    return policy;
  }

  PortableServer::ThreadPolicy_ptr
  Loadable_Thread_Policy::create (
    const CORBA::Any &value
    ACE_ENV_ARG_DECL)
      ACE_THROW_SPEC ((CORBA::PolicyError))
  {
    POA_ThreadPolicy* policy = 0;

    ACE_NEW_RETURN (policy,
                    POA_ThreadPolicy,
                    PortableServer::ThreadPolicy::_nil ());

    (void) policy->init (value ACE_ENV_ARG_PARAMETER);
    ACE_CHECK_RETURN (PortableServer::ThreadPolicy::_nil ());

    return policy;
  }

  TAO_Cached_Policy_Type
  Loadable_Thread_Policy::_tao_cached_type (void) const
  {
    return TAO_CACHED_POLICY_THREAD;
  }

  TAO_Policy_Scope
  Loadable_Thread_Policy::_tao_scope (void) const
  {
    return TAO_POLICY_POA_SCOPE;
  }

  ACE_STATIC_SVC_DEFINE (
      Loadable_Thread_Policy,
      ACE_TEXT ("Loadable_Thread_Policy"),
      ACE_SVC_OBJ_T,
      &ACE_SVC_NAME (Loadable_Thread_Policy),
      ACE_Service_Type::DELETE_THIS | ACE_Service_Type::DELETE_OBJ,
      0
    )

  ACE_FACTORY_DEFINE (TAO_PortableServer, Loadable_Thread_Policy)

  #if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)

  template class ACE_Dynamic_Service<Loadable_Thread_Policy>;

  #elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)

  #pragma instantiate ACE_Dynamic_Service<Loadable_Thread_Policy>

  #endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */
}

