// -*- C++ -*-

#include "Security_ORBInitializer.h"
#include "tao/debug.h"

ACE_RCSID (TAO_Security,
           Security_ORBInitializer,
           "$Id$")

#include "tao/ORBInitInfo.h"

#include "Security_Current.h"
#include "Security_PolicyFactory.h"


void
TAO_Security_ORBInitializer::pre_init (
    PortableInterceptor::ORBInitInfo_ptr info
    TAO_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  // @@ This is busted.  TAO_ORBInitInfo should do proper reference
  //    counting.
  // Narrow to a TAO_ORBInitInfo object to get access to the
  // allocate_tss_slot_id() TAO extension.
  TAO_ORBInitInfo_var tao_info = TAO_ORBInitInfo::_narrow (info
                                                           TAO_ENV_ARG_PARAMETER);
  ACE_CHECK;

  if (CORBA::is_nil (tao_info.in ()))
    {
      if (TAO_debug_level > 0)
        ACE_ERROR ((LM_ERROR,
                    "(%P|%t) Security_ORBInitializer::pre_init:\n"
                    "(%P|%t)    Unable to narrow "
                    "\"PortableInterceptor::ORBInitInfo_ptr\" to\n"
                    "(%P|%t)   \"TAO_ORBInitInfo *.\"\n"));

      ACE_THROW (CORBA::INTERNAL ());
    }

  // Reserve a TSS slot in the ORB core internal TSS resources for the
  // thread-specific portion of Security::Current.
  size_t tss_slot = tao_info->allocate_tss_slot_id (0
                                                    TAO_ENV_ARG_PARAMETER);
  ACE_CHECK;

  CORBA::String_var orb_id = info->orb_id (TAO_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;

  // Create the SecurityLevel2::Current object.
  SecurityLevel2::Current_ptr current = SecurityLevel2::Current::_nil ();
  ACE_NEW_THROW_EX (current,
                    TAO_Security_Current (tss_slot, orb_id.in ()),
                    CORBA::NO_MEMORY (
                      CORBA_SystemException::_tao_minor_code (
                        TAO_DEFAULT_MINOR_CODE,
                        ENOMEM),
                      CORBA::COMPLETED_NO));
  ACE_CHECK;

  SecurityLevel2::Current_var security_current = current;

  // Register the SecurityLevel2::Current object reference with the
  // ORB.
  info->register_initial_reference ("SecurityCurrent",
                                    security_current.in ()
                                    TAO_ENV_ARG_PARAMETER);
  ACE_CHECK;
}

void
TAO_Security_ORBInitializer::post_init (
    PortableInterceptor::ORBInitInfo_ptr info
    TAO_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  this->register_policy_factories (info
                                   TAO_ENV_ARG_PARAMETER);
  ACE_CHECK;
}

void
TAO_Security_ORBInitializer::register_policy_factories (
  PortableInterceptor::ORBInitInfo_ptr info
  TAO_ENV_ARG_DECL)
{
  // Register the security policy factories.

  // The security policy factory is stateless and reentrant, so share a
  // single instance between all ORBs.
  PortableInterceptor::PolicyFactory_ptr policy_factory =
    &(this->policy_factory_);

  // Bind the same policy factory to all security related policy
  // types since a single policy factory is used to create each of
  // the different types of security policies.

  CORBA::PolicyType type;

  type = Security::SecQOPPolicy;
  info->register_policy_factory (type,
                                 policy_factory
                                 TAO_ENV_ARG_PARAMETER);
  ACE_CHECK;

  type = Security::SecMechanismsPolicy;
  info->register_policy_factory (type,
                                 policy_factory
                                 TAO_ENV_ARG_PARAMETER);
  ACE_CHECK;

  type = Security::SecInvocationCredentialsPolicy;
  info->register_policy_factory (type,
                                 policy_factory
                                 TAO_ENV_ARG_PARAMETER);
  ACE_CHECK;

  type = Security::SecFeaturePolicy;   // Deprecated
  info->register_policy_factory (type,
                                 policy_factory
                                 TAO_ENV_ARG_PARAMETER);
  ACE_CHECK;

  type = Security::SecDelegationDirectivePolicy;
  info->register_policy_factory (type,
                                 policy_factory
                                 TAO_ENV_ARG_PARAMETER);
  ACE_CHECK;

  type = Security::SecEstablishTrustPolicy;
  info->register_policy_factory (type,
                                 policy_factory
                                 TAO_ENV_ARG_PARAMETER);
  ACE_CHECK;

  // ----------------------------------------------------------------
}
