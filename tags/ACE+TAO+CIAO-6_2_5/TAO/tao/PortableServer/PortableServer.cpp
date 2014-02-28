// $Id$
#include "tao/PortableServer/PortableServer.h"
#include "tao/PortableServer/Object_Adapter.h"
#include "tao/PortableServer/Object_Adapter_Factory.h"
#include "tao/PortableServer/POA_Current_Factory.h"

#include "tao/PortableServer/ThreadStrategyFactoryImpl.h"
#include "tao/PortableServer/LifespanStrategyFactoryImpl.h"
#include "tao/PortableServer/IdAssignmentStrategyFactoryImpl.h"
#include "tao/PortableServer/IdUniquenessStrategyFactoryImpl.h"
#include "tao/PortableServer/ImplicitActivationStrategyFactoryImpl.h"
#include "tao/PortableServer/RequestProcessingStrategyFactoryImpl.h"
#include "tao/PortableServer/ServantRetentionStrategyFactoryImpl.h"

#include "tao/PortableServer/ThreadStrategyORBControl.h"
#include "tao/PortableServer/ThreadStrategySingle.h"
#include "tao/PortableServer/ThreadStrategySingleFactoryImpl.h"

#include "tao/PortableServer/IdAssignmentStrategySystem.h"
#include "tao/PortableServer/IdAssignmentStrategyUser.h"

#include "tao/PortableServer/IdUniquenessStrategyMultiple.h"
#include "tao/PortableServer/IdUniquenessStrategyUnique.h"

#include "tao/PortableServer/ImplicitActivationStrategyExplicit.h"
#include "tao/PortableServer/ImplicitActivationStrategyImplicit.h"

#include "tao/PortableServer/RequestProcessingStrategyAOMOnly.h"
#include "tao/PortableServer/RequestProcessingStrategyDefaultServant.h"
#include "tao/PortableServer/RequestProcessingStrategyServantManager.h"

#include "tao/PortableServer/ServantRetentionStrategyNonRetainFactoryImpl.h"
#include "tao/PortableServer/ServantRetentionStrategyRetainFactoryImpl.h"

#include "tao/PortableServer/RequestProcessingStrategyDefaultServantFI.h"
#include "tao/PortableServer/RequestProcessingStrategyAOMOnlyFactoryImpl.h"
#include "tao/PortableServer/RequestProcessingStrategyServantActivatorFI.h"
#include "tao/PortableServer/RequestProcessingStrategyServantLocatorFI.h"

#include "tao/PortableServer/IdUniquenessStrategyUniqueFactoryImpl.h"

#include "tao/PortableServer/LifespanStrategyPersistentFactoryImpl.h"
#include "tao/PortableServer/LifespanStrategyTransientFactoryImpl.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

int
TAO_POA_Initializer::init (void)
{
  ACE_Service_Config::process_directive (
      ace_svc_desc_IdAssignmentStrategySystem);

#if !defined (CORBA_E_MICRO)
  ACE_Service_Config::process_directive (
      ace_svc_desc_IdAssignmentStrategyUser);
#endif /* CORBA_E_MICRO */

#if !defined (CORBA_E_MICRO)
  ACE_Service_Config::process_directive (
      ace_svc_desc_IdUniquenessStrategyMultiple);
#endif /* CORBA_E_MICRO */

  ACE_Service_Config::process_directive (
      ace_svc_desc_IdUniquenessStrategyUnique);

  ACE_Service_Config::process_directive (
      ace_svc_desc_IdUniquenessStrategyUniqueFactoryImpl);

  ACE_Service_Config::process_directive (
      ace_svc_desc_ImplicitActivationStrategyExplicit);

#if !defined (CORBA_E_MICRO) && !defined (CORBA_E_COMPACT)
  ACE_Service_Config::process_directive (
      ace_svc_desc_ImplicitActivationStrategyImplicit);
#endif /* CORBA_E_MICRO */

  // Strategy factories

  ACE_Service_Config::process_directive (
      ace_svc_desc_ThreadStrategyFactoryImpl);

#if (TAO_HAS_MINIMUM_POA == 0) && !defined (CORBA_E_MICRO) && !defined (CORBA_E_COMPACT)
  ACE_Service_Config::process_directive (
      ace_svc_desc_ThreadStrategySingleFactoryImpl);
#endif /* TAO_HAS_MINIMUM_POA == 0 */

  ACE_Service_Config::process_directive (
      ace_svc_desc_LifespanStrategyFactoryImpl
    );

#if !defined (CORBA_E_MICRO)
  ACE_Service_Config::process_directive (
      ace_svc_desc_LifespanStrategyPersistentFactoryImpl);
#endif /* CORBA_E_MICRO */

  ACE_Service_Config::process_directive (
      ace_svc_desc_LifespanStrategyTransientFactoryImpl);

  ACE_Service_Config::process_directive (
      ace_svc_desc_IdAssignmentStrategyFactoryImpl);

  ACE_Service_Config::process_directive (
      ace_svc_desc_IdUniquenessStrategyFactoryImpl);

  ACE_Service_Config::process_directive (
      ace_svc_desc_ImplicitActivationStrategyFactoryImpl);

  ACE_Service_Config::process_directive (
      ace_svc_desc_RequestProcessingStrategyFactoryImpl);

  ACE_Service_Config::process_directive (
      ace_svc_desc_RequestProcessingStrategyAOMOnlyFactoryImpl);

#if (TAO_HAS_MINIMUM_POA == 0) && !defined (CORBA_E_COMPACT) && !defined (CORBA_E_MICRO)
  ACE_Service_Config::process_directive (
      ace_svc_desc_RequestProcessingStrategyDefaultServantFactoryImpl);

  ACE_Service_Config::process_directive (
      ace_svc_desc_RequestProcessingStrategyServantActivatorFactoryImpl);

  ACE_Service_Config::process_directive (
      ace_svc_desc_RequestProcessingStrategyServantLocatorFactoryImpl);
#endif /* TAO_HAS_MINIMUM_POA == 0 */

  ACE_Service_Config::process_directive (
      ace_svc_desc_ServantRetentionStrategyFactoryImpl);

  ACE_Service_Config::process_directive (
      ace_svc_desc_ServantRetentionStrategyRetainFactoryImpl
    );

#if (TAO_HAS_MINIMUM_POA == 0) && !defined (CORBA_E_MICRO) && !defined (CORBA_E_COMPACT)
  ACE_Service_Config::process_directive (
      ace_svc_desc_ServantRetentionStrategyNonRetainFactoryImpl);
#endif /* TAO_HAS_MINIMUM_POA == 0 */

  // Strategy implementations

  ACE_Service_Config::process_directive (
      ace_svc_desc_ThreadStrategyORBControl);

#if (TAO_HAS_MINIMUM_POA == 0) && !defined (CORBA_E_MICRO) && !defined (CORBA_E_COMPACT)
  ACE_Service_Config::process_directive (
      ace_svc_desc_ThreadStrategySingle);
#endif /* TAO_HAS_MINIMUM_POA == 0 */

  ACE_Service_Config::process_directive (
      ace_svc_desc_TAO_POA_Current_Factory);

  return
    ACE_Service_Config::process_directive (
        ace_svc_desc_TAO_Object_Adapter_Factory);
}

TAO_END_VERSIONED_NAMESPACE_DECL
