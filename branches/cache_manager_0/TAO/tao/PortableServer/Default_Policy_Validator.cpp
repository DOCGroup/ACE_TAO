// @(#) $Id$

#include "Default_Policy_Validator.h"
#include "POA_Policies.h"
#include "tao/Policy_Set.h"

ACE_RCSID(tao, POA, "$Id$")


TAO_POA_Default_Policy_Validator::TAO_POA_Default_Policy_Validator (TAO_ORB_Core &orb_core)
  : TAO_Policy_Validator (orb_core)
{
}

TAO_POA_Default_Policy_Validator::~TAO_POA_Default_Policy_Validator (void)
{
}

void
TAO_POA_Default_Policy_Validator::validate_impl (TAO_Policy_Set &policies
                                                 TAO_ENV_ARG_DECL)
{
#if (TAO_HAS_MINIMUM_POA == 0)
  CORBA::Policy_var policy =
    policies.get_cached_policy (TAO_CACHED_POLICY_SERVANT_RETENTION);
  PortableServer::ServantRetentionPolicy_var srp =
    PortableServer::ServantRetentionPolicy::_narrow (policy.in ()
                                                     TAO_ENV_ARG_PARAMETER);
  ACE_CHECK;
  PortableServer::ServantRetentionPolicyValue servant_retention =
    srp->value (TAO_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;

  policy = policies.get_cached_policy (TAO_CACHED_POLICY_REQUEST_PROCESSING);
  PortableServer::RequestProcessingPolicy_var rpp =
    PortableServer::RequestProcessingPolicy::_narrow (policy.in ()
                                                      TAO_ENV_ARG_PARAMETER);
  ACE_CHECK;
  PortableServer::RequestProcessingPolicyValue request_processing =
    rpp->value (TAO_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;

  // The NON_RETAIN policy requires either the USE_DEFAULT_SERVANT or
  // USE_SERVANT_MANAGER policies.
  if (servant_retention == PortableServer::NON_RETAIN)
    if (request_processing != PortableServer::USE_SERVANT_MANAGER &&
        request_processing  != PortableServer::USE_DEFAULT_SERVANT)
      ACE_THROW (PortableServer::POA::InvalidPolicy ());

  // USE_ACTIVE_OBJECT_MAP_ONLY requires the RETAIN policy.
  if (request_processing == PortableServer::USE_ACTIVE_OBJECT_MAP_ONLY)
    if (servant_retention != PortableServer::RETAIN)
      ACE_THROW (PortableServer::POA::InvalidPolicy ());


  policy = policies.get_cached_policy (TAO_CACHED_POLICY_ID_UNIQUENESS);
  PortableServer::IdUniquenessPolicy_var iup =
    PortableServer::IdUniquenessPolicy::_narrow (policy.in ()
                                                 TAO_ENV_ARG_PARAMETER);
  ACE_CHECK;
  PortableServer::IdUniquenessPolicyValue id_uniqueness =
    iup->value (TAO_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;


  policy = policies.get_cached_policy (TAO_CACHED_POLICY_IMPLICIT_ACTIVATION);
  PortableServer::ImplicitActivationPolicy_var iap =
    PortableServer::ImplicitActivationPolicy::_narrow (policy.in ()
                                                       TAO_ENV_ARG_PARAMETER);
  ACE_CHECK;
  PortableServer::ImplicitActivationPolicyValue implicit_activation =
    iap->value (TAO_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;

  policy = policies.get_cached_policy (TAO_CACHED_POLICY_ID_ASSIGNMENT);
  PortableServer::IdAssignmentPolicy_var idap =
    PortableServer::IdAssignmentPolicy::_narrow (policy.in ()
                                                 TAO_ENV_ARG_PARAMETER);
  ACE_CHECK;
  PortableServer::IdAssignmentPolicyValue id_assignment =
    idap->value (TAO_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;


  // USE_DEFAULT_SERVANT requires the MULTIPLE_ID policy.
  if (request_processing == PortableServer::USE_DEFAULT_SERVANT)
    if (id_uniqueness != PortableServer::MULTIPLE_ID)
      ACE_THROW (PortableServer::POA::InvalidPolicy ());

  // IMPLICIT_ACTIVATION requires the SYSTEM_ID and RETAIN policies.
  if (implicit_activation == PortableServer::IMPLICIT_ACTIVATION)
    if (servant_retention != PortableServer::RETAIN ||
        id_assignment != PortableServer::SYSTEM_ID)
      ACE_THROW (PortableServer::POA::InvalidPolicy ());
#else /* TAO_HAS_MINIMUM_POA == 0 */
  ACE_UNUSED_ARG (policies);
  TAO_ENV_ARG_NOT_USED; // FUZZ: ignore check_for_ace_check
#endif /* TAO_HAS_MINIMUM_POA == 0 */
}


CORBA::Boolean
TAO_POA_Default_Policy_Validator::legal_policy_impl (CORBA::PolicyType type)
{
  return (type == PortableServer::THREAD_POLICY_ID ||
          type == PortableServer::LIFESPAN_POLICY_ID ||
          type == PortableServer::ID_UNIQUENESS_POLICY_ID ||
          type == PortableServer::ID_ASSIGNMENT_POLICY_ID ||
          type == PortableServer::IMPLICIT_ACTIVATION_POLICY_ID ||
          type == PortableServer::SERVANT_RETENTION_POLICY_ID ||
          type == PortableServer::REQUEST_PROCESSING_POLICY_ID);
}

void
TAO_POA_Default_Policy_Validator::merge_policies_impl (TAO_Policy_Set &
                                                       TAO_ENV_ARG_DECL_NOT_USED)
{
}
