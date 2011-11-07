// $Id$

#include "tao/PI/PI_PolicyFactory.h"

#if TAO_HAS_INTERCEPTORS == 1

#include "tao/PI/ProcessingModePolicy.h"
#include "tao/ORB_Constants.h"
#include "tao/SystemException.h"
#include "ace/CORBA_macros.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

CORBA::Policy_ptr
TAO_PI_PolicyFactory::create_policy (
    CORBA::PolicyType type,
    const CORBA::Any &value)
{
  if (type == PortableInterceptor::PROCESSING_MODE_POLICY_TYPE)
    {
      TAO_ProcessingModePolicy *processing_mode_policy = 0;
      PortableInterceptor::ProcessingMode policy_value;

      if ((value >>= policy_value) == 0)
        {
          throw ::CORBA::PolicyError (CORBA::BAD_POLICY_VALUE);
        }

      ACE_NEW_THROW_EX (processing_mode_policy,
                        TAO_ProcessingModePolicy (policy_value),
                        CORBA::NO_MEMORY (TAO::VMCID,
                                          CORBA::COMPLETED_NO));

      return processing_mode_policy;
    }

  throw ::CORBA::PolicyError (CORBA::BAD_POLICY_TYPE);
}

TAO_END_VERSIONED_NAMESPACE_DECL

#endif  /* TAO_HAS_INTERCEPTORS == 1 */
