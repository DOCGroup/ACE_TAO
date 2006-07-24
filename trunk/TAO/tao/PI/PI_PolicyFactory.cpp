#include "tao/PI/PI_PolicyFactory.h"

#if TAO_HAS_INTERCEPTORS == 1

#include "tao/PI/ProcessingModePolicy.h"
#include "tao/ORB_Constants.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

ACE_RCSID (tao,
           PI_PolicyFactory,
           "$Id$")

CORBA::Policy_ptr
TAO_PI_PolicyFactory::create_policy (
    CORBA::PolicyType type,
    const CORBA::Any &value
    ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   CORBA::PolicyError))
{
  if (type == PortableInterceptor::PROCESSING_MODE_POLICY_TYPE)
    {
      TAO_ProcessingModePolicy *processing_mode_policy = 0;
      PortableInterceptor::ProcessingMode policy_value;

      if ((value >>= policy_value) == 0)
        {
          ACE_THROW_RETURN (CORBA::PolicyError (CORBA::BAD_POLICY_VALUE),
                            CORBA::Policy::_nil ());
        }

      ACE_NEW_THROW_EX (processing_mode_policy,
                        TAO_ProcessingModePolicy (policy_value),
                        CORBA::NO_MEMORY (TAO::VMCID,
                                          CORBA::COMPLETED_NO));
      ACE_CHECK_RETURN (CORBA::Policy::_nil ());

      return processing_mode_policy;
    }

  ACE_THROW_RETURN (CORBA::PolicyError (CORBA::BAD_POLICY_TYPE),
                    CORBA::Policy::_nil ());
}

TAO_END_VERSIONED_NAMESPACE_DECL

#endif  /* TAO_HAS_INTERCEPTORS == 1 */
