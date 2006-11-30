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
      preparePlan (const ::Deployment::DeploymentPlan & plan,
                   ::Deployment::ResourceCommitmentManager_ptr resourceCommitment)
        throw (CORBA::SystemException,
               Deployment::ResourceNotAvailable,
               Deployment::PlanError,
               Deployment::StartError);

      virtual Deployment::DomainApplicationManagers *
      getManagers ()
        throw (CORBA::SystemException);

      virtual void
      destroyManager (Deployment::DomainApplicationManager_ptr manager)
        throw (CORBA::SystemException,
               Deployment::StopError);
      
      // Below method is CIAO specific extension
      virtual Deployment::DomainApplicationManager_ptr
      getManager (const char * plan_uuid)
        throw (CORBA::SystemException, 
	       Deployment::PlanNotExist);

      virtual void shutdown (void)
        throw (CORBA::SystemException);

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
    };
  }
}

#include /**/ "ace/post.h"
#endif /* EXECUTIONMANAGER_IMPL_H */
