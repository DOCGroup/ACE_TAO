// -*- C++ -*-

//=============================================================================
/**
 *  @file    NodeManager_Impl.h
 *
 *  $Id$
 *
 * @brief  Implementation of POA_Deployment::NodeManager
 *
 * @author Erwin Gottlieb <eg@prismtech.com>
 * @author William R. Otte <wotte@dre.vanderbilt.edu>
 */
//=============================================================================

#ifndef NODEMANAGER_IMPL_H_
#define NODEMANAGER_IMPL_H_

#include "tao/Object.h"
#include "tao/ORB.h"
#include "dance/DAnCE_Utility.h"
#include "dance/Deployment/Deployment_NodeManagerS.h"
#include "dance/NodeApplicationManager/NodeApplicationManager_Impl.h"
#include "dance/DAnCE_ArtifactInstallationC.h"
#include "ace/Map_Manager.h"

namespace DAnCE
{
  class NodeManager_Impl :
    public virtual POA_Deployment::NodeManager
  {
  public:
    /// The fixed listener port is caused by the usage of CCM Object locator
    NodeManager_Impl (CORBA::ORB_ptr orb,
                      PortableServer::POA_ptr poa,
                      DAnCE::ArtifactInstallation_ptr installer,
                      const char* name,
                      std::string config_file,
                      const PROPERTY_MAP &options);

    virtual ~NodeManager_Impl();

    virtual void joinDomain (const ::Deployment::Domain & theDomain,
                             ::Deployment::TargetManager_ptr manager,
                             ::Deployment::Logger_ptr log,
                             ::CORBA::Long updateInterval);

    virtual void leaveDomain (void);

    virtual ::Deployment::NodeApplicationManager_ptr preparePlan (
            const ::Deployment::DeploymentPlan & plan,
            ::Deployment::ResourceCommitmentManager_ptr resourceCommitment);

    virtual void destroyManager (::Deployment::NodeApplicationManager_ptr appManager);

    virtual ::Deployment::Resources * getDynamicResources (void);

  private:
    CORBA::ORB_var orb_;
    PortableServer::POA_var poa_;
    DAnCE::ArtifactInstallation_var installer_;
    ACE_CString name_;
    /// @todo, see if we can use a _ptr
    typedef ACE_Map_Manager<ACE_CString, NodeApplicationManager_Impl*, ACE_Null_Mutex > TManagers;
    TManagers managers_;
    PROPERTY_MAP properties_;
    DAnCE::Deployment_Scheduler scheduler_;
  };
}

#endif /*NODEMANAGER_IMPL_H_*/
