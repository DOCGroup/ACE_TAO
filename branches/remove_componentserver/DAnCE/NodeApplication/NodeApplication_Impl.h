// -*- C++ -*-

//=============================================================================
/**
 *  @file    NodeApplication_Impl.h
 *
 *  $Id$
 *
 * @brief  Implementation of Deployment::NodeApplication
 *
 * @author Erwin Gottlieb <eg@prismtech.com>
 * @author William R. Otte <wotte@dre.vanderbilt.edu>
 */
//=============================================================================


#ifndef NODEAPPLICATION_IMPL_H_
#define NODEAPPLICATION_IMPL_H_

#include "NodeApplication_Export.h"

#include "ace/Containers_T.h"
#include "tao/AnyTypeCode/AnyTypeCode_methods.h"
#include "ccm/CCM_KeylessCCMHomeC.h"
#include "ciao/ComponentServer/CIAO_ServerActivator_Impl.h"

#include "ccm/ComponentServer/CCM_ComponentServerC.h"
#include "Deployment/Deployment_NodeApplicationS.h"
#include "Deployment/Deployment_DeploymentPlanC.h"
#include "Deployment/DeploymentC.h"
#include "DAnCE/DAnCE_Utility.h"
#include "DAnCE/DAnCE_LocalityManagerC.h"
#include "LocalityManager/Scheduler/Deployment_Scheduler.h"
#include "DAnCE/DAnCE_ArtifactInstallationC.h"
#include "Split_Plan/Locality_Splitter.h"
#include "Split_Plan/Split_Plan.h"

#include <map>
#include <vector>

using namespace DAnCE::Utility;

namespace DAnCE
{

  typedef DAnCE::Split_Plan < DAnCE::Locality_Splitter > LocalitySplitter;

  class NodeManager_Impl;
  
  class NodeApplication_Export NodeApplication_Impl :
    public virtual POA_Deployment::NodeApplication
  {
  public:
    NodeApplication_Impl (CORBA::ORB_ptr orb,
                          PortableServer::POA_ptr poa,
                          DAnCE::ArtifactInstallation_ptr installer,
                          const ACE_CString& node_name,
                          const PROPERTY_MAP &properties);

    virtual ~NodeApplication_Impl();

    virtual void finishLaunch (const ::Deployment::Connections & providedReference,
                               ::CORBA::Boolean start);

    virtual void start (void);

    void prepare_instances (const LocalitySplitter::TSubPlans &plans);

    void start_launch_instances (const Deployment::Properties &prop,
                                 Deployment::Connections_out providedReference);

    void remove_instances (void);

    typedef std::map <std::string, ::DAnCE::LocalityManager_var>
      LOCALITY_MAP;
    
    
    typedef std::pair <CORBA::ULong, ::Deployment::DeploymentPlan> SUB_PLAN;
    typedef std::map <std::string, SUB_PLAN> PLAN_MAP;

  protected:
    void prepare_instance (const char *name,
                           const ::Deployment::DeploymentPlan &plan);

    CORBA::ORB_var orb_;

    PortableServer::POA_var poa_;

    DAnCE::ArtifactInstallation_var installer_;

    ACE_CString node_name_;

    LOCALITY_MAP localities_;
    
    DAnCE::Deployment_Scheduler scheduler_;

    PLAN_MAP sub_plans_;
    
    CORBA::ULong spawn_delay_;
  };
}
#endif /*NODEAPPLICATION_IMPL_H_*/
