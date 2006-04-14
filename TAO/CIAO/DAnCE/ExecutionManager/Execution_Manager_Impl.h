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

#include "ExecutionManagerS.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "DAM_Map.h"
#include "ace/SString.h"

#if defined(_MSC_VER)
#pragma warning(push)
#pragma warning(disable:4250)
#endif /* _MSC_VER */

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
      Execution_Manager_Impl (CORBA::ORB_ptr orb,
                              PortableServer::POA_ptr poa,
                              const char * init_file);

      /// Template methods from ExecutionManagerDaemon, please see
      /// $CIAO_ROOT/ciao/Deployment.idl for documentation
      virtual Deployment::DomainApplicationManager_ptr
      preparePlan (const Deployment::DeploymentPlan & plan,
                   CORBA::Boolean commitResources
                   ACE_ENV_ARG_DECL)
        ACE_THROW_SPEC ((CORBA::SystemException,
                         Deployment::ResourceNotAvailable,
                         Deployment::PlanError,
                         Deployment::StartError));

      virtual Deployment::DomainApplicationManagers *
      getManagers (ACE_ENV_SINGLE_ARG_DECL)
        ACE_THROW_SPEC ((CORBA::SystemException));

      virtual void
      destroyManager (Deployment::DomainApplicationManager_ptr manager
                      ACE_ENV_ARG_DECL)
        ACE_THROW_SPEC ((CORBA::SystemException,
                         Deployment::StopError));

      virtual void shutdown (ACE_ENV_SINGLE_ARG_DECL)
        ACE_THROW_SPEC ((CORBA::SystemException));

    protected:
      /// Destructor.
      ~Execution_Manager_Impl (void);

      /// Cached ORB pointer
      CORBA::ORB_var orb_;

      /// Cached POA pointer
      PortableServer::POA_var poa_;

      // Path to the initialization file
      const ACE_CString init_file_;

      DAM_Map map_;
    };
  }
}

#if defined(_MSC_VER)
#pragma warning(pop)
#endif /* _MSC_VER */

#include /**/ "ace/post.h"
#endif /* EXECUTIONMANAGER_IMPL_H */
