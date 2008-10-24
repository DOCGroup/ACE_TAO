// -*- C++ -*-

//=============================================================================
/**
 *  @file    DomainApplicationManager_Impl.h
 *
 *  $Id$
 *
 * @Brief  Implementation of POA_Deployment::DomainApplicationManager
 *
 * @author Erwin Gottlieb <eg@prismtech.com>
 */
//=============================================================================

#ifndef DOMAINAPPLICATIONMANAGER_IMPL_H_
#define DOMAINAPPLICATIONMANAGER_IMPL_H_

#include "ace/Vector_T.h"
#include "ace/SStringfwd.h"
#include "ace/Map_Manager.h"
#include "DomainApplicationManager_Export.h"
#include "Deployment/Deployment_DomainApplicationManagerS.h"
#include "Deployment/Deployment_NodeApplicationManagerC.h"
#include "DomainApplication/Domain_Application_Impl.h"
#include "Deployment/Deployment_NodeManagerC.h"

namespace DAnCE
  {
  class DomainApplicationManager_Export DomainApplicationManager_Impl
        : public virtual POA_Deployment::DomainApplicationManager
    {
    private:
      typedef ACE_Map_Manager<ACE_CString, Deployment::DeploymentPlan, ACE_Null_Mutex> TNodePlans;

    public:
      typedef ACE_Map_Manager<ACE_CString, Deployment::NodeManager_var, ACE_Null_Mutex> TNodeManagers;

      DomainApplicationManager_Impl (CORBA::ORB_ptr orb,
                                     PortableServer::POA_ptr poa,
                                     const Deployment::DeploymentPlan& plan,
                                     TNodeManagers & nodes);

      virtual ~DomainApplicationManager_Impl();

      virtual Deployment::Application_ptr
      startLaunch (const Deployment::Properties & configProperty,
                   Deployment::Connections_out providedReference);

      virtual void destroyApplication (Deployment::Application_ptr);

      virtual ::Deployment::Applications * getApplications (
      );

      virtual ::Deployment::DeploymentPlan * getPlan (
      );

      void dump_connections (const ::Deployment::Connections & connections) const;

    private:
      CORBA::ORB_var orb_;
      PortableServer::POA_var poa_;
      const Deployment::DeploymentPlan plan_;
      DomainApplication_Impl::TNam2Nm sub_app_mgr_;
      TNodeManagers & nodes_;

      typedef ACE_Vector<DomainApplication_Impl*> TApplications;
      TApplications running_app_;

      static void split_plan (const Deployment::DeploymentPlan & plan,
                              TNodePlans & sub_plans);

      void preparePlan();
    };

};//DAnCE

#endif /*DOMAINAPPLICATIONMANAGER_IMPL_H_*/
