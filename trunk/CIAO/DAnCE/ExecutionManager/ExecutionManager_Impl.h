// -*- C++ -*-

//=============================================================================
/**
 *  @file    ExecutionManager_Impl.h
 *
 *  $Id$
 *
 * @Brief  Implementation of Deployment::ExecutionManager and POA_DAnCE::ExecutionManagerDaemon
 *
 * @author Erwin Gottlieb <eg@prismtech.com>
 */
//=============================================================================

#ifndef EXECUTIONMANAGER_IMPL_H_
#define EXECUTIONMANAGER_IMPL_H_

#include "ace/Map_Manager.h"
#include "ace/SStringfwd.h"
#include "orbsvcs/orbsvcs/CosNamingC.h"
#include "ExecutionManager_Export.h"
#include "Interfaces/ExecutionManagerDaemonS.h"
#include "Deployment/Deployment_NodeManagerC.h"
#include "DomainApplicationManager/DomainApplicationManager_Impl.h"
#include "DomainApplicationManager/Node_Locator.h"

namespace DAnCE
  {
  class ExecutionManager_Export ExecutionManager_Impl
        : public virtual POA_DAnCE::ExecutionManagerDaemon
    {
    private:
      typedef ACE_Map_Manager<ACE_CString, DomainApplicationManager_Impl*, ACE_Null_Mutex> TDomainManagers;

    public:

      ExecutionManager_Impl (CORBA::ORB_ptr orb,
                             PortableServer::POA_ptr poa,
                             CosNaming::NamingContext_ptr);

      virtual ~ExecutionManager_Impl();

      virtual ::Deployment::DomainApplicationManager_ptr preparePlan (
        const ::Deployment::DeploymentPlan & plan,
        ::Deployment::ResourceCommitmentManager_ptr resourceCommitment
      );

      virtual ::Deployment::DomainApplicationManagers * getManagers (
      );

      virtual void destroyManager (
        ::Deployment::DomainApplicationManager_ptr manager
      );

      // This one derived from ExecutionManagerDaemon interface
      // for shutdowning DAnCE agent
      virtual void shutdown ();
      
      void add_node_manager (const ACE_TCHAR *name, const ACE_TCHAR *ior);

      void load_node_map (const ACE_TCHAR *filename);
      
    private:
      CORBA::ORB_var orb_;
      PortableServer::POA_var poa_;
      TDomainManagers managers_;
      Node_Locator locator_;
    };
}//DAnCE

#endif /*EXECUTIONMANAGER_IMPL_H_*/
