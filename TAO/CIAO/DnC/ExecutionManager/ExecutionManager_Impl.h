//$Id$
/*=======================================================================
 *
 * @file ExecutionManager_Impl.h
 *
 * This file contains implementation for :
 * Deployment::ExecutionManager interface.
 *
 * @auther Tao Lu <lu@dre.vanderbilt.edu>
 *
 *======================================================================*/

#ifndef EXECUTIONMANAGER_IMPL_H
#define EXECUTIONMANAGER_IMPL_H

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "ace/Active_Map_Manager_T.h"
#include "ace/Synch_Traits.h"
#include "../DeploymentS.h"
#include "Deployment_Configuration.h"

/**
 *
 * @class ExecutionManager_Impl
 *
 * @brief This class implements the ExecutionManger. ExecutionManager starts
 * the execution process after the planning stage.
 *
 * Serval assumptions I made now:
 *
 * 1. There are 1 ExecutionManager per application. => AssemblyFactory
 *
 * 2. This interface will be brought up at or before the actual execution
 *    of an application starts.
 *
 * 3. No XML parsing is be done in this interface, the requrest will be
 *    forwarded to the DomainApplicationManager.
 *
 **/


/*@@ Note: Right now we have tow Deployment namespace:
 *
 *   1. ::Components::Deployment => from the old spec
 *   2. ::Deployment => from the new spec.
 *
 *   I plan to keep the new spec seperated from the old one
 *   until I figure out how much of the "pure" code reuse
 *   we could have in the DnC implementation.
 **/

namespace CIAO
{
  class ExecutionManager_Impl :
    public virtual POA_Deployment::ExecutionManager,
    public virtual PortableServer::RefCountServantBase
  {

  public:

    // Default constructor.
    ExecutionManager_Impl (CORBA::ORB_ptr orb,
			   PortableServer::poa_ptr poa,
			   char * config_file
			   ACE_ENV_ARG_DECL);

    // Default destructor.
    ~ExecutionManager_Impl ();


    // Generate an ApplicationManager factory.
    virtual DomainApplicationManager_ptr
    preparePlan
      (
        const Deployment::DeploymentPlan & plan,
        CORBA::Boolean commitResources
        ACE_ENV_ARG_DECL_WITH_DEFAULTS
      )
      ACE_THROW_SPEC ((
	       CORBA::SystemException,
	       Deployment::ResourceNotAvailable,
	       Deployment::PlanError,
	       Deployment::StartError
      ));

    // Return a set of the currently held DomainApplicationMnager.
    virtual Components::Deployment::DomainApplicationManagers *
    getManagers (ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)
      ACE_THROW_SPEC ((
        CORBA::SystemException
      ));


    // Destroy a specific manager.
    virtual void
    destroyManager
      (
	Deployment::DomainApplicationManager_ptr manager
        ACE_ENV_ARG_DECL_WITH_DEFAULTS
      )
      ACE_THROW_SPEC ((
	CORBA::SystemException,
        Components::Deployment::StopError
      ));


    /*==================================================================
     * Below are CIAO specific helper member functions.
     * They are ported from the AssemblyFactory_Impl class from
     * the old DnC spec.
     *================================================================*/

    /* Initialize the ExecutionManager with a configuration file.
     * @This method is also responsible for:
     * 1. initialize targetmanager
     *
     * //2. initialize the nodelist (daemon list)
     * This step has been postponed to the DomainApplicationMananger.
     *
     * 3. .....more?
     **/
    int init (ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)
      ACE_THROW_SEPC ((CORBA::SystemExecption));

    // Some other helper funtions might be really helpful in the future.
    // For example for GUI support..
    // 1. Lookup for DomainApplicationManager.
    // 2. Lookup for NodeManager.

  protected:

    // Cached last created DomainApplicationManager Factory.
    Deployment::DomainApplicationManager_var
    dam_cache_;

    // Cached TargetManager.
    Deployment::TargetManager_var tm_cache_;

    // Cached logger
    Deployment::Logger logger_cache_;

    // Keep a list of managed DomainApplicationManager objects.
    Deployment::DomainApplicationManagers_var
    dam_list_;

    /// Keep a list of managed NodeManager objects.
    ACE_Active_Map_Manager
    <Deployment::NodeManager_var>
    nodemanager_map_;

    // Deployment Configuration Info (INFO of Nodes for now)
    //CIAO_Deployment_Configuration deployment_config_;
    CORBA::String_var deployment_config_file_;

    // Cached ORB pointer
    CORBA::ORB_var orb_;

    // Cached POA pointer
    PortableServer::POA_var poa_;

    // Synchronize mutex. This is just a place holder, this interface
    // is supposed to run in singl thread mode only for now.
    ACE_SYNCH_MUTEX lock_;
  };

};

#if defined (__ACE_INLINE__)
# include "ExecutionManager_Impl.inl"
#endif /*__ACE_INLINE__*/

#include /**/ "ace/post.h"
#endif /* EXECUTIONMANAGER_IMPL_H */
