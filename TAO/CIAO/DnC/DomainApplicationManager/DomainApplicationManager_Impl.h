//==================================================================
/**
 *  @file DomainApplicationManager_Impl.h
 *
 *  $Id$
 *
 *  This file contains implementation for the servant of
 *  Deployment::DomainApplicationManager.
 *
 *  @author Gan Deng <gan.deng@vanderbilt.edu>
 */
//=====================================================================

#ifndef CIAO_DOMAINAPPLICATIONMANAGER_IMPL_H
#define CIAO_DOMAINAPPLICATIONMANAGER_IMPL_H
#include /**/ "ace/pre.h"

#include "ace/config-all.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */


#include "Core/Object_Set_T.h"
#include "Core/DeploymentS.h"

#include "tao/Valuetype/ValueBase.h"
#include "tao/Valuetype/Valuetype_Adapter_Impl.h"

namespace CIAO
{
  /**
   * @class DomainApplicationManager_Impl
   *
   * @brief A servant for the Deployment::DomainApplicationManager interface.
   * The DomainApplicationManager is responsible for deploying an application
   * on the domain level, i.e. across nodes. It specializes the ApplicationManager 
   * abstract interface.
   */
  //class CIAO_DOMAINAPPLICATIONMANAGER_Export DomainApplicationManager_Impl
  class DomainApplicationManager_Impl
    : public virtual POA_Deployment::DomainApplicationManager,
      public virtual PortableServer::RefCountServantBase
  {
  public:
    /// Constructor
    DomainApplicationManager_Impl (CORBA::ORB_ptr orb,
				                           PortableServer::POA_ptr poa,
                                   Deployment::TargetManager_ptr manager,
                                   Deployment::DeploymentPlan & plan,
                                   const char * deployment_file);

    /// Destructor
    virtual ~DomainApplicationManager_Impl (void);

    /*===========================================================
     * Below are helper methods for the DomainApplicationManager
     *
     *============================================================*/
    /**
     * Initialize the DomainApplicationManager. 
     * (1) Set the total number of child plans.
     * (2) Set the list of NodeManager names, which is an array of strings.
     *     The <node_manager_names> is a pointer to an array of ACE_CString 
     *     objets, which is allocated by the caller and deallocated by the
     *     DomainApplicationManager destructor.
     * (3) Check the validity of the deployment plan and the deployment
     *     information data file.
     * (4) Call split_plan () member function.
     * (5) Invoke all the preparePlan () operations on all the corresponding
     *     NodeManagers with child plans.
     *     
     *
     * @@ What else do we need to initialize here?
     */
    void init ()
      ACE_THROW_SPEC ((Deployment::ResourceNotAvailable,
                       Deployment::StartError,
                       Deployment::PlanError));

    /*===========================================================
     * Below are operations from the DomainApplicationManager
     *
     *============================================================*/

    /** 
     * Executes the application, but does not start it yet. Users can optionally provide
     * launch-time configuration properties to override properties that are part of the
     * plan. Raises the InvalidProperty exception if a configuration property 
     * is invalid. Raises the StartError exception if an error occurs
     * during launching. Raises the ResourceNotAvailable exception if the
     * commitResources parameter to the prepare operation of the
     * ExecutionManager was true, if late resource allocation is used, and 
     * one of the requested resources is not available.
     */
    virtual void startLaunch (const ::Deployment::Properties & configProperty,
                              ::CORBA::Boolean start
                              ACE_ENV_ARG_DECL_WITH_DEFAULTS)
      ACE_THROW_SPEC ((CORBA::SystemException,
                       ::Deployment::ResourceNotAvailable,
                       ::Deployment::StartError,
                       ::Deployment::InvalidProperty));

    /** 
     * The second step in launching an application in the domain-level.
     * If the start parameter is true, the application is started as well. 
     * Raises the StartError exception if launching or starting the application 
     * fails.
     */
    virtual void finishLaunch (::CORBA::Boolean start
                               ACE_ENV_ARG_DECL_WITH_DEFAULTS)
      ACE_THROW_SPEC ((CORBA::SystemException,
                       ::Deployment::StartError));

    /**
     * Starts the application. Raises the StartError exception if starting the 
     * application fails.
     */
    virtual void start (ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)
      ACE_THROW_SPEC ((CORBA::SystemException,
                       ::Deployment::StartError));

    /**
     * Terminates a running application. Raises the StopError exception if an error
     * occurs during termination. Raises the InvalidReference exception if the appliction
     * referen is unknown.
     */
    virtual void destroyApplication (ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)
      ACE_THROW_SPEC ((CORBA::SystemException,
                       ::Deployment::StopError));

    /**
     * Returns the DeploymentPlan associated with this ApplicationManager.
     */
    virtual ::Deployment::DeploymentPlan * getPlan (ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)
      ACE_THROW_SPEC ((CORBA::SystemException));

  protected:
    /**
     * Check whether all the NodeManager names are present in the
     * deployment information data file.
     */
    bool check_validity ();

    /**
     * Split the global (domain-level) deployment plan to a set of 
     * local (node-level) deployment plans. The set of local plans
     * are cached in the ACE hash map member variable.
     */
    int split_plan ();

  protected:
    /// location of the Domainapplication
    CORBA::String_var domainapp_path_;

    /// Keep a pointer to the managing ORB serving this servant.
    CORBA::ORB_var orb_;

    /// Keep a pointer to the managing POA.
    PortableServer::POA_var poa_;

    /// Cache a object reference to this servant.
    Deployment::DomainApplicationManager_var objref_;

    /// Cache the ior of the previous reference
    CORBA::String_var ior_;

    /// Cached TargetManager.
    Deployment::TargetManager_var target_manager_;

    /// Cached deployment plan for the particular domain.
    /// The plan will be initialized when init is called.
    Deployment::DeploymentPlan & plan_;

    /// Cached child plans.
    //Deployment::DeploymentPlan * child_plan_;

    /// Total number of child deployment plans.
    int num_child_plans_;

    /// The list of node manager names, each of them
    /// corresponds to one child plan.
    ACE_CString * node_manager_names_;

    /// Cached child plans in a map.
    /// Key: NodeManager name.
    /// Value: child deployment plan with _var type.
    ACE_Hash_Map_Manager_Ex<ACE_CString,
                            ::Deployment::DeploymentPlan_var,
                            ACE_Hash<ACE_CString>,
                            ACE_Equal_To<ACE_CString>,
                            ACE_Null_Mutex> child_plans_info_;

    /// The deployment information data file.
    const char * deployment_file_;

    /// Deployment Configuration info, which contains the deployment topology.
    Deployment_Configuration &deployment_config_;
  };
}

#include /**/ "ace/post.h"
#endif /* CIAO_DOMAINAPPLICATIONMANAGER_IMPL_H */
