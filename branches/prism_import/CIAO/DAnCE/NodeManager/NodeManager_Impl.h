// -*- C++ -*-

//=============================================================================
/**
 *  @file    NodeManager_Impl.h
 *
 *  $Id$
 *
 * @Brief  Implementation of POA_Deployment::NodeManager
 *
 * @author Erwin Gottlieb <eg@prismtech.com>
 */
//=============================================================================

#ifndef NODEMANAGER_IMPL_H_
#define NODEMANAGER_IMPL_H_

#include "NodeManager_Export.h"
#include <Deployment/Deployment_NodeManagerS.h>
#include <NodeApplicationManager/NodeApplicationManager_Impl.h>
#include "RedirectionService/RedirectionService.h"

namespace DAnCE
  {
  class NodeManager_Export NodeManager_Impl : public virtual POA_Deployment::NodeManager
    {
    public:
      // the fixed listener port is caused by the usage of CCM Object locator
      NodeManager_Impl (CORBA::ORB_ptr orb
                        , PortableServer::POA_ptr poa
                        , const char* name
                        , RedirectionService& redirection);

      virtual ~NodeManager_Impl();

      virtual void joinDomain (
        const ::Deployment::Domain & theDomain,
        ::Deployment::TargetManager_ptr manager,
        ::Deployment::Logger_ptr log,
        ::CORBA::Long updateInterval
        ACE_ENV_ARG_DECL_WITH_DEFAULTS
      )
      ACE_THROW_SPEC ( (
                         ::CORBA::SystemException
                       ));

      virtual void leaveDomain (
        ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS
      )
      ACE_THROW_SPEC ( (
                         ::CORBA::SystemException
                       ));

      virtual ::Deployment::NodeApplicationManager_ptr preparePlan (
        const ::Deployment::DeploymentPlan & plan,
        ::Deployment::ResourceCommitmentManager_ptr resourceCommitment
        ACE_ENV_ARG_DECL_WITH_DEFAULTS
      )
      ACE_THROW_SPEC ( (
                         ::CORBA::SystemException,
                         ::Deployment::StartError,
                         ::Deployment::PlanError
                       ));

      virtual void destroyManager (
        ::Deployment::NodeApplicationManager_ptr appManager
        ACE_ENV_ARG_DECL_WITH_DEFAULTS
      )
      ACE_THROW_SPEC ( (
                         ::CORBA::SystemException,
                         ::Deployment::StopError,
                         ::Deployment::InvalidReference
                       ));

      virtual ::Deployment::Resources * getDynamicResources (
        ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS
      )
      ACE_THROW_SPEC ( (
                         ::CORBA::SystemException
                       ));

      const char* getName();

    private:
      CORBA::ORB_var orb_;
      PortableServer::POA_var poa_;
      ACE_CString name_;
      typedef ACE_Map_Manager<ACE_CString, NodeApplicationManager_Impl*, ACE_Null_Mutex > TManagers;
      TManagers managers_;
      RedirectionService& redirection_;
    };
};

#endif /*NODEMANAGER_IMPL_H_*/
