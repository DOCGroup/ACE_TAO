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
 *  @author William R. Otte <wotte@dre.vanderbilt.edu>
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
#include "ace/Containers.h"

#include "ciao/DomainApplicationManagerS.h"
#include "ciao/Deployment_common.h"

#include "tao/Valuetype/ValueBase.h"
#include "tao/Valuetype/Valuetype_Adapter_Impl.h"

#include "Deployment_Configuration.h"
#include "DomainApplicationManager_Export.h"
#include "ciao/CIAO_common.h"
#include "ciao/Deployment_EventsC.h"

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
    /// Define the type which contains a list of DnC artifacts.
    struct Node_Level_Plan
    {
      ::Deployment::NodeManager_var node_manager_;
      ::Deployment::NodeApplicationManager_var node_application_manager_;
      ::Deployment::DeploymentPlan_var child_plan_;
    };

    /// Constructor
    DomainApplicationManager_Impl (CORBA::ORB_ptr orb,
                                   PortableServer::POA_ptr poa,
                                   Deployment::TargetManager_ptr manager,
                                   const Deployment::DeploymentPlan &plan,
                                   const char * deployment_file)
      throw (CORBA::SystemException);


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
    void init (::Deployment::ResourceCommitmentManager_ptr resourceCommitment)
      throw (CORBA::SystemException,
             Deployment::ResourceNotAvailable,
             Deployment::StartError,
             Deployment::PlanError);
    
    /**
     * set_uuid method on the DomainApplicationManager assigns the
     * UUID of the associated plan to this Manager. This helps in
     * reverse lookup operations such as given a manager, obtaining
     * the UUID associated. The get_uuid method is used in this case.
     */
    void set_uuid (const char * uuid) throw  ();

    const char * get_uuid () const throw ();

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
    virtual ::Deployment::Application_ptr
    startLaunch (const ::Deployment::Properties & configProperty,
                 ::Deployment::Connections_out providedReference)
      throw (::CORBA::SystemException,
             ::Deployment::ResourceNotAvailable,
             ::Deployment::StartError,
             ::Deployment::InvalidProperty,
             ::Deployment::InvalidNodeExecParameter,
             ::Deployment::InvalidComponentExecParameter);

    /**
     * Terminates a running application. Raises the StopError
     * exception if an error occurs during termination. Raises the
     * InvalidReference exception if the appliction referen is
     * unknown.
     */
    virtual void destroyApplication (::Deployment::Application_ptr app)
      throw (CORBA::SystemException,
             ::Deployment::StopError);

    /**
     * Returns the DeploymentPlan associated with this ApplicationManager.
     */
    virtual ::Deployment::DeploymentPlan * getPlan (void)
      throw (CORBA::SystemException);
    
    /**
     * Returns all active applications created by this DAM.
     */
    virtual
    ::Deployment::Applications * getApplications (void)
      throw (::CORBA::SystemException);
    
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
    void get_plan_info (void)
      throw (Deployment::PlanError);

    /**
     * Split the global (domain-level) deployment plan to a set of
     * local (node-level) deployment plans. The set of local plans
     * are cached in the ACE hash map member variable.
     * Also, fill in the <node_manager_> field of the Node_Level_Plan
     * struct.
     */
    void split_plan (void)
      throw (CORBA::NO_MEMORY);
    
    /**
     * Get the actual event connection QoS properties based on the
     * deployment requirement. The deployment requirement only specifies
     * an identifier/reference to the EventServiceDeploymentDescriptions.
     */
    virtual const Deployment::Properties &
    get_connection_QoS_configuration (
      const Deployment::Requirement & requirement);

  protected:
    /// Keep a pointer to the managing ORB serving this servant.
    CORBA::ORB_var orb_;

    /// Keep a pointer to the managing POA.
    PortableServer::POA_var poa_;

    /// Cache the ior of the previous reference
    CORBA::String_var ior_;

    /// Cached TargetManager.
    Deployment::TargetManager_var target_manager_;

    /// Cached deployment plan for the particular domain.
    /// The plan will be initialized when init is called.
    Deployment::DeploymentPlan plan_;

    /// Total number of child deployment plans.
    size_t num_child_plans_;

    /// The list of node manager names, each of them
    /// corresponds to one child plan.
    ACE_Vector<ACE_CString> node_manager_names_;

    /// Cached information for NodeManager, NodeApplicationManager,
    /// NodeApplication, Connections, etc.
    /// Key: NodeManager name with CString type.
    /// Value: Node_Level_Plan struct type.
    typedef ACE_Hash_Map_Manager_Ex<ACE_CString,
                                    Node_Level_Plan,
                                    ACE_Hash<ACE_CString>,
                                    ACE_Equal_To<ACE_CString>,
                                    ACE_Null_Mutex> Node_Level_Plan_Table;
    typedef ACE_Hash_Map_Entry <ACE_CString,
                                Node_Level_Plan> Node_Level_Plan_Entry;

    Node_Level_Plan_Table plan_table_;
    
    typedef ACE_DLList< ::Deployment::DomainApplication_var > DA_List;
    
    DA_List applications_;
    
    /*
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
    */
    /// The deployment information data file.
    const char * deployment_file_;

    /// Deployment Configuration info, which contains the deployment topology.
    Deployment_Configuration deployment_config_;

    /// UUID from the Plan
    CORBA::String_var uuid_;

  };
}

#if defined (__ACE_INLINE__)
# include "DomainApplicationManager_Impl.inl"
#endif /* __ACE_INLINE__ */

#include /**/ "ace/post.h"
#endif /* CIAO_DOMAINAPPLICATIONMANAGER_IMPL_H */
