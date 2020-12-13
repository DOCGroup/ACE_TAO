#include "PortableServer.h"
#include "Object_Adapter.h"
#include "Object_Adapter_Factory.h"
#include "POA_Current_Factory.h"

#include "ThreadStrategyFactoryImpl.h"
#include "LifespanStrategyFactoryImpl.h"
#include "IdAssignmentStrategyFactoryImpl.h"
#include "IdUniquenessStrategyFactoryImpl.h"
#include "ImplicitActivationStrategyFactoryImpl.h"
#include "RequestProcessingStrategyFactoryImpl.h"
#include "ServantRetentionStrategyFactoryImpl.h"

#include "ThreadStrategyORBControl.h"
#include "ThreadStrategySingle.h"
#include "ThreadStrategySingleFactoryImpl.h"

#include "IdAssignmentStrategySystem.h"
#include "IdAssignmentStrategyUser.h"

#include "IdUniquenessStrategyMultiple.h"
#include "IdUniquenessStrategyUnique.h"

#include "ImplicitActivationStrategyExplicit.h"
#include "ImplicitActivationStrategyImplicit.h"

#include "RequestProcessingStrategyAOMOnly.h"
#include "RequestProcessingStrategyDefaultServant.h"
#include "RequestProcessingStrategyServantManager.h"

#include "ServantRetentionStrategyNonRetainFactoryImpl.h"
#include "ServantRetentionStrategyRetainFactoryImpl.h"

#include "RequestProcessingStrategyDefaultServantFI.h"
#include "RequestProcessingStrategyAOMOnlyFactoryImpl.h"
#include "RequestProcessingStrategyServantActivatorFI.h"
#include "RequestProcessingStrategyServantLocatorFI.h"

#include "IdUniquenessStrategyUniqueFactoryImpl.h"

#include "LifespanStrategyPersistentFactoryImpl.h"
#include "LifespanStrategyTransientFactoryImpl.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

int
TAO_POA_Initializer::init ()
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
      ace_svc_desc_LifespanStrategyFactoryImpl);

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
      ace_svc_desc_ServantRetentionStrategyRetainFactoryImpl);

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
