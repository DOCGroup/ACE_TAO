// $Id$

#include "IdAssignmentPolicyFactory.h"
#include "ace/Dynamic_Service.h"
#include "IdAssignmentPolicy.h"

ACE_RCSID (PortableServer,
           IdAssignmentPolicyFactory,
           "$Id$")

namespace TAO
{
  namespace Portable_Server
  {
    IdAssignmentPolicyFactory::~IdAssignmentPolicyFactory (void)
    {
    }

    ::PortableServer::IdAssignmentPolicy_ptr
    IdAssignmentPolicyFactory::create (
      ::PortableServer::IdAssignmentPolicyValue value)
    {
      IdAssignmentPolicy* policy = 0;

      ACE_NEW_RETURN (policy,
                      IdAssignmentPolicy,
                      ::PortableServer::IdAssignmentPolicy::_nil ());

      (void) policy->init (value);

      return policy;
    }

    ::PortableServer::IdAssignmentPolicy_ptr
    IdAssignmentPolicyFactory::create (
      const CORBA::Any &value
      ACE_ENV_ARG_DECL)
        ACE_THROW_SPEC ((CORBA::PolicyError))
    {
      IdAssignmentPolicy* policy = 0;

      ACE_NEW_RETURN (policy,
                      IdAssignmentPolicy,
                      ::PortableServer::IdAssignmentPolicy::_nil ());

      (void) policy->init (value ACE_ENV_ARG_PARAMETER);
      ACE_CHECK_RETURN (::PortableServer::IdAssignmentPolicy::_nil ());

      return policy;
    }

    TAO_Cached_Policy_Type
    IdAssignmentPolicyFactory::_tao_cached_type (void) const
    {
      return TAO_CACHED_POLICY_ID_ASSIGNMENT;
    }

    TAO_Policy_Scope
    IdAssignmentPolicyFactory::_tao_scope (void) const
    {
      return TAO_POLICY_POA_SCOPE;
    }

ACE_STATIC_SVC_DEFINE (IdAssignmentPolicyFactory,
                       ACE_TEXT ("IdAssignmentPolicyFactory"),
                       ACE_SVC_OBJ_T,
                       &ACE_SVC_NAME (IdAssignmentPolicyFactory),
                       ACE_Service_Type::DELETE_THIS | ACE_Service_Type::DELETE_OBJ,
                       0)

ACE_FACTORY_DEFINE (TAO_PortableServer, IdAssignmentPolicyFactory)

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)

  template class ACE_Dynamic_Service<IdAssignmentPolicyFactory>;

#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)

    #pragma instantiate ACE_Dynamic_Service<IdAssignmentPolicyFactory>

#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */
  }
}
