// -*- C++ -*-

#include "Security_ORBInitializer.h"
#include "tao/debug.h"

ACE_RCSID (TAO_Security,
           Security_ORBInitializer,
           "$Id$")

#include "tao/ORBInitInfo.h"

//#include "Security_PolicyFactory.h"
#include "Security_Current.h"


void
TAO_Security_ORBInitializer::pre_init (
    PortableInterceptor::ORBInitInfo_ptr info
    TAO_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  TAO_ENV_ARG_DEFN;

  // @@ This is busted.  TAO_ORBInitInfo should do proper reference
  //    counting.
  // Narrow to a TAO_ORBInitInfo object to get access to the
  // allocate_tss_slot_id() TAO extension.
  TAO_ORBInitInfo_var tao_info = TAO_ORBInitInfo::_narrow (info,
                                                           ACE_TRY_ENV);
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
  size_t tss_slot = tao_info->allocate_tss_slot_id (0,
                                                    ACE_TRY_ENV);
  ACE_CHECK;

  CORBA::String_var orb_id = info->orb_id (ACE_TRY_ENV);
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
                                    security_current.in (),
                                    ACE_TRY_ENV);
  ACE_CHECK;
}

void
TAO_Security_ORBInitializer::post_init (
    PortableInterceptor::ORBInitInfo_ptr
    TAO_ENV_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  // @todo:  Secure invocation policy support should hopefully be
  //         ready for TAO 1.2.
#if 0
  this->register_policy_factories (info,
                                   ACE_TRY_ENV);
  ACE_CHECK;
#endif  /* 0 */
}

// @todo:  Secure invocation policy support should hopefully be
//         ready for TAO 1.2.
#if 0
void
TAO_Security_ORBInitializer::register_policy_factories (
  PortableInterceptor::ORBInitInfo_ptr info,
  CORBA::Environment &ACE_TRY_ENV)
{
  /// Register the Security policy factories.
  PortableInterceptor::PolicyFactory_ptr temp_factory =
    PortableInterceptor::PolicyFactory::_nil ();
  PortableInterceptor::PolicyFactory_var policy_factory;

  // ----------------------------------------------------------------
  // This policy factory is used for all SecureInvocation related
  // policies.
  ACE_NEW_THROW_EX (temp_factory,
                    TAO_SecureInvocationPolicyFactory,
                    CORBA::NO_MEMORY (
                      CORBA::SystemException::_tao_minor_code (
                         TAO_DEFAULT_MINOR_CODE,
                         ENOMEM),
                      CORBA::COMPLETED_NO));
  ACE_CHECK;

  policy_factory = temp_factory;

  // Bind the same policy factory to all SecureInvocation related
  // policy types since a single policy factory is used to create each
  // of the different types of SecureInvocation policies.

  CORBA::PolicyType type;

  type = Security::SecClientSecureInvocation;
  info->register_policy_factory (type,
                                 policy_factory.in (),
                                 ACE_TRY_ENV);
  ACE_CHECK;

  type = Security::SecTargetSecureInvocation;
  info->register_policy_factory (type,
                                 policy_factory.in (),
                                 ACE_TRY_ENV);
  ACE_CHECK;
  // ----------------------------------------------------------------
}
#endif  /* 0 */
