// $Id$

#include "Loadable_Id_Assignment_Policy.h"
#include "ace/Dynamic_Service.h"
#include "POA_Id_AssignmentPolicy.h"

ACE_RCSID (PortableServer,
           Loadable_Id_Assignment_Policy,
           "$Id$")

namespace TAO
{
  Loadable_Id_Assignment_Policy::~Loadable_Id_Assignment_Policy (void)
  {
  }

  PortableServer::IdAssignmentPolicy_ptr
  Loadable_Id_Assignment_Policy::create (
    PortableServer::IdAssignmentPolicyValue value)
  {
    POA_Id_AssignmentPolicy* policy = 0;

    ACE_NEW_RETURN (policy,
                    POA_Id_AssignmentPolicy,
                    PortableServer::IdAssignmentPolicy::_nil ());

    (void) policy->init (value);

    return policy;
  }

  PortableServer::IdAssignmentPolicy_ptr
  Loadable_Id_Assignment_Policy::create (
    const CORBA::Any &value
    ACE_ENV_ARG_DECL)
      ACE_THROW_SPEC ((CORBA::PolicyError))
  {
    POA_Id_AssignmentPolicy* policy = 0;

    ACE_NEW_RETURN (policy,
                    POA_Id_AssignmentPolicy,
                    PortableServer::IdAssignmentPolicy::_nil ());

    (void) policy->init (value ACE_ENV_ARG_PARAMETER);
    ACE_CHECK_RETURN (PortableServer::IdAssignmentPolicy::_nil ());

    return policy;
  }

  TAO_Cached_Policy_Type
  Loadable_Id_Assignment_Policy::_tao_cached_type (void) const
  {
    return TAO_CACHED_POLICY_ID_ASSIGNMENT;
  }

  TAO_Policy_Scope
  Loadable_Id_Assignment_Policy::_tao_scope (void) const
  {
    return TAO_POLICY_POA_SCOPE;
  }

  ACE_STATIC_SVC_DEFINE (
      Loadable_Id_Assignment_Policy,
      ACE_TEXT ("Loadable_Id_Assignment_Policy"),
      ACE_SVC_OBJ_T,
      &ACE_SVC_NAME (Loadable_Id_Assignment_Policy),
      ACE_Service_Type::DELETE_THIS | ACE_Service_Type::DELETE_OBJ,
      0
    )

  ACE_FACTORY_DEFINE (TAO_PortableServer, Loadable_Id_Assignment_Policy)

  #if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)

  template class ACE_Dynamic_Service<Loadable_Id_Assignment_Policy>;

  #elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)

  #pragma instantiate ACE_Dynamic_Service<Loadable_Id_Assignment_Policy>

  #endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */
}

