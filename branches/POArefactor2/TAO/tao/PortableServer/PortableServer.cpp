// $Id$
#include "PortableServer.h"
#include "Object_Adapter_Factory.h"

#include "ThreadPolicyFactoryImpl.h"
#include "LifespanPolicyFactoryImpl.h"
#include "IdAssignmentPolicyFactoryImpl.h"
#include "IdUniquenessPolicyFactoryImpl.h"
#include "ImplicitActivationPolicyFactoryImpl.h"
#include "RequestProcessingPolicyFactoryImpl.h"
#include "ServantRetentionPolicyFactoryImpl.h"

#include "ThreadStrategyFactoryImpl.h"
#include "LifespanStrategyFactoryImpl.h"
#include "IdAssignmentStrategyFactoryImpl.h"
#include "IdUniquenessStrategyFactoryImpl.h"
#include "ImplicitActivationStrategyFactoryImpl.h"
#include "RequestProcessingStrategyFactoryImpl.h"
#include "ServantRetentionStrategyFactoryImpl.h"

#include "ThreadPolicyValueORBControl.h"
#include "ThreadPolicyValueSingle.h"

#include "ThreadStrategyORBControl.h"
#include "ThreadStrategySingle.h"
#include "ThreadStrategySingleFactoryImpl.h"

#include "LifespanPolicyValueTransient.h"
#include "LifespanPolicyValuePersistent.h"

#include "IdAssignmentStrategySystem.h"
#include "IdAssignmentStrategyUser.h"

#include "IdUniquenessPolicyValueMultiple.h"
#include "IdUniquenessPolicyValueUnique.h"

#include "IdUniquenessStrategyMultiple.h"
#include "IdUniquenessStrategyUnique.h"

#include "ImplicitActivationPolicyValueExplicit.h"
#include "ImplicitActivationPolicyValueImplicit.h"

#include "ImplicitActivationStrategyExplicit.h"
#include "ImplicitActivationStrategyImplicit.h"

#include "RequestProcessingPolicyValueAOMOnly.h"
#include "RequestProcessingPolicyValueDefaultServant.h"
#include "RequestProcessingPolicyValueServantManager.h"

#include "RequestProcessingStrategyAOMOnly.h"
#include "RequestProcessingStrategyDefaultServant.h"
#include "RequestProcessingStrategyServantManager.h"

#include "ServantRetentionPolicyValueRetain.h"
#include "ServantRetentionPolicyValueNonRetain.h"

#include "ServantRetentionStrategyNonRetainFactoryImpl.h"
#include "ServantRetentionStrategyRetainFactoryImpl.h"

#include "RequestProcessingStrategyDefaultServantFactoryImpl.h"
#include "RequestProcessingStrategyAOMOnlyFactoryImpl.h"

ACE_RCSID (PortableServer,
           PortableServer,
           "$Id$")

int
TAO_POA_Initializer::init (void)
{
#if (TAO_HAS_MINIMUM_POA == 0)
  ACE_Service_Config::process_directive (
      TAO::Portable_Server::ace_svc_desc_ThreadPolicyFactoryImpl
    );
#endif /* TAO_HAS_MINIMUM_POA == 0 */

  ACE_Service_Config::process_directive (
      TAO::Portable_Server::ace_svc_desc_LifespanPolicyFactoryImpl
    );

  ACE_Service_Config::process_directive (
      TAO::Portable_Server::ace_svc_desc_IdAssignmentPolicyFactoryImpl
    );

  ACE_Service_Config::process_directive (
      TAO::Portable_Server::ace_svc_desc_IdUniquenessPolicyFactoryImpl
    );

#if (TAO_HAS_MINIMUM_POA == 0)
  ACE_Service_Config::process_directive (
      TAO::Portable_Server::ace_svc_desc_ImplicitActivationPolicyFactoryImpl
    );

  ACE_Service_Config::process_directive (
      TAO::Portable_Server::ace_svc_desc_RequestProcessingPolicyFactoryImpl
    );

  ACE_Service_Config::process_directive (
      TAO::Portable_Server::ace_svc_desc_ServantRetentionPolicyFactoryImpl
    );
#endif /* TAO_HAS_MINIMUM_POA == 0 */

#if (TAO_HAS_MINIMUM_POA == 0)
  ACE_Service_Config::process_directive (
      TAO::Portable_Server::ace_svc_desc_RequestProcessingPolicyValueDefaultServant
    );

  ACE_Service_Config::process_directive (
      TAO::Portable_Server::ace_svc_desc_RequestProcessingPolicyValueServantManager
    );

  ACE_Service_Config::process_directive (
      TAO::Portable_Server::ace_svc_desc_RequestProcessingPolicyValueAOMOnly
    );
#endif /* TAO_HAS_MINIMUM_POA == 0 */

#if (TAO_HAS_MINIMUM_POA == 0)
//  ACE_Service_Config::process_directive (
//      TAO::Portable_Server::ace_svc_desc_RequestProcessingStrategyDefaultServant
//    );
#endif /* TAO_HAS_MINIMUM_POA == 0 */

#if (TAO_HAS_MINIMUM_POA == 0)
//  ACE_Service_Config::process_directive (
//      TAO::Portable_Server::ace_svc_desc_RequestProcessingStrategyServantManager
//    );
#endif /* TAO_HAS_MINIMUM_POA == 0 */

//  ACE_Service_Config::process_directive (
//      TAO::Portable_Server::ace_svc_desc_RequestProcessingStrategyAOMOnly
//    );

#if (TAO_HAS_MINIMUM_POA == 0)
  ACE_Service_Config::process_directive (
      TAO::Portable_Server::ace_svc_desc_ServantRetentionPolicyValueNonRetain
    );

  ACE_Service_Config::process_directive (
      TAO::Portable_Server::ace_svc_desc_ServantRetentionPolicyValueRetain
    );
#endif /* TAO_HAS_MINIMUM_POA == 0 */

  // Policy Values

#if (TAO_HAS_MINIMUM_POA == 0)
  ACE_Service_Config::process_directive (
      TAO::Portable_Server::ace_svc_desc_ThreadPolicyValueORBControl
    );

  ACE_Service_Config::process_directive (
      TAO::Portable_Server::ace_svc_desc_ThreadPolicyValueSingle
    );
#endif /* TAO_HAS_MINIMUM_POA == 0 */

  ACE_Service_Config::process_directive (
      TAO::Portable_Server::ace_svc_desc_LifespanPolicyValueTransient
    );

  ACE_Service_Config::process_directive (
      TAO::Portable_Server::ace_svc_desc_LifespanPolicyValuePersistent
    );

  ACE_Service_Config::process_directive (
      TAO::Portable_Server::ace_svc_desc_IdAssignmentStrategySystem
    );

  ACE_Service_Config::process_directive (
      TAO::Portable_Server::ace_svc_desc_IdAssignmentStrategyUser
    );

  ACE_Service_Config::process_directive (
      TAO::Portable_Server::ace_svc_desc_IdUniquenessPolicyValueMultiple
    );

  ACE_Service_Config::process_directive (
      TAO::Portable_Server::ace_svc_desc_IdUniquenessPolicyValueUnique
    );

  ACE_Service_Config::process_directive (
      TAO::Portable_Server::ace_svc_desc_IdUniquenessStrategyMultiple
    );

  ACE_Service_Config::process_directive (
      TAO::Portable_Server::ace_svc_desc_IdUniquenessStrategyUnique
    );

#if (TAO_HAS_MINIMUM_POA == 0)
  ACE_Service_Config::process_directive (
      TAO::Portable_Server::ace_svc_desc_ImplicitActivationPolicyValueExplicit
    );

  ACE_Service_Config::process_directive (
      TAO::Portable_Server::ace_svc_desc_ImplicitActivationPolicyValueImplicit
    );
#endif /* TAO_HAS_MINIMUM_POA == 0 */

#if (TAO_HAS_MINIMUM_POA == 0)
  ACE_Service_Config::process_directive (
      TAO::Portable_Server::ace_svc_desc_ImplicitActivationStrategyExplicit
    );
#endif /* TAO_HAS_MINIMUM_POA == 0 */

  ACE_Service_Config::process_directive (
      TAO::Portable_Server::ace_svc_desc_ImplicitActivationStrategyImplicit
    );

  // Strategy factories

  ACE_Service_Config::process_directive (
      TAO::Portable_Server::ace_svc_desc_ThreadStrategyFactoryImpl
    );

#if (TAO_HAS_MINIMUM_POA == 0)
  ACE_Service_Config::process_directive (
      TAO::Portable_Server::ace_svc_desc_ThreadStrategySingleFactoryImpl
    );
#endif /* TAO_HAS_MINIMUM_POA == 0 */

  ACE_Service_Config::process_directive (
      TAO::Portable_Server::ace_svc_desc_LifespanStrategyFactoryImpl
    );

  ACE_Service_Config::process_directive (
      TAO::Portable_Server::ace_svc_desc_IdAssignmentStrategyFactoryImpl
    );

  ACE_Service_Config::process_directive (
      TAO::Portable_Server::ace_svc_desc_IdUniquenessStrategyFactoryImpl
    );

  ACE_Service_Config::process_directive (
      TAO::Portable_Server::ace_svc_desc_ImplicitActivationStrategyFactoryImpl
    );

  ACE_Service_Config::process_directive (
      TAO::Portable_Server::ace_svc_desc_RequestProcessingStrategyFactoryImpl
    );

  ACE_Service_Config::process_directive (
      TAO::Portable_Server::ace_svc_desc_RequestProcessingStrategyAOMOnlyFactoryImpl
    );

  ACE_Service_Config::process_directive (
      TAO::Portable_Server::ace_svc_desc_RequestProcessingStrategyDefaultServantFactoryImpl
    );

  ACE_Service_Config::process_directive (
      TAO::Portable_Server::ace_svc_desc_ServantRetentionStrategyFactoryImpl
    );

  ACE_Service_Config::process_directive (
      TAO::Portable_Server::ace_svc_desc_ServantRetentionStrategyRetainFactoryImpl
    );

#if (TAO_HAS_MINIMUM_POA == 0)
  ACE_Service_Config::process_directive (
      TAO::Portable_Server::ace_svc_desc_ServantRetentionStrategyNonRetainFactoryImpl
    );
#endif /* TAO_HAS_MINIMUM_POA == 0 */

  // Strategy implementations

  ACE_Service_Config::process_directive (
      TAO::Portable_Server::ace_svc_desc_ThreadStrategyORBControl
    );

#if (TAO_HAS_MINIMUM_POA == 0)
  ACE_Service_Config::process_directive (
      TAO::Portable_Server::ace_svc_desc_ThreadStrategySingle
    );
#endif /* TAO_HAS_MINIMUM_POA == 0 */

  return
    ACE_Service_Config::process_directive (
        ace_svc_desc_TAO_Object_Adapter_Factory
      );
}
