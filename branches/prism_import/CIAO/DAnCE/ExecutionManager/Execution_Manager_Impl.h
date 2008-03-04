/*=======================================================================
 *
 * @file Execution_Manager_Impl.h
 *
 * $Id$
 *
 * @brief This file contains implementation for
 * Deployment::ExecutionManager interface.
 *
 * @author Arvind S. Krishna <arvindk@dre.vanderbilt.edu>
 * @auther Tao Lu <lu@dre.vanderbilt.edu>
 *
 *======================================================================*/

#ifndef CIAO_EXECUTION_MANAGER_IMPL_H
#define CIAO_EXECUTION_MANAGER_IMPL_H
#include /**/ "ace/pre.h"

#include "Interfaces/ExecutionManagerDaemonS.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "DAM_Map.h"
#include "ace/SString.h"
#include "DAnCE/Deployment/Deployment_common.h"

namespace CIAO
{
  namespace Execution_Manager
  {
    /**
     *
     * @class Execution_Manager_Impl
     *
     * @brief This class implements the ExecutionManger. ExecutionManager
     * starts the execution process after the planning stage.
     *
     */
    class Execution_Manager_Impl
      : public virtual POA_CIAO::ExecutionManagerDaemon
    {
    public:
      /// Constructor
      Execution_Manager_Impl (CORBA::ORB_ptr orb,
                              PortableServer::POA_ptr poa,
                              const char * init_file,
                              bool is_using_ami,
                              bool is_using_active_object);

      /// Template methods from ExecutionManagerDaemon, please see
      /// $CIAO_ROOT/DAnCE/Deployment/Deployment.idl for documentation
      virtual Deployment::DomainApplicationManager_ptr
      preparePlan (const Deployment::DeploymentPlan & plan,
                   CORBA::Boolean commitResources);

      virtual Deployment::DomainApplicationManagers *getManagers ();

      // Below method is CIAO specific extension
      virtual Deployment::DomainApplicationManager_ptr
      getManager (const char * plan_uuid);

      virtual void
      destroyManager (Deployment::DomainApplicationManager_ptr manager);

      // Below method is CIAO specific extension, please see the IDL
      // definition for more details.
      virtual void destroyManagerByPlan (const char * plan_uuid);

      virtual void shutdown ();

      // The input parameter is a *new_plan* which has the
      // same UUID of the existing running plan.
      virtual void
      perform_redeployment (const Deployment::DeploymentPlan & plan);

      virtual Deployment::DeploymentPlan * getPlan (const char * plan_uuid);

    /// ****************** C++ Methods *************************

    /// If input <add_connection> is true, then it will add new
    /// connections which are across different assemblies. Otherwise
    /// it will remove the specified connections of this component.
    ///
    /// @@GD: Later we can add another method which could accept
    /// a list of bindings and do the batch job.
    virtual void finalize_global_binding (
        const Component_Binding_Info & binding,
        CORBA::Boolean add_connection);

    virtual void passivate_shared_components (
        const Component_Binding_Info & binding);

    virtual void activate_shared_components (
        const Component_Binding_Info & binding);

      /// Add shared component information.
      /// This call will be made by DomainApplicationManager.
      virtual void
        add_shared_component (const Component_Binding_Info & binding);

      /// Remove shared component
      virtual void
        remove_shared_component (const Component_Binding_Info & binding);

      /// If the input component <name> was found in the internal
      /// cached shared component list, and the plan_uuid also matches,
      /// then this member function returns <true>, otherwise it
      /// returns <false>.
      virtual bool
        is_component_running (const char * name, const char * plan_uuid);

    protected:
      /// Return the NodeApplication hosting the given biding
      virtual Deployment::NodeApplication_ptr
        find_node_application (const Component_Binding_Info & binding);

      CORBA::Short get_current_thread_priority (void);

    protected:
      /// Destructor.
      virtual ~Execution_Manager_Impl (void);

      /// Cached ORB pointer
      CORBA::ORB_var orb_;

      /// Cached POA pointer
      PortableServer::POA_var poa_;

      /// Path to the initialization file
      ACE_CString const init_file_;

      /// Whether we use AMI
      bool is_using_ami_;

      /// Whether we use active object based deployment
      bool is_using_active_object_;

      /// A map which caches the DomainApplicationManager object ref.
      DAM_Map map_;

      /// A set of shared components and their location info.
      ACE_Unbounded_Set<Component_Binding_Info> shared_components_;
    };
  }
}

#include /**/ "ace/post.h"
#endif /* EXECUTIONMANAGER_IMPL_H */
