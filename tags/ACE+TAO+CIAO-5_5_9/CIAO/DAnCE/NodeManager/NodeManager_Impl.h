// -*- C++ -*-
//=============================================================================
/**
 *  @file NodeManager_Impl.h
 *
 *  $Id$
 *
 *  This file contains servant implementation for Deployment:NodeManager
 *  interface. In the current design of the NodeManager, each NodeManager
 *  could manage one or more NodeApplication Managers.
 *  Though, the name intuitively suggests
 *  that there be one NodeManager  for every node, our design, allows
 *  the end-user to have multiple components run on the same node.
 *
 *  @author Arvind S. Krishna <arvindk@dre.vanderbilt.edu>
 *  @author Gan Deng <dengg@dre.vanderbilt.edu>
 */
//=============================================================================


#ifndef CIAO_NODEMANAGER_IMPL_H
#define CIAO_NODEMANAGER_IMPL_H
#include /**/ "ace/pre.h"

#include "Interfaces/NodeManagerDaemonS.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#if defined (_MSC_VER)
# pragma warning(push)
# pragma warning (disable:4250)
#endif /* _MSC_VER */

#include "NodeManager_svnt_export.h"
#include "NAM_Map.h"
#include "MonitorController.h"
#include "ace/Vector_T.h"

namespace CIAO
{
  class NodeApplicationManager_Impl_Base;

  /**
   * @class NodeManager_Impl
   *
   * @brief Servant implementation CIAO's daemon process control
   * interface
   *
   * This class implements the CIAO:NodeManagerDaemon interface.
   *
   */
  class NodeManager_svnt_Export NodeManager_Impl_Base
    : public virtual POA_CIAO::NodeManagerDaemon
  {
  public:
    /// A struct that tracks the reference count of a particular
    /// component as well as the "child_plan_uuid" where the component is
    /// actually installed
    typedef struct _ref_count_info
    {
      ACE_CString plan_uuid_;
      size_t count_;
    } Ref_Count_Info;

    /// Constructor
    NodeManager_Impl_Base (const char *name,
                      CORBA::ORB_ptr orb,
                      PortableServer::POA_ptr p,
                      const char * nodeapp_loc,
                      const char* nodeapp_options,
                      int spawn_delay);

    /// Initializes the NodeManager.
    virtual void init ();

    /// POA operation..
    virtual PortableServer::POA_ptr _default_POA (void);

    /// CIAO::Daemon defined attributes/operations.
    virtual char * name ();

    virtual void shutdown ();

    virtual void joinDomain (const Deployment::Domain & domain,
                             Deployment::TargetManager_ptr manager,
                             Deployment::Logger_ptr log
                             );

    virtual void leaveDomain ();

    virtual ::Deployment::NodeApplicationManager_ptr
    preparePlan (const Deployment::DeploymentPlan &plan);

    virtual void
    destroyManager (Deployment::NodeApplicationManager_ptr appManager);

    /// CIAO specific extension, which is particularly useful for dealing with
    /// shared components issue
    virtual void destroyPlan (const Deployment::DeploymentPlan & plan);

    /// CIAO specific extension, return a set of shared components
    virtual ::Deployment::ComponentPlans *
    get_shared_components ();

    /// RACE specific extension. Modify the priority of a node application
    /// process.

    virtual ::CORBA::Long
    set_priority (
        const char * plan_id,
        const char * cid,
        const ::Deployment::Sched_Params & nm_params);

    // ********* CIAO Specific Helper functions ************

    virtual ::Components::FacetDescriptions *
    get_all_facets (ACE_CString & name);

    virtual ::Components::ConsumerDescriptions *
    get_all_consumers (ACE_CString & name);

    virtual void
    set_all_facets (ACE_CString &name,
                   const ::Components::FacetDescriptions_var & facets);

    virtual void
    set_all_consumers (ACE_CString &name,
                       const ::Components::ConsumerDescriptions_var & consumers);

    // ********* Function added for getting component ids...

    struct Component_Ids
    {
      ACE_Unbounded_Set <ACE_CString> cid_seq_;
      pid_t process_id_;
    };

    virtual void push_component_id_info (Component_Ids comps);


    Component_Ids get_component_detail ();

  private:
    /// Validate the child deployment plan. In particular, we are
    /// trying to verify that all the component instances within this
    /// child deployment plan have the same "server_resource_id"
    /// assigned.
    bool validate_plan (const Deployment::DeploymentPlan &plan);

  protected:
    /// Since this class is reference counted, making this
    /// destructor protected to enforce proper memory managment
    /// through the reference counting mechanism (i.e. to
    /// disallow calling operator delete() on an instance of
    /// this class.
    virtual ~NodeManager_Impl_Base (void);

    virtual ::CIAO::NodeApplicationManager_Impl_Base *
    create_node_app_manager (CORBA::ORB_ptr orb,
                             PortableServer::POA_ptr poa)
      =0;

    /// Actual implementation of the @c get_shared_components IDL operation
    virtual ::Deployment::ComponentPlans *
    get_shared_components_i (void);

    /// A helper method that convert ACE_Unbounded_Set to CORBA StringSeq
    CORBA::StringSeq * shared_components_seq (void);

    /// Keep a pointer to the managing ORB serving this servant.
    CORBA::ORB_var orb_;

    /// Keep a pointer to the managing POA.
    PortableServer::POA_var poa_;

    /// My Canonical name.
    CORBA::String_var name_;

    /// NodeApplication location
    CORBA::String_var nodeapp_location_;

    /// NodeApplication options
    CORBA::String_var nodeapp_options_;

    /// Cached callback POA.
    PortableServer::POA_var callback_poa_;

    /// Spawn delay for the NodeAppMgr
    int spawn_delay_;

    /// Cache reference of last NodeAppManager
    /// Deployment::NodeApplicationManager_var manager_;
    NAM_Map map_;

    /// A reference count map for the components installed on this node
    /// @@TODO We should also keep the information about *where* the
    /// component is installed
    typedef
    ACE_Hash_Map_Manager_Ex <ACE_CString,
                             Ref_Count_Info,
                             ACE_Hash<ACE_CString>,
                             ACE_Equal_To<ACE_CString>,
                             ACE_Null_Mutex> Reference_Count_Map;
    typedef Reference_Count_Map::iterator Ref_Count_Iterator;

    Reference_Count_Map ref_count_map_;

    /// A set to track the names of shared component instances
    ACE_Unbounded_Set<ACE_CString> shared_components_;

    /// Cached object references of ports (facets/consumers) of
    /// all components. This is useful for getting the port object
    /// references of "shared components". The key in the map
    /// is the component instance name.
    typedef
    ACE_Hash_Map_Manager_Ex <ACE_CString,
                             Components::FacetDescriptions_var,
                             ACE_Hash<ACE_CString>,
                             ACE_Equal_To<ACE_CString>,
                             ACE_Null_Mutex> Component_Facets_Map;

    Component_Facets_Map comp_facets_map_;

    typedef
    ACE_Hash_Map_Manager_Ex <ACE_CString,
                             Components::ConsumerDescriptions_var,
                             ACE_Hash<ACE_CString>,
                             ACE_Equal_To<ACE_CString>,
                             ACE_Null_Mutex> Component_Consumers_Map;

    Component_Consumers_Map comp_consumers_map_;

    /// The MonitorController pointer
    auto_ptr <MonitorController> monitor_controller_;

    /// The set of Components
    Component_Ids components_;
  };


  /**
   * @class NodeManager_Impl
   *
   */
  class NodeManager_svnt_Export NodeManager_Impl
    : public virtual NodeManager_Impl_Base
  {
  public:

    /// Constructor
    NodeManager_Impl (const char *name,
                     CORBA::ORB_ptr orb,
                     PortableServer::POA_ptr p,
                     const char * nodeapp_loc,
                     const char * nodeapp_options,
                     int spawn_delay,
                     bool is_multi_threaded);

  protected:
    /// Since this class is reference counted, making this
    /// destructor protected to enforce proper memory managment
    /// through the reference counting mechanism (i.e. to
    /// disallow calling operator delete() on an instance of
    /// this class.
    virtual ~NodeManager_Impl (void);

    virtual ::CIAO::NodeApplicationManager_Impl_Base *
    create_node_app_manager (CORBA::ORB_ptr orb,
                             PortableServer::POA_ptr poa);

  private:
    bool is_multi_threaded_;
  };

  class NodeApplicationManager_Impl;
  struct Static_Config_EntryPoints_Maps;

  /**
   * @class Static_NodeManager_Impl
   *
   */
  class NodeManager_svnt_Export Static_NodeManager_Impl
    : public virtual NodeManager_Impl_Base
  {
  public:

    /// Constructor
    Static_NodeManager_Impl (const char *name,
                            CORBA::ORB_ptr orb,
                            PortableServer::POA_ptr p,
                            const char * nodeapp_loc,
                            const char * nodeapp_options,
                            int spawn_delay,
                            Static_Config_EntryPoints_Maps* static_config_entrypoints_maps
                            );

    void destroyManager
    (Deployment::NodeApplicationManager_ptr manager);

  protected:
    /// Since this class is reference counted, making this
    /// destructor protected to enforce proper memory managment
    /// through the reference counting mechanism (i.e. to
    /// disallow calling operator delete() on an instance of
    /// this class.
    virtual ~Static_NodeManager_Impl (void);

    virtual ::CIAO::NodeApplicationManager_Impl_Base *
    create_node_app_manager (CORBA::ORB_ptr orb,
                             PortableServer::POA_ptr poa);

    Static_Config_EntryPoints_Maps* static_config_entrypoints_maps_;
  };

}

#if defined(_MSC_VER)
#pragma warning(pop)
#endif /* _MSC_VER */

#include /**/ "ace/post.h"
#endif /* CIAO_NODEMANAGER_IMPL_H */
