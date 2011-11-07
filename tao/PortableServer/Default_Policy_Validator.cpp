// $Id$

#include "tao/PortableServer/Default_Policy_Validator.h"
#include "tao/ORB_Core.h"
#include "tao/Policy_Set.h"
#include "tao/PolicyFactory_Registry_Adapter.h"
#include "tao/PortableServer/PortableServer.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

TAO_POA_Default_Policy_Validator::TAO_POA_Default_Policy_Validator (
  TAO_ORB_Core &orb_core)
  : TAO_Policy_Validator (orb_core)
{
}

TAO_POA_Default_Policy_Validator::~TAO_POA_Default_Policy_Validator (void)
{
}

void
TAO_POA_Default_Policy_Validator::validate_impl (TAO_Policy_Set &policies)
{
#if (TAO_HAS_MINIMUM_POA == 0) && !defined (CORBA_E_COMPACT) && !defined (CORBA_E_MICRO)
  CORBA::Policy_var policy =
    policies.get_cached_policy (TAO_CACHED_POLICY_SERVANT_RETENTION);

  PortableServer::ServantRetentionPolicy_var srp =
    PortableServer::ServantRetentionPolicy::_narrow (policy.in ());
  PortableServer::ServantRetentionPolicyValue servant_retention =
    srp->value ();

  policy = policies.get_cached_policy (TAO_CACHED_POLICY_REQUEST_PROCESSING);

  PortableServer::RequestProcessingPolicy_var rpp =
    PortableServer::RequestProcessingPolicy::_narrow (policy.in ());
  PortableServer::RequestProcessingPolicyValue request_processing =
    rpp->value ();

  // The NON_RETAIN policy requires either the USE_DEFAULT_SERVANT or
  // USE_SERVANT_MANAGER policies.
  if (servant_retention == PortableServer::NON_RETAIN)
    if (request_processing != PortableServer::USE_SERVANT_MANAGER &&
        request_processing  != PortableServer::USE_DEFAULT_SERVANT)
      throw PortableServer::POA::InvalidPolicy ();

  // USE_ACTIVE_OBJECT_MAP_ONLY requires the RETAIN policy.
  if (request_processing == PortableServer::USE_ACTIVE_OBJECT_MAP_ONLY)
    if (servant_retention != PortableServer::RETAIN)
      throw PortableServer::POA::InvalidPolicy ();

  policy = policies.get_cached_policy (TAO_CACHED_POLICY_ID_UNIQUENESS);

  PortableServer::IdUniquenessPolicy_var iup =
    PortableServer::IdUniquenessPolicy::_narrow (policy.in ());
  PortableServer::IdUniquenessPolicyValue id_uniqueness =
    iup->value ();

  policy =
    policies.get_cached_policy (TAO_CACHED_POLICY_IMPLICIT_ACTIVATION);

  PortableServer::ImplicitActivationPolicy_var iap =
    PortableServer::ImplicitActivationPolicy::_narrow (policy.in ());
  PortableServer::ImplicitActivationPolicyValue implicit_activation =
    iap->value ();

  policy = policies.get_cached_policy (TAO_CACHED_POLICY_ID_ASSIGNMENT);

  PortableServer::IdAssignmentPolicy_var idap =
    PortableServer::IdAssignmentPolicy::_narrow (policy.in ());
  PortableServer::IdAssignmentPolicyValue id_assignment =
    idap->value ();

  // USE_DEFAULT_SERVANT requires the MULTIPLE_ID policy.
  if (request_processing == PortableServer::USE_DEFAULT_SERVANT)
    if (id_uniqueness != PortableServer::MULTIPLE_ID)
      throw PortableServer::POA::InvalidPolicy ();

  // IMPLICIT_ACTIVATION requires the SYSTEM_ID and RETAIN policies.
  if (implicit_activation == PortableServer::IMPLICIT_ACTIVATION)
    if (servant_retention != PortableServer::RETAIN ||
        id_assignment != PortableServer::SYSTEM_ID)
      throw PortableServer::POA::InvalidPolicy ();
#else /* TAO_HAS_MINIMUM_POA == 0 */
  ACE_UNUSED_ARG (policies);
#endif /* TAO_HAS_MINIMUM_POA == 0 */
}

CORBA::Boolean
TAO_POA_Default_Policy_Validator::legal_policy_impl (CORBA::PolicyType type)
{
  // Check known POA policies, or if given PolicyType has a
  // corresponding PolicyFactory.  The PolicyFactory check is mandated
  // by the CORBA specification.
  return
    (
#   if ! defined (CORBA_E_COMPACT) && ! defined (CORBA_E_MICRO)
      type == PortableServer::THREAD_POLICY_ID ||
#   endif
#   if ! defined (CORBA_E_MICRO)
      type == PortableServer::LIFESPAN_POLICY_ID ||
      type == PortableServer::ID_UNIQUENESS_POLICY_ID ||
      type == PortableServer::ID_ASSIGNMENT_POLICY_ID ||
#   endif
#   if ! defined (CORBA_E_COMPACT) && ! defined (CORBA_E_MICRO)
      type == PortableServer::IMPLICIT_ACTIVATION_POLICY_ID ||
      type == PortableServer::SERVANT_RETENTION_POLICY_ID ||
      type == PortableServer::REQUEST_PROCESSING_POLICY_ID ||
#   endif
      (this->orb_core_.policy_factory_registry () != 0 &&
         this->orb_core_.policy_factory_registry ()->factory_exists (type)));
}

void
TAO_POA_Default_Policy_Validator::merge_policies_impl (TAO_Policy_Set &)
{
}

TAO_END_VERSIONED_NAMESPACE_DECL
