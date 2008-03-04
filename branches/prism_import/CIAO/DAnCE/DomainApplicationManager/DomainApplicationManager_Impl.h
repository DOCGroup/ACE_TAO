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

#include "DAnCE/Deployment/Deployment_DomainApplicationManagerS.h"
#include "DAnCE/Deployment/Deployment_common.h"

#include "tao/Valuetype/ValueBase.h"
#include "tao/Valuetype/Valuetype_Adapter_Impl.h"

#include "ciao/CIAO_common.h"
#include "DAnCE/Deployment/Deployment_EventsC.h"

#include "Deployment_Configuration.h"
#include "DomainApplicationManager_Export.h"

namespace CIAO
{
  namespace Execution_Manager
  {
    class Execution_Manager_Impl;
  }

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

    /// External_Connections means we search all the connections including
    /// the connectiosn for external/shared components of this plan which hold
    /// port objrefs of components within this plan
    enum Connection_Search_Type
      {
        External_Connections,
        Internal_Connections
      };

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
      ::Deployment::DeploymentPlan_var old_child_plan_;
    } Chained_Artifacts;

    /// Constructor
    DomainApplicationManager_Impl (CORBA::ORB_ptr orb,
                                   PortableServer::POA_ptr poa,
                                   Deployment::TargetManager_ptr manager,
                                   Execution_Manager::Execution_Manager_Impl * em,
                                   const Deployment::DeploymentPlan &plan,
                                   const char * deployment_file);


    /*===========================================================
     * Below are helper methods for the DomainApplicationManager
     *
     *============================================================*/
    /**
     * Initialize the DomainApplicationManager.
     * @para em A pointer to the ExecutionManager servant C++ object.
     *
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
    void init ();

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
     * Fetch the NodeApplication object reference based on the NodeManager name.
     */
    virtual Deployment::NodeApplication_ptr get_node_app (
        const char * node_name);

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
                   ::CORBA::Boolean start);

    /**
     * The second step in launching an application in the
     * domain-level.  If the start parameter is true, the application
     * is started as well.  Raises the StartError exception if
     * launching or starting the application fails.
     */
    virtual void finishLaunch (CORBA::Boolean start,
                               CORBA::Boolean is_ReDAC);

    /**
     * The last step in launching an application in the
     * domain-level.  We establish connection bindings
     * for external/shared components of this deployment plan
     * components.
     * Internally, this operation will invoke some operations
     * on ExecutionManager to finish up this task.
     */
    virtual void post_finishLaunch (void);

    virtual void passivate_shared_components (void);

    virtual void activate_shared_components (void);

    /**
     * Starts the application. Raises the StartError exception if
     * starting the application fails.
     */
    virtual void start ();

    /**
     * Terminates a running application. Raises the StopError
     * exception if an error occurs during termination. Raises the
     * InvalidReference exception if the appliction reference is
     * unknown.
     */
    virtual void destroyApplication ();

    /**
     * Returns the DeploymentPlan associated with this ApplicationManager.
     */
    virtual ::Deployment::DeploymentPlan * getPlan ();

    /**
     * This is a helper function to destroy the NodeAppManager.
     * Since we don't want to do so in the destructor so we will
     * ask the ExecutionManager to do this on us when the same IDL
     * op invoked on it. This is part of the result for merging DAM
     * with DA.
     */
    virtual void destroyManager ();

    // The input parameter is a *new_plan* which has the
    // same UUID of the existing running plan.
    virtual void
    perform_redeployment (
      const Deployment::DeploymentPlan & plan);

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
     * Construct <Component_Binding_Info> struct for the component instance.
     *
     * @param name component instance name
     * @param child_uuid child plan uuid string
     */
    CIAO::Component_Binding_Info *
      populate_binding_info (const ACE_CString& name,
                             const ACE_CString& child_uuid);

    /**
     * Construct <Component_Binding_Info> struct for the component instance.
     * Fetch the plan_uuid info from the internally cached shared component
     * list.
     *
     * @param name component instance name
     */
    CIAO::Component_Binding_Info *
      populate_binding_info (const ACE_CString& name);

    /**
     * Contact each NodeManager to get shared compnents information
     * and then update its internal cache.
     */
    void synchronize_shared_components_with_node_managers (void);

    /**
     * A helper function to add a list of shared components into
     * the cached shared component list.
     *
     * @param shared A list of shared components to be added.
     */
    void add_shared_components (const Deployment::ComponentPlans & shared);

    /**
     * A private function to check whether a component is in the shared
     * component list.
     *
     * @param name The name of a component instance.
     */
    bool is_shared_component (const char * name);

    /**
     * Cache the incoming connections, which is a sequence of Connections,
     * into the <all_connections_> list.
     */
    void add_connections (const Deployment::Connections & incoming_conn);

    /**
     * Given a child deployment plan, find the <Connections> sequence
     * of the "providedReference" for the component instances in the
     * child deployment plan as Receiver side.
     * By default, we search in the new plan.
     *
     * If <is_getting_all_connections> is false, then we only
     * search for "new connections" (valid for ReDaC case only).
     * Otherwise, we will search for both new connections and those
     * already existing connections.
     */
    Deployment::Connections *
    get_outgoing_connections (const Deployment::DeploymentPlan &plan,
                              bool is_getting_all_connections = true,
                              bool is_search_new_plan = true,
                              Connection_Search_Type t = Internal_Connections);

    /// This is a helper function to find the connection for a component.
    bool
    get_outgoing_connections_i (const char * instname,
                                Deployment::Connections & retv,
                                bool is_ReDAC,
                                bool is_search_new_plan);

    bool
    populate_connection_for_binding (
        const char * instname,
        const Deployment::PlanConnectionDescription & binding,
        const Deployment::DeploymentPlan & plan,
        Deployment::Connections & retv);

    bool
    handle_es_connection (
        const char * instname,
        const Deployment::PlanConnectionDescription & binding,
        const Deployment::DeploymentPlan & plan,
        Deployment::Connections & retv);

    bool
    handle_direct_connection (
        const char * instname,
        const Deployment::PlanConnectionDescription & binding,
        const Deployment::DeploymentPlan & plan,
        Deployment::Connections & retv);

    /// Dump connections, a static method
    void dump_connections (const ::Deployment::Connections & connections);

    /// Check whether a connection already exists in the cached old plan
    bool already_exists (const Deployment::PlanConnectionDescription & conn);

    /// Remove those appeared in <right> from the <left>
    Deployment::Connections *
    subtract_connections (const Deployment::Connections & left,
                          const Deployment::Connections & right);

    void
    purge_connections (Deployment::Connections_var & connections,
                       const char * inst);

    /**
     * The first step in finish_launching an application in the
     * domain-level.  We install all the CIAO_Event_Service objects
     * as specified in the DeploymentPlan.
     * Internally, this operation will invoke an operation on each cached
     * NodeApplication object.
     */
    virtual void install_all_es (void);

    /**
     * Add all CIAO_Event_Service objects into the cached map.
     */
    virtual void
    add_es_to_map (const char* node_name,
                   CIAO::CIAO_Event_Service * ciao_es);

    /**
     * Get the actual event connection QoS properties based on the
     * deployment requirement. The deployment requirement only specifies
     * an identifier/reference to the EventServiceDeploymentDescriptions.
     */
    virtual const Deployment::Properties *
    get_connection_QoS_configuration (
      const Deployment::Requirement & requirement);

  protected:
    /// location of the Domainapplication
    CORBA::String_var domainapp_path_;

    /// Keep a pointer to the managing ORB serving this servant.
    CORBA::ORB_var orb_;

    /// Keep a pointer to the managing POA.
    PortableServer::POA_var poa_;

    /// Pointer to the ExecutionManager_Impl "singleton" servant object
    /// We could do this because ExecutionManager and DomainApplicationManager
    /// are always collocated in the same process, so we don't have
    /// to pass CORBA object reference back and forth.
    Execution_Manager::Execution_Manager_Impl * execution_manager_;

    /// Cache a list of shared components
    Deployment::ComponentPlans_var shared_;

    /// Cache the ior of the previous reference
    CORBA::String_var ior_;

    /// Cached TargetManager.
    Deployment::TargetManager_var target_manager_;

    /// Cached deployment plan for the particular domain.
    /// The plan will be initialized when init is called.
    Deployment::DeploymentPlan plan_;

    /// Cached old deployment plan, i.e., before redeployment
    Deployment::DeploymentPlan old_plan_;

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

    /// Cached information of all the CIAO_Event_Service objects within
    /// the deployment plan
    /// Key: the string identifier of the CIAO_Event_Service
    /// Value: the object reference of the CIAO_Event_Service
    typedef ACE_Hash_Map_Manager_Ex<ACE_CString,
                                    CIAO::CIAO_Event_Service_var,
                                    ACE_Hash<ACE_CString>,
                                    ACE_Equal_To<ACE_CString>,
                                    ACE_Null_Mutex> Event_Service_Table;

    Event_Service_Table es_map_;

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
    bool is_redeployment_;

    /// Cache the CIAO_Event_Service deployment description
    CIAO::DAnCE::EventServiceDeploymentDescriptions_var esd_;
  };
}

#if defined (__ACE_INLINE__)
# include "DomainApplicationManager_Impl.inl"
#endif /* __ACE_INLINE__ */

#include /**/ "ace/post.h"
#endif /* CIAO_DOMAINAPPLICATIONMANAGER_IMPL_H */
