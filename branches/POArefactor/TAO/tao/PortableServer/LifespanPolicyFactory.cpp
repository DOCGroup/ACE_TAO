// $Id$

#include "LifespanPolicyFactory.h"
#include "ace/Dynamic_Service.h"
#include "LifespanPolicy.h"

ACE_RCSID (PortableServer,
           LifespanPolicyFactory,
           "$Id$")

namespace TAO
{
  namespace Portable_Server
  {
    LifespanPolicyFactory::~LifespanPolicyFactory (void)
    {
    }

    ::PortableServer::LifespanPolicy_ptr
    LifespanPolicyFactory::create (
      ::PortableServer::LifespanPolicyValue value)
    {
      POA_LifespanPolicy* policy = 0;

      ACE_NEW_RETURN (policy,
                      POA_LifespanPolicy,
                      ::PortableServer::LifespanPolicy::_nil ());

      (void) policy->init (value);

      return policy;
    }

    ::PortableServer::LifespanPolicy_ptr
    LifespanPolicyFactory::create (
      const CORBA::Any &value
      ACE_ENV_ARG_DECL)
        ACE_THROW_SPEC ((CORBA::PolicyError))
    {
      POA_LifespanPolicy* policy = 0;

      ACE_NEW_RETURN (policy,
                      POA_LifespanPolicy,
                      ::PortableServer::LifespanPolicy::_nil ());

      (void) policy->init (value ACE_ENV_ARG_PARAMETER);
      ACE_CHECK_RETURN (::PortableServer::LifespanPolicy::_nil ());

      return policy;
    }

    TAO_Cached_Policy_Type
    LifespanPolicyFactory::_tao_cached_type (void) const
    {
      return TAO_CACHED_POLICY_LIFESPAN;
    }

    TAO_Policy_Scope
    LifespanPolicyFactory::_tao_scope (void) const
    {
      return TAO_POLICY_POA_SCOPE;
    }

    ACE_STATIC_SVC_DEFINE (
        LifespanPolicyFactory,
        ACE_TEXT ("LifespanPolicyFactory"),
        ACE_SVC_OBJ_T,
        &ACE_SVC_NAME (LifespanPolicyFactory),
        ACE_Service_Type::DELETE_THIS | ACE_Service_Type::DELETE_OBJ,
        0
      )

    ACE_FACTORY_DEFINE (TAO_PortableServer, LifespanPolicyFactory)

    #if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)

    template class ACE_Dynamic_Service<LifespanPolicyFactory>;

    #elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)

    #pragma instantiate ACE_Dynamic_Service<LifespanPolicyFactory>

    #endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */
  }
}

