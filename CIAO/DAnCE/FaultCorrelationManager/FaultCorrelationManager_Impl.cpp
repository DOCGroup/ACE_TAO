// -*- C++ -*-

//=============================================================================
/**
 *  @file    FaultCorrelationManager_Impl.cpp
 *
 *  $Id$
 *
 *  @author Friedhelm Wolf <fwolf@dre.vanderbilt.edu>
 */
//=============================================================================

#include "FaultCorrelationManager_Impl.h"

namespace DAnCE
{
  FaultCorrelationManager_Impl::FaultCorrelationManager_Impl (
    CORBA::ORB_ptr orb,
    Deployment::ExecutionManager_ptr exec_mgr,
    const PROPERTY_MAP & options)
    : orb_ (CORBA::ORB::_duplicate (orb)),
      exec_mgr_ (Deployment::ExecutionManager::_duplicate (exec_mgr)),
      properties_ (options.current_size ())
  {
  }

  FaultCorrelationManager_Impl::~FaultCorrelationManager_Impl()
  {
  }

  void
  FaultCorrelationManager_Impl::stop_failver_unit (const char * /* fou_id */)
  {
  }

  void
  FaultCorrelationManager_Impl::proc_failure (const char * /* object_id */,
                                              const char * /* node_id */)
  {
  }

  ::Deployment::DomainApplicationManager_ptr
  FaultCorrelationManager_Impl::preparePlan (
        const ::Deployment::DeploymentPlan & plan,
        ::Deployment::ResourceCommitmentManager_ptr resourceCommitment)
  {
    Deployment::DomainApplicationManager_var dam =
      exec_mgr_->preparePlan (plan, resourceCommitment);

    dams_.bind (plan.UUID.in(), dam.in ());

    this->process_deployment_plan (plan);

    return Deployment::DomainApplicationManager::_duplicate (dam.in ());
  }
    
  ::Deployment::DomainApplicationManagers * 
  FaultCorrelationManager_Impl::getManagers (void)
  {
    return exec_mgr_->getManagers ();
  }

  void
  FaultCorrelationManager_Impl::destroyManager (
    Deployment::DomainApplicationManager_ptr manager)
  {
    Deployment::DeploymentPlan_var plan = manager->getPlan ();

    dams_.unbind (plan->UUID.in ());

    exec_mgr_->destroyManager (manager);
  }

  void
  FaultCorrelationManager_Impl::process_deployment_plan (
    const Deployment::DeploymentPlan & /* plan */)
  {
  }

};
