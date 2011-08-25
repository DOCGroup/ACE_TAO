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

#include <list>
#include <map>
#include <queue>
#include "ace/High_Res_Timer.h"
#include "ace/Hash_Map_Manager_T.h"
#include "ace/Map_Manager.h"
#include "ace/Thread_Mutex.h"
#include "ace/Task.h"
#include "ace/Condition_T.h"
#include "tao/ORB.h"
#include "FaultCorrelationManager_export.h"
#include "FaultCorrelationManagerS.h"
#include "dance/Deployment/Deployment_common.h"
#include "dance/Deployment/Deployment_ExecutionManagerC.h"
#include "orbsvcs/orbsvcs/LWFT/ReplicationManagerC.h"

namespace DAnCE
{
  /**
   *  @brief Implementation of a manager to provide fail-stop behavior
   *         for component deployments.
   */
  class FAULTCORRELATIONMANAGER_Export FaultCorrelationManager_Impl : 
    public virtual POA_DAnCE::FaultCorrelationManager,
    public ACE_Task_Base
  {
  public:
    // the fixed listener port is caused by the usage of CCM Object locator
    FaultCorrelationManager_Impl (CORBA::ORB_ptr orb,
                                  ::Deployment::ExecutionManager_ptr exec_mgr,
                                  ReplicationManager_ptr rep_mgr,
                                  const PROPERTY_MAP &options);

    virtual ~FaultCorrelationManager_Impl();

    virtual void stop_failover_unit (const char * fou_id);

    // methods from inherited interfaces

    // FaultNotification
    virtual void app_failure (
        const char * host,
        const ::FLARE::ApplicationList & applications);

    // ExecutionManager
    virtual ::Deployment::DomainApplicationManager_ptr preparePlan (
        const ::Deployment::DeploymentPlan & plan,
        ::Deployment::ResourceCommitmentManager_ptr resourceCommitment);
    
    virtual ::Deployment::DomainApplicationManagers * getManagers (
        void);

    virtual void destroyManager (
        ::Deployment::DomainApplicationManager_ptr manager);

    virtual void shutdown (void);

    virtual int svc (void);

  private:

    // FaultNotification
    void app_failure_i (const char * host,
                        const char * application);

    void process_deployment_plan (const Deployment::DeploymentPlan & plan,
                                  bool deploy = true);

    char * get_property (const char * name,
                         const Deployment::Properties & properties);

    void add_constraints (const Deployment::DeploymentPlan & plan);

    void remove_constraints (const Deployment::DeploymentPlan & plan);

    RankListConstraints * get_constraints (void);

  private:
    typedef ACE_Map_Manager<ACE_CString, 
                            Deployment::DomainApplicationManager_var, 
                            ACE_Null_Mutex> TDomainManagers;

    typedef ACE_Map_Manager<ACE_CString,
                            ACE_CString,
                            ACE_Null_Mutex> TStringMap;

    typedef std::map<ACE_CString, ACE_CString> TObjectIdMap;

    typedef ACE_Map_Manager<ACE_CString,
                            TObjectIdMap,
                            ACE_Null_Mutex> TNodeMap;

    typedef TStringMap TInstancesOfPlan;

    struct FailureInfo 
    {
      ACE_CString host;
      ACE_CString application;
    };

  typedef std::list<ACE_CString> RANKLIST_CONSTRAINT;

  typedef ACE_Hash_Map_Manager_Ex <
    ACE_CString,
    RANKLIST_CONSTRAINT,
    ACE_Hash<ACE_CString>,
    ACE_Equal_To<ACE_CString>,
    ACE_Null_Mutex> RANKLIST_CONSTRAINT_MAP;

  private:
    CORBA::ORB_var orb_;

    ::Deployment::ExecutionManager_var exec_mgr_;

    ReplicationManager_var rep_mgr_;

    PROPERTY_MAP properties_;

    TDomainManagers dams_;

    TInstancesOfPlan instances_;

    TNodeMap nodes_;

    RANKLIST_CONSTRAINT_MAP constraints_;

    // these parts belong the role as active object
    bool stop_;

    ACE_Thread_Mutex app_failure_lock_;

    ACE_Condition <ACE_Thread_Mutex> new_notification_;

    std::queue <FailureInfo> notification_queue_;

    ACE_High_Res_Timer timer_;

    struct TFouShutdownTime {
      unsigned long shutdown;
      ACE_Time_Value start_shutdown;
      ACE_Time_Value end_shutdown;
      std::string fou_id;
    };
    
    typedef std::list <TFouShutdownTime> SHUTDOWN_TIME_LIST;

    SHUTDOWN_TIME_LIST history_;

    ACE_Thread_Mutex history_lock_;
  };
}

#endif /* FAULTCORRELATIONMANAGER_IMPL_H_ */
