// -*- C++ -*-

#include "orbsvcs/Security/Security_ORBInitializer.h"

ACE_RCSID (Security,
           Security_ORBInitializer,
           "$Id$")


// #include "Security_Current.h"
#include "orbsvcs/Security/SL3_SecurityCurrent.h"
#include "orbsvcs/Security/SL3_CredentialsCurator.h"
#include "orbsvcs/Security/SL3_SecurityManager.h"

#include "orbsvcs/SecurityC.h"

#include "tao/PI/ORBInitInfo.h"
#include "tao/ORB_Constants.h"
#include "tao/debug.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

void
TAO::Security::ORBInitializer::pre_init (
    PortableInterceptor::ORBInitInfo_ptr info
    ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  // Narrow to a TAO_ORBInitInfo object to get access to the
  // allocate_tss_slot_id() TAO extension.
  TAO_ORBInitInfo_var tao_info =
    TAO_ORBInitInfo::_narrow (info
                              ACE_ENV_ARG_PARAMETER);

  if (CORBA::is_nil (tao_info.in ()))
    {
      if (TAO_debug_level > 0)
        ACE_ERROR ((LM_ERROR,
                    "(%P|%t) Security_ORBInitializer::pre_init:\n"
                    "(%P|%t)    Unable to narrow "
                    "\"PortableInterceptor::ORBInitInfo_ptr\" to\n"
                    "(%P|%t)   \"TAO_ORBInitInfo_ptr.\"\n"));

      ACE_THROW (CORBA::INTERNAL ());
    }

//   // Reserve a TSS slot in the ORB core internal TSS resources for the
//   // thread-specific portion of Security::Current.
//   size_t old_tss_slot = tao_info->allocate_tss_slot_id (0
//                                                         ACE_ENV_ARG_PARAMETER);

//   CORBA::String_var orb_id = info->orb_id ();

//   // Create the SecurityLevel2::Current object.
//   SecurityLevel2::Current_ptr current = SecurityLevel2::Current::_nil ();
//   ACE_NEW_THROW_EX (current,
//                     TAO_Security_Current (old_tss_slot, orb_id.in ()),
//                     CORBA::NO_MEMORY (
//                       CORBA::SystemException::_tao_minor_code (
//                         TAO::VMCID,
//                         ENOMEM),
//                       CORBA::COMPLETED_NO));

//   SecurityLevel2::Current_var security_current = current;

//   // Register the SecurityLevel2::Current object reference with the
//   // ORB.
//   info->register_initial_reference ("SecurityCurrent",
//                                     security_current.in ()
//                                     ACE_ENV_ARG_PARAMETER);

  // Reserve a TSS slot in the ORB core internal TSS resources for the
  // thread-specific portion of SecurityLevel3::SecurityCurrent
  // object.
  size_t tss_slot = tao_info->allocate_tss_slot_id (0
                                                    ACE_ENV_ARG_PARAMETER);


  // Create the SecurityLevel3::Current object.
  SecurityLevel3::SecurityCurrent_ptr current3;
  ACE_NEW_THROW_EX (current3,
                    TAO::SL3::SecurityCurrent (tss_slot,
                                               tao_info->orb_core ()),
                    CORBA::NO_MEMORY (
                      CORBA::SystemException::_tao_minor_code (
                        TAO::VMCID,
                        ENOMEM),
                      CORBA::COMPLETED_NO));

  SecurityLevel3::SecurityCurrent_var security_current3 = current3;

  // Register the SecurityLevel2::Current object reference with the
  // ORB.
  info->register_initial_reference ("SecurityLevel3:SecurityCurrent",
                                    security_current3.in ()
                                    ACE_ENV_ARG_PARAMETER);

  // Create the SecurityLevel3::CredentialsCurator object.
  SecurityLevel3::CredentialsCurator_ptr curator;
  ACE_NEW_THROW_EX (curator,
                    TAO::SL3::CredentialsCurator,
                    CORBA::NO_MEMORY (
                      CORBA::SystemException::_tao_minor_code (
                        TAO::VMCID,
                        ENOMEM),
                      CORBA::COMPLETED_NO));

  SecurityLevel3::CredentialsCurator_var credentials_curator = curator;

  // Register the SecurityLevel3::CredentialsCurator object reference
  // with the ORB.
  info->register_initial_reference ("SecurityLevel3:CredentialsCurator",
                                    credentials_curator.in ()
                                    ACE_ENV_ARG_PARAMETER);

  // Create the SecurityLevel3::SecurityManager object.
  SecurityLevel3::SecurityManager_ptr manager3;
  ACE_NEW_THROW_EX (manager3,
                    TAO::SL3::SecurityManager (credentials_curator.in ()),
                    CORBA::NO_MEMORY (
                      CORBA::SystemException::_tao_minor_code (
                        TAO::VMCID,
                        ENOMEM),
                      CORBA::COMPLETED_NO));

  SecurityLevel3::SecurityManager_var security_manager3 = manager3;

  // Register the SecurityLevel3::SecurityManager object reference
  // with the ORB.
  info->register_initial_reference ("SecurityLevel3:SecurityManager",
                                    security_manager3.in ()
                                    ACE_ENV_ARG_PARAMETER);
}

void
TAO::Security::ORBInitializer::post_init (
    PortableInterceptor::ORBInitInfo_ptr info
    ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  this->register_policy_factories (info
                                   ACE_ENV_ARG_PARAMETER);
}

void
TAO::Security::ORBInitializer::register_policy_factories (
  PortableInterceptor::ORBInitInfo_ptr info
  ACE_ENV_ARG_DECL)
{
  // Register the security policy factories.

  if (CORBA::is_nil (this->policy_factory_.in ()))
    {
      PortableInterceptor::PolicyFactory_ptr policy_factory;
      ACE_NEW_THROW_EX (policy_factory,
                        TAO::Security::PolicyFactory,
                          CORBA::NO_MEMORY (
                            CORBA::SystemException::_tao_minor_code (
                              TAO::VMCID,
                              ENOMEM),
                            CORBA::COMPLETED_NO));

      this->policy_factory_ = policy_factory;
    }

  // Bind the same policy factory to all security related policy
  // types since a single policy factory is used to create each of
  // the different types of security policies.

  CORBA::PolicyType type;

  type = ::Security::SecQOPPolicy;
  info->register_policy_factory (type,
                                 this->policy_factory_.in ()
                                 ACE_ENV_ARG_PARAMETER);

  type = ::Security::SecMechanismsPolicy;
  info->register_policy_factory (type,
                                 this->policy_factory_.in ()
                                 ACE_ENV_ARG_PARAMETER);

  type = ::Security::SecInvocationCredentialsPolicy;
  info->register_policy_factory (type,
                                 this->policy_factory_.in ()
                                 ACE_ENV_ARG_PARAMETER);

  type = ::Security::SecFeaturePolicy;   // Deprecated
  info->register_policy_factory (type,
                                 this->policy_factory_.in ()
                                 ACE_ENV_ARG_PARAMETER);

  type = ::Security::SecDelegationDirectivePolicy;
  info->register_policy_factory (type,
                                 this->policy_factory_.in ()
                                 ACE_ENV_ARG_PARAMETER);

  type = ::Security::SecEstablishTrustPolicy;
  info->register_policy_factory (type,
                                 this->policy_factory_.in ()
                                 ACE_ENV_ARG_PARAMETER);


  type = SecurityLevel3::ContextEstablishmentPolicyType;
  info->register_policy_factory (type,
                                 this->policy_factory_.in ()
                                 ACE_ENV_ARG_PARAMETER);


  type = SecurityLevel3::ObjectCredentialsPolicyType;
  info->register_policy_factory (type,
                                 this->policy_factory_.in ()
                                 ACE_ENV_ARG_PARAMETER);


  // ----------------------------------------------------------------
}

TAO_END_VERSIONED_NAMESPACE_DECL
