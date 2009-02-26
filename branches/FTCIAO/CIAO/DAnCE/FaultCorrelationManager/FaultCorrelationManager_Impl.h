// -*- C++ -*-

//=============================================================================
/**
 *  @file    FaultCorrelationManager_Impl.h
 *
 *  $Id$
 *
 *  @author Friedhelm Wolf <fwolf@dre.vanderbilt.edu>
 */
//=============================================================================

#ifndef FAULTCORRELATIONMANAGER_IMPL_H_
#define FAULTCORRELATIONMANAGER_IMPL_H_

#include "ace/Map_Manager.h"
#include "tao/ORB.h"
#include "FaultCorrelationManager_export.h"
#include "Deployment/FaultCorrelationManagerS.h"
#include "Deployment/Deployment_common.h"
#include "Deployment/Deployment_ExecutionManagerC.h"
//#include "Deployment/Deployment_DomainApplicationManagerC.h"

namespace DAnCE
{
  /**
   *  @brief Implementation of a manager to provide fail-stop behavior
   *         for component deployments.
   */
  class FAULTCORRELATIONMANAGER_Export FaultCorrelationManager_Impl : 
    public virtual POA_FTDeployment::FaultCorrelationManager
  {
  public:
    // the fixed listener port is caused by the usage of CCM Object locator
    FaultCorrelationManager_Impl (CORBA::ORB_ptr orb,
                                  Deployment::ExecutionManager_ptr exec_mgr,
                                  const PROPERTY_MAP &options);

    virtual ~FaultCorrelationManager_Impl();

    virtual void stop_failver_unit (const char * fou_id);

    // methods from inherited interfaces

    // FaultNotification
    virtual void proc_failure (const char * object_id,
                               const char * node_id);

    // ExecutionManager
    virtual ::Deployment::DomainApplicationManager_ptr preparePlan (
        const ::Deployment::DeploymentPlan & plan,
        ::Deployment::ResourceCommitmentManager_ptr resourceCommitment);
    
    virtual ::Deployment::DomainApplicationManagers * getManagers (
        void);

    virtual void destroyManager (
        ::Deployment::DomainApplicationManager_ptr manager);

  private:

    void process_deployment_plan (const Deployment::DeploymentPlan & plan);

  private:
    typedef ACE_Map_Manager<ACE_CString, 
                            Deployment::DomainApplicationManager_var, 
                            ACE_Null_Mutex> TDomainManagers;

    typedef ACE_Map_Manager<ACE_CString,
                            ACE_CString,
                            ACE_Null_Mutex> TStringMap;

    typedef TStringMap TInstancesOfPlan;
    typedef TStringMap TNodeComponents;

  private:
    CORBA::ORB_var orb_;

    Deployment::ExecutionManager_var exec_mgr_;

    PROPERTY_MAP properties_;

    TDomainManagers dams_;

    TInstancesOfPlan instances_;

    TNodeComponents nodes_;
  };
}

#endif /* FAULTCORRELATIONMANAGER_IMPL_H_ */
