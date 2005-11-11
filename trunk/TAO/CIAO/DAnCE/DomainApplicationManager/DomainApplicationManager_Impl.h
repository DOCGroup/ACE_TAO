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
 *  @author Arvind S. Krishna <arvindk@dre.vanderbilt.edu>
 *  @author Tao Lu <lu@dre.vanderbilt.edu>
 */
//=====================================================================

#ifndef CIAO_DOMAINAPPLICATIONMANAGER_IMPL_H
#define CIAO_DOMAINAPPLICATIONMANAGER_IMPL_H
#include /**/ "ace/pre.h"

#include "ace/config-all.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */


#include "ace/Hash_Map_Manager.h"
#include "ace/Vector_T.h"
#include "ace/Functor.h"
#include "ace/OS_NS_string.h"
#include "DeploymentS.h"

#include "tao/Valuetype/ValueBase.h"
#include "tao/Valuetype/Valuetype_Adapter_Impl.h"

#include "Deployment_Configuration.h"
#include "DomainApplicationManager_Export.h"
#include "ciao/CIAO_common.h"

namespace CIAO
{
  /**
   * @class DomainApplicationManager_Impl
   *
   * @brief A servant for the Deployment::DomainApplicationManager
   * interface.  The DomainApplicationManager is responsible for
   * deploying an application on the domain level, i.e. across
   * nodes. It specializes the ApplicationManager abstract interface.
   */
  class DomainApplicationManager_Export DomainApplicationManager_Impl
    : public virtual POA_Deployment::DomainApplicationManager
  {
  public:
    /// Define the type which contains a list of DnC artifacts.
    /// @@ Assumption: Each NodeApplicationManager create only one
    /// NodeApplication when the startLaunch() operation is invoked,
    /// which means all the nodes on that child plan are collocated in
    /// the same process.
    typedef struct _chained_artifacts
    {
      ::Deployment::NodeManager_var node_manager_;
      ::Deployment::DeploymentPlan_var child_plan_;
      ::Deployment::NodeApplicationManager_var node_application_manager_;
      ::Deployment::NodeApplication_var node_application_;
    } Chained_Artifacts;

    /// Constructor
    DomainApplicationManager_Impl (CORBA::ORB_ptr orb,
                                   PortableServer::POA_ptr poa,
                                   Deployment::TargetManager_ptr manager,
                                   const Deployment::DeploymentPlan &plan,
                                   const char * deployment_file)
      ACE_THROW_SPEC ((CORBA::SystemException));


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
    void init (ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)
      ACE_THROW_SPEC ((CORBA::SystemException,
                       Deployment::ResourceNotAvailable,
                       Deployment::StartError,
                       Deployment::PlanError));

    /**
     * set_uuid method on the DomainApplicationManager assigns the
     * UUID of the associated plan to this Manager. This helps in
     * reverse lookup operations such as given a manager, obtaining
     * the UUID associated. The get_uuid method is used in this case.
     */
    void set_uuid (const char * uuid);

    const char * get_uuid () const;

    /*===========================================================
     * Below are operations from the DomainApplicationManager
     *
     *============================================================*/

    /**
     * Executes the application, but does not start it yet. Users can
     * optionally provide launch-time configuration properties to
     * override properties that are part of the plan. Raises the
     * InvalidProperty exception if a configuration property is
     * invalid. Raises the StartError exception if an error occurs
     * during launching. Raises the ResourceNotAvailable exception if
     * the commitResources parameter to the prepare operation of the
     * ExecutionManager was true, if late resource allocation is used,
     * and one of the requested resources is not available.
     */
    virtual void
      startLaunch (const ::Deployment::Properties & configProperty,
                   ::CORBA::Boolean start
                   ACE_ENV_ARG_DECL_WITH_DEFAULTS)
      ACE_THROW_SPEC ((CORBA::SystemException,
                       ::Deployment::ResourceNotAvailable,
                       ::Deployment::StartError,
                       ::Deployment::InvalidProperty));

    /**
     * The second step in launching an application in the
     * domain-level.  If the start parameter is true, the application
     * is started as well.  Raises the StartError exception if
     * launching or starting the application fails.
     */
    virtual void finishLaunch (::CORBA::Boolean start
                               ACE_ENV_ARG_DECL_WITH_DEFAULTS)
      ACE_THROW_SPEC ((CORBA::SystemException,
                       ::Deployment::StartError));

    /**
     * Starts the application. Raises the StartError exception if
     * starting the application fails.
     */
    virtual void start (ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)
      ACE_THROW_SPEC ((CORBA::SystemException,
                       ::Deployment::StartError));

    /**
     * Terminates a running application. Raises the StopError
     * exception if an error occurs during termination. Raises the
     * InvalidReference exception if the appliction referen is
     * unknown.
     */
    virtual void destroyApplication (ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)
      ACE_THROW_SPEC ((CORBA::SystemException,
                       ::Deployment::StopError));

    /**
     * Returns the DeploymentPlan associated with this ApplicationManager.
     */
    virtual ::Deployment::DeploymentPlan * getPlan (ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)
      ACE_THROW_SPEC ((CORBA::SystemException));

    /**
     * This is a helper function to destroy the NodeAppManager.
     * Since we don't want to do so in the destructor so we will
     * ask the ExecutionManager to do this on us when the same IDL
     * op invoked on it. This is part of the result for merging DAM
     * with DA.
     */
    virtual void destroyManager (ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)
      ACE_THROW_SPEC ((CORBA::SystemException,
                       Deployment::StopError));

    // The input parameter is a *new_plan* which has the
    // same UUID of the existing running plan.
    virtual void 
    perform_redeployment (
      const Deployment::DeploymentPlan & plan
      ACE_ENV_ARG_DECL_WITH_DEFAULTS)
      ACE_THROW_SPEC ((CORBA::SystemException,
                        Deployment::PlanError,
                        Deployment::InstallationFailure,
                        Deployment::UnknownImplId,
                        Deployment::ImplEntryPointNotFound,
                        Deployment::InvalidConnection,
                        ::Components::RemoveFailure));

  protected:
    /// Destructor
    virtual ~DomainApplicationManager_Impl (void);

    /**
     * (1) Parse the global deployment plan, get the total number of
     *     child plans included in the global plan, and get the list of
     *     names of NodeManagers for each child plan.
     * (2) Check whether all the NodeManager names are present in the
     *     deployment information data file.
     */
    bool get_plan_info (void);

    /**
     * Split the global (domain-level) deployment plan to a set of
     * local (node-level) deployment plans. The set of local plans
     * are cached in the ACE hash map member variable.
     * Also, fill in the <node_manager_> field of the Chained_Artifacts
     * struct.
     */
    int split_plan (void);


    /**
     * Cache the incoming connections, which is a sequence of Connections,
     * into the <all_connections_> list.
     */
    void add_connections (const Deployment::Connections & incoming_conn);

    /**
     * Given a child deployment plan, find the <Connections> sequence
     * of the "providedReference" for the component instances in the
     * child deployment plan as Receiver side.
     */
    Deployment::Connections *
    get_outgoing_connections (const Deployment::DeploymentPlan &plan
			      ACE_ENV_ARG_DECL);

    /// This is a helper function to find the connection for a component.
    bool
    get_outgoing_connections_i (const char * instname,
                                Deployment::Connections & retv
				ACE_ENV_ARG_DECL)
      ACE_THROW_SPEC ((Deployment::StartError));

    /// Dump connections, a static method
    void dump_connections (const ::Deployment::Connections & connections);

  protected:
    /// location of the Domainapplication
    CORBA::String_var domainapp_path_;

    /// Keep a pointer to the managing ORB serving this servant.
    CORBA::ORB_var orb_;

    /// Keep a pointer to the managing POA.
    PortableServer::POA_var poa_;

    /// Cache a object reference to this servant.
    /// Deployment::DomainApplicationManager_var objref_;

    /// Cache the ior of the previous reference
    CORBA::String_var ior_;

    /// Cached TargetManager.
    Deployment::TargetManager_var target_manager_;

    /// Cached deployment plan for the particular domain.
    /// The plan will be initialized when init is called.
    Deployment::DeploymentPlan plan_;

    /// Cached child plans.
    //Deployment::DeploymentPlan * child_plan_;

    /// Total number of child deployment plans.
    size_t num_child_plans_;

    /// The list of node manager names, each of them
    /// corresponds to one child plan.
    ACE_Vector<ACE_CString> node_manager_names_;

    /// Cached information for NodeManager, NodeApplicationManager,
    /// NodeApplication, Connections, etc.
    /// Key: NodeManager name with CString type.
    /// Value: Chained_Artifacts struct type.
    typedef ACE_Hash_Map_Manager_Ex<ACE_CString,
                                    Chained_Artifacts,
                                    ACE_Hash<ACE_CString>,
                                    ACE_Equal_To<ACE_CString>,
                                    ACE_Null_Mutex> Chained_Artifacts_Table;

    Chained_Artifacts_Table artifact_map_;

    /// The deployment information data file.
    const char * deployment_file_;

    /// Deployment Configuration info, which contains the deployment topology.
    Deployment_Configuration deployment_config_;

    /// UUID from the Plan
    CORBA::String_var uuid_;

    /// Maintain a list of <Connections>, each of which is obtained by
    /// calling the startLaunch() method on the NodeApplicationManager object.
    ::Deployment::Connections_var all_connections_;

    /// This variable is used to control the execution path of some
    /// member function implementations. The reason is because we want
    /// to avoid unnecessary code duplicate. The default value is "false".
    bool is_redeployment;
  };
}

#if defined (__ACE_INLINE__)
# include "DomainApplicationManager_Impl.inl"
#endif /* __ACE_INLINE__ */

#include /**/ "ace/post.h"
#endif /* CIAO_DOMAINAPPLICATIONMANAGER_IMPL_H */
