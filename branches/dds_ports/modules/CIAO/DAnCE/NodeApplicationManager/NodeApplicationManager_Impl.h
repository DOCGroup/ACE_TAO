// -*- C++ -*-

//=============================================================================
/**
 *  @file    NodeApplicationManager_Impl.h
 *
 *  $Id$
 *
 * @Brief  Implementation of POA_Deployment::NodeApplicationManager
 *
 * @author Erwin Gottlieb <eg@prismtech.com>
 */
//=============================================================================

#ifndef NODEAPPLICATIONMANAGER_IMPL_H_
#define NODEAPPLICATIONMANAGER_IMPL_H_

#include "NodeApplicationManager_Export.h"
#include "Deployment/Deployment_DeploymentPlanC.h"
#include "Deployment/Deployment_NodeApplicationManagerS.h"
#include "Deployment/Deployment_NodeApplicationC.h"
#include "NodeApplication/NodeApplication_Impl.h"
#include "RedirectionService/RedirectionService.h"

namespace DAnCE
  {

  class NodeManager_Impl;

  class NodeApplicationManager_Export NodeApplicationManager_Impl : public virtual POA_Deployment::NodeApplicationManager
    {
    public:
      NodeApplicationManager_Impl (CORBA::ORB_ptr orb, 
                                   PortableServer::POA_ptr poa,
                                   const Deployment::DeploymentPlan& plan, 
                                   RedirectionService & redirection, 
                                   const ACE_CString& name,
                                   const PROPERTY_MAP &properties);

      virtual ~NodeApplicationManager_Impl();

      virtual Deployment::Application_ptr
      startLaunch (const Deployment::Properties & configProperty,
                   Deployment::Connections_out providedReference);

      virtual void destroyApplication (Deployment::Application_ptr);
    private:
      void register_plan();

      Deployment::DeploymentPlan plan_;
      CORBA::ORB_var orb_;
      PortableServer::POA_var poa_;
      NodeApplication_Impl* application_;
      RedirectionService & redirection_;
      ACE_CString node_name_;
      PROPERTY_MAP properties_;
    };
}
#endif /*NODEAPPLICATIONMANAGER_IMPL_H_*/
