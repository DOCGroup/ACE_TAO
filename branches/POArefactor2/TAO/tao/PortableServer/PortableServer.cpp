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
      TAO::Portable_Server::ace_svc_desc_ORB_CTRL_Thread_Policy
    );

  ACE_Service_Config::process_directive (
      TAO::Portable_Server::ace_svc_desc_SINGLE_THREAD_Thread_Policy
    );

  ACE_Service_Config::process_directive (
      TAO::Portable_Server::ace_svc_desc_Transient_Lifespan_Policy
    );

  ACE_Service_Config::process_directive (
      TAO::Portable_Server::ace_svc_desc_Persistent_Lifespan_Policy
    );

  ACE_Service_Config::process_directive (
      TAO::Portable_Server::ace_svc_desc_System_IdAssignment_Policy
    );

  ACE_Service_Config::process_directive (
      TAO::Portable_Server::ace_svc_desc_User_IdAssignment_Policy
    );

  ACE_Service_Config::process_directive (
      TAO::Portable_Server::ace_svc_desc_Multiple_IdUniqueness_Policy
    );

  ACE_Service_Config::process_directive (
      TAO::Portable_Server::ace_svc_desc_Unique_IdUniqueness_Policy
    );

  ACE_Service_Config::process_directive (
      TAO::Portable_Server::ace_svc_desc_No_Implicit_Implicit_Activation_Policy
    );

  ACE_Service_Config::process_directive (
      TAO::Portable_Server::ace_svc_desc_Implicit_Implicit_Activation_Policy
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

  ACE_Service_Config::process_directive (
      TAO::Portable_Server::ace_svc_desc_ThreadStrategySingle
    );

  return
    ACE_Service_Config::process_directive (
        ace_svc_desc_TAO_Object_Adapter_Factory
      );
}
