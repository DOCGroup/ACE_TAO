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
#include "Deployment/Deployment_ExecutionManagerS.h"
#include "Deployment/Deployment_NodeManagerC.h"
#include "DomainApplicationManager/DomainApplicationManager_Impl.h"
#include "DomainApplicationManager/Node_Locator.h"
#include "Completion/CompletionHandler_T.h"
#include "Completion/Completion_Counter_Base.h"

namespace DAnCE
  {
  class ExecutionManager_Impl;

  typedef PortableServer::Servant_var<ExecutionManager_Impl> ExecutionManager_Impl_var;

  class ExecutionManager_Impl
        : public virtual POA_Deployment::AMH_ExecutionManager
    {
    public:
      ExecutionManager_Impl (CORBA::ORB_ptr orb,
                             PortableServer::POA_ptr poa,
                             CosNaming::NamingContext_ptr);

      virtual ~ExecutionManager_Impl(void);

      virtual void preparePlan (
        ::Deployment::AMH_ExecutionManagerResponseHandler_ptr _tao_rh,
        const ::Deployment::DeploymentPlan & plan,
        ::Deployment::ResourceCommitmentManager_ptr resourceCommitment);

      virtual void getManagers (
        ::Deployment::AMH_ExecutionManagerResponseHandler_ptr _tao_rh);

      virtual void destroyManager (
        ::Deployment::AMH_ExecutionManagerResponseHandler_ptr _tao_rh,
        ::Deployment::DomainApplicationManager_ptr manager);

      void add_node_manager (const char *name, const char *ior);

      void load_node_map (const ACE_TCHAR *filename);

      class PreparePlanCompletionHandler
            : public CompletionHandler<DomainApplicationManager_Impl>
        {
        public:
          PreparePlanCompletionHandler (
            ExecutionManager_Impl *em_servant,
            ::Deployment::AMH_ExecutionManagerResponseHandler_ptr _tao_rh);

          virtual void handle_completion (DomainApplicationManager_Impl *dam_servant);
          virtual void handle_exception (CORBA::Exception* local_ex);

        private:
          ExecutionManager_Impl_var em_servant_;
          ::Deployment::AMH_ExecutionManagerResponseHandler_var em_rh_;
        };

      class DestroyManagerCompletionHandler
            : public CompletionHandler<DomainApplicationManager_Impl>
        {
        public:
          DestroyManagerCompletionHandler (
            ExecutionManager_Impl *em_servant,
            ::Deployment::AMH_ExecutionManagerResponseHandler_ptr _tao_rh,
            DomainApplicationManager_Impl* dam_servant);

          virtual void handle_completion (DomainApplicationManager_Impl *dam_servant);
          virtual void handle_exception (CORBA::Exception* local_ex);

        private:
          ExecutionManager_Impl_var em_servant_;
          ::Deployment::AMH_ExecutionManagerResponseHandler_var em_rh_;
          DomainApplicationManager_Impl_var dam_servant_;
        };

    private:
      void finish_preparePlan (
        ::Deployment::AMH_ExecutionManagerResponseHandler_ptr _tao_rh,
        DomainApplicationManager_Impl *dam_servant);
      void fail_preparePlan (
        ::Deployment::AMH_ExecutionManagerResponseHandler_ptr _tao_rh,
        CORBA::Exception* local_ex);

      friend class PreparePlanCompletionHandler;

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
