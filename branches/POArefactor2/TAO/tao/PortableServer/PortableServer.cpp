/* -*- C++ -*- */

//=============================================================================
/**
 *  @file    PortableServer.cpp
 *
 *  $Id$
 *
 *  @author Carlos O'Ryan <coryan@uci.edu>
 */
//=============================================================================


#include "tao/PortableServer/PortableServer.h"
#include "tao/PortableServer/Object_Adapter_Factory.h"

#include "tao/PortableServer/ThreadPolicyFactoryImpl.h"
#include "tao/PortableServer/LifespanPolicyFactoryImpl.h"
#include "tao/PortableServer/IdAssignmentPolicyFactoryImpl.h"
#include "tao/PortableServer/IdUniquenessPolicyFactoryImpl.h"
#include "tao/PortableServer/ImplicitActivationPolicyFactoryImpl.h"
#include "tao/PortableServer/RequestProcessingPolicyFactoryImpl.h"
#include "tao/PortableServer/ServantRetentionPolicyFactoryImpl.h"

#include "tao/PortableServer/ThreadStrategyFactoryImpl.h"
#include "tao/PortableServer/LifespanStrategyFactoryImpl.h"
#include "tao/PortableServer/IdAssignmentStrategyFactoryImpl.h"
#include "tao/PortableServer/IdUniquenessStrategyFactoryImpl.h"
#include "tao/PortableServer/ImplicitActivationStrategyFactoryImpl.h"
#include "tao/PortableServer/RequestProcessingStrategyFactoryImpl.h"
#include "tao/PortableServer/ServantRetentionStrategyFactoryImpl.h"

#include "tao/PortableServer/ThreadPolicyValueORBControl.h"
#include "tao/PortableServer/ThreadPolicyValueSingle.h"

#include "tao/PortableServer/ThreadStrategyORBControl.h"
#include "tao/PortableServer/ThreadStrategySingle.h"

#include "tao/PortableServer/LifespanPolicyValueTransient.h"
#include "tao/PortableServer/LifespanPolicyValuePersistent.h"

#include "tao/PortableServer/IdAssignmentPolicyValueSystem.h"
#include "tao/PortableServer/IdAssignmentPolicyValueUser.h"

#include "tao/PortableServer/IdUniquenessPolicyValueMultiple.h"
#include "tao/PortableServer/IdUniquenessPolicyValueUnique.h"

#include "tao/PortableServer/ImplicitActivationPolicyValueExplicit.h"
#include "tao/PortableServer/ImplicitActivationPolicyValueImplicit.h"

#include "tao/PortableServer/RequestProcessingPolicyValue.h"

#include "tao/PortableServer/ServantRetentionPolicyValueRetain.h"
#include "tao/PortableServer/ServantRetentionPolicyValueNonRetain.h"

ACE_RCSID (PortableServer,
           PortableServer,
           "$Id$")

int
TAO_POA_Initializer::init (void)
{
  ACE_Service_Config::process_directive (
      TAO::Portable_Server::ace_svc_desc_ThreadPolicyFactoryImpl
    );

  ACE_Service_Config::process_directive (
      TAO::Portable_Server::ace_svc_desc_LifespanPolicyFactoryImpl
    );

  ACE_Service_Config::process_directive (
      TAO::Portable_Server::ace_svc_desc_IdAssignmentPolicyFactoryImpl
    );

  ACE_Service_Config::process_directive (
      TAO::Portable_Server::ace_svc_desc_IdUniquenessPolicyFactoryImpl
    );

  ACE_Service_Config::process_directive (
      TAO::Portable_Server::ace_svc_desc_ImplicitActivationPolicyFactoryImpl
    );

  ACE_Service_Config::process_directive (
      TAO::Portable_Server::ace_svc_desc_RequestProcessingPolicyFactoryImpl
    );

  ACE_Service_Config::process_directive (
      TAO::Portable_Server::ace_svc_desc_ServantRetentionPolicyFactoryImpl
    );

  ACE_Service_Config::process_directive (
      TAO::Portable_Server::ace_svc_desc_Default_Servant_Request_Processing_Policy
    );

  ACE_Service_Config::process_directive (
      TAO::Portable_Server::ace_svc_desc_Servant_Manager_Request_Processing_Policy
    );

  ACE_Service_Config::process_directive (
      TAO::Portable_Server::ace_svc_desc_Active_Object_Map_Request_Processing_Policy
    );

  ACE_Service_Config::process_directive (
      TAO::Portable_Server::ace_svc_desc_Non_Retain_Servant_Retention_Policy
    );

  ACE_Service_Config::process_directive (
      TAO::Portable_Server::ace_svc_desc_Retain_Servant_Retention_Policy
    );

  // Policy Values

  ACE_Service_Config::process_directive (
      TAO::Portable_Server::ace_svc_desc_ThreadPolicyValueORBControl
    );

#if (TAO_HAS_MINIMUM_POA == 0)
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
      TAO::Portable_Server::ace_svc_desc_IdAssignmentPolicyValueSystem
    );

  ACE_Service_Config::process_directive (
      TAO::Portable_Server::ace_svc_desc_IdAssignmentPolicyValueUser
    );

  ACE_Service_Config::process_directive (
      TAO::Portable_Server::ace_svc_desc_IdUniquenessPolicyValueMultiple
    );

  ACE_Service_Config::process_directive (
      TAO::Portable_Server::ace_svc_desc_IdUniquenessPolicyValueUnique
    );

  ACE_Service_Config::process_directive (
      TAO::Portable_Server::ace_svc_desc_ImplicitActivationPolicyValueExplicit
    );

  ACE_Service_Config::process_directive (
      TAO::Portable_Server::ace_svc_desc_ImplicitActivationPolicyValueImplicit
    );

  // Strategy factories

  ACE_Service_Config::process_directive (
      TAO::Portable_Server::ace_svc_desc_ThreadStrategyFactoryImpl
    );

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
      TAO::Portable_Server::ace_svc_desc_ServantRetentionStrategyFactoryImpl
    );

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
