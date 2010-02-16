// -*- C++ -*-

//=============================================================================
/**
 *  @file    ExecutionManager_Impl.h
 *
 *  $Id$
 *
 * @brief  Implementation of Deployment::ExecutionManager and POA_DAnCE::ExecutionManagerDaemon
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
#include "Deployment/Deployment_ExecutionManagerS.h"
#include "Deployment/Deployment_NodeManagerC.h"
#include "DomainApplicationManager/DomainApplicationManager_Impl.h"
#include "DomainApplicationManager/Node_Locator.h"

namespace DAnCE
  {
  class ExecutionManager_Export ExecutionManager_Impl
        : public virtual POA_Deployment::ExecutionManager
    {
    public:
      ExecutionManager_Impl (CORBA::ORB_ptr orb,
                             PortableServer::POA_ptr poa,
                             CosNaming::NamingContext_ptr);

      virtual ~ExecutionManager_Impl(void);

      virtual ::Deployment::DomainApplicationManager_ptr preparePlan (
        const ::Deployment::DeploymentPlan & plan,
        ::Deployment::ResourceCommitmentManager_ptr resourceCommitment);

      virtual ::Deployment::DomainApplicationManagers * getManagers (void);

      virtual void destroyManager (
        ::Deployment::DomainApplicationManager_ptr manager);

      void add_node_manager (const char *name, const char *ior);

      void load_node_map (const ACE_TCHAR *filename);

    private:
      typedef ACE_Map_Manager<
        ACE_CString,
        DomainApplicationManager_Impl*,
        ACE_Null_Mutex> TDomainManagers;
      CORBA::ORB_var orb_;
      PortableServer::POA_var poa_;
      TDomainManagers managers_;
      Node_Locator locator_;
    };
}//DAnCE

#endif /*EXECUTIONMANAGER_IMPL_H_*/
