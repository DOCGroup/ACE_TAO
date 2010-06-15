// -*- C++ -*-

//=============================================================================
/**
 *  @file    NodeApplicationManager_Impl.h
 *
 *  $Id$
 *
 * @brief  Implementation of POA_Deployment::NodeApplicationManager
 *
 * @author Erwin Gottlieb <eg@prismtech.com>
 */
//=============================================================================

#ifndef NODEAPPLICATIONMANAGER_IMPL_H_
#define NODEAPPLICATIONMANAGER_IMPL_H_

#include "NodeApplicationManager_Export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "orbsvcs/orbsvcs/CosNamingC.h"

#include "Deployment/Deployment_DeploymentPlanC.h"
#include "Deployment/Deployment_NodeApplicationManagerS.h"
#include "NodeApplication/NodeApplication_Impl.h"
#include "DAnCE/DAnCE_ArtifactInstallationC.h"

namespace DAnCE
  {

  class NodeApplicationManager_Export NodeApplicationManager_Impl :
    public virtual POA_Deployment::NodeApplicationManager
    {
    public:

      NodeApplicationManager_Impl (CORBA::ORB_ptr orb,
                                   PortableServer::POA_ptr poa,
                                   DAnCE::ArtifactInstallation_ptr installer,
                                   const ACE_CString& name,
                                   const PROPERTY_MAP &properties);

      virtual ~NodeApplicationManager_Impl();

      virtual Deployment::Application_ptr
      startLaunch (const Deployment::Properties & configProperty,
                   Deployment::Connections_out providedReference);

      virtual void destroyApplication (Deployment::Application_ptr);

      void preparePlan (const Deployment::DeploymentPlan& plan);

    private:
      LocalitySplitter split_plan_;
      CORBA::ORB_var orb_;
      PortableServer::POA_var poa_;
      DAnCE::ArtifactInstallation_var installer_;
      NodeApplication_Impl* application_;
      ACE_CString node_name_;
      PROPERTY_MAP properties_;
      CosNaming::NamingContext_var instance_nc_;
    };

  typedef PortableServer::Servant_var<NodeApplicationManager_Impl> Safe_NodeApplicationManager_Impl;

}
#endif /*NODEAPPLICATIONMANAGER_IMPL_H_*/
