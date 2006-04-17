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
#include "ciao/Deployment_common.h"

namespace CIAO
{
  namespace Execution_Manager
  {
    /**
     *
     * @class Execution_Manager_Impl
     *
     * @brief This class implements the
     * ExecutionManger. ExecutionManager starts the execution process
     * after the planning stage.
     *
     */
    class Execution_Manager_Impl
      : public virtual POA_CIAO::ExecutionManagerDaemon
    {
    public:
      /// Constructor
      Execution_Manager_Impl (CORBA::ORB_ptr orb,
                              PortableServer::POA_ptr poa,
                              const char * init_file);

      /// Template methods from ExecutionManagerDaemon, please see
      /// $CIAO_ROOT/ciao/Deployment.idl for documentation
      virtual Deployment::DomainApplicationManager_ptr
      preparePlan (const Deployment::DeploymentPlan & plan,
                   CORBA::Boolean commitResources
                   ACE_ENV_ARG_DECL_WITH_DEFAULTS)
        ACE_THROW_SPEC ((CORBA::SystemException,
                         Deployment::ResourceNotAvailable,
                         Deployment::PlanError,
                         Deployment::StartError));

      virtual Deployment::DomainApplicationManagers *
      getManagers (ACE_ENV_SINGLE_ARG_DECL)
        ACE_THROW_SPEC ((CORBA::SystemException));

      // Below method is CIAO specific extension
      virtual Deployment::DomainApplicationManager_ptr
      getManager (const char * plan_uuid
                  ACE_ENV_ARG_DECL_WITH_DEFAULTS)
        ACE_THROW_SPEC ((CORBA::SystemException, Deployment::PlanNotExist));

      virtual void
      destroyManager (Deployment::DomainApplicationManager_ptr manager
                      ACE_ENV_ARG_DECL_WITH_DEFAULTS)
        ACE_THROW_SPEC ((CORBA::SystemException,
                         Deployment::StopError));

      // Below method is CIAO specific extension, please see the IDL
      // definition for more details.
      virtual void
      destroyManagerByPlan (const char * plan_uuid
                            ACE_ENV_ARG_DECL_WITH_DEFAULTS)
        ACE_THROW_SPEC ((::CORBA::SystemException,
                         ::Deployment::StopError));

      virtual void shutdown (ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)
        ACE_THROW_SPEC ((CORBA::SystemException));

      // The input parameter is a *new_plan* which has the
      // same UUID of the existing running plan.
      virtual void
      perform_redeployment (
          const Deployment::DeploymentPlan & plan
          ACE_ENV_ARG_DECL_WITH_DEFAULTS)
        ACE_THROW_SPEC ((::CORBA::SystemException,
                         ::Deployment::PlanError,
                         ::Deployment::InstallationFailure,
                         ::Deployment::UnknownImplId,
                         ::Deployment::ImplEntryPointNotFound,
                         ::Deployment::InvalidConnection,
                         ::Deployment::InvalidProperty,
                         ::Components::RemoveFailure));

      virtual Deployment::DeploymentPlan * getPlan (
          const char * plan_uuid
          ACE_ENV_ARG_DECL_WITH_DEFAULTS)
        ACE_THROW_SPEC ((::CORBA::SystemException));

    /// ****************** C++ Methods *************************

    /// If input <add_connection> is true, then it will add new
    /// connections which are across different assemblies. Otherwise
    /// it will remove the specified connections of this component.
    ///
    /// @@GD: Later we can add another method which could accept
    /// a list of bindings and do the batch job.
    virtual void finalize_global_binding (
        const Component_Binding_Info & binding,
        CORBA::Boolean add_connection)
      ACE_THROW_SPEC ((
        ::CORBA::SystemException,
        ::Deployment::InvalidConnection));

    virtual void passivate_shared_components (
        const Component_Binding_Info & binding)
      ACE_THROW_SPEC ((CORBA::SystemException,
                      Deployment::StartError));

    virtual void activate_shared_components (
        const Component_Binding_Info & binding)
      ACE_THROW_SPEC ((CORBA::SystemException,
                      Deployment::StartError));

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
        find_node_application (const Component_Binding_Info & binding)
      ACE_THROW_SPEC ((
        ::CORBA::SystemException,
        ::Deployment::InvalidConnection));

    protected:
      /// Destructor.
      virtual ~Execution_Manager_Impl (void);

      /// Cached ORB pointer
      CORBA::ORB_var orb_;

      /// Cached POA pointer
      PortableServer::POA_var poa_;

      /// Path to the initialization file
      const ACE_CString init_file_;

      /// A map which caches the DomainApplicationManager object ref.
      DAM_Map map_;

      /// A set of shared components and their location info.
      ACE_Unbounded_Set<Component_Binding_Info> shared_components_;
    };
  }
}

#include /**/ "ace/post.h"
#endif /* EXECUTIONMANAGER_IMPL_H */
