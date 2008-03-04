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
#include "ExecutionManager_Export.h"
#include "Interfaces/ExecutionManagerDaemonS.h"
#include "Deployment/Deployment_NodeManagerC.h"
#include "DomainApplicationManager/DomainApplicationManager_Impl.h"

namespace DAnCE
  {
  class ExecutionManager_Export ExecutionManager_Impl
        : public virtual POA_DAnCE::ExecutionManagerDaemon
    {
    private:
      typedef ACE_Map_Manager<ACE_CString, DomainApplicationManager_Impl*, ACE_Null_Mutex> TDomainManagers;

    public:

      ExecutionManager_Impl (CORBA::ORB_ptr orb,
                             PortableServer::POA_ptr poa);

      virtual ~ExecutionManager_Impl();

      virtual ::Deployment::DomainApplicationManager_ptr preparePlan (
        const ::Deployment::DeploymentPlan & plan,
        ::Deployment::ResourceCommitmentManager_ptr resourceCommitment
        ACE_ENV_ARG_DECL_WITH_DEFAULTS
      )
      ACE_THROW_SPEC ( (
                         ::CORBA::SystemException,
                         ::Deployment::ResourceNotAvailable,
                         ::Deployment::PlanError,
                         ::Deployment::StartError
                       ));

      virtual ::Deployment::DomainApplicationManagers * getManagers (
        ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS
      )
      ACE_THROW_SPEC ( (
                         ::CORBA::SystemException
                       ));

      virtual void destroyManager (
        ::Deployment::DomainApplicationManager_ptr manager
        ACE_ENV_ARG_DECL_WITH_DEFAULTS
      )
      ACE_THROW_SPEC ( (
                         ::CORBA::SystemException,
                         ::Deployment::StopError
                       ));

      // This one derived from ExecutionManagerDaemon interface
      // for shutdowning DAnCE agent
      virtual void shutdown (
        ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS
      )
      ACE_THROW_SPEC ( (
                         ::CORBA::SystemException
                       ))
      {
        this->orb_->shutdown();
      };

      void add_node_manager (const ACE_CString & name, Deployment::NodeManager_ptr obj)
      {
        this->nodes_.rebind (name, Deployment::NodeManager::_duplicate (obj));
      };

    private:
      CORBA::ORB_var orb_;
      PortableServer::POA_var poa_;
      DomainApplicationManager_Impl::TNodeManagers nodes_;
      TDomainManagers managers_;

    };
};//DAnCE

#endif /*EXECUTIONMANAGER_IMPL_H_*/
