/**
 * @file Plan_Launcher_Base.h
 * @author William R. Otte <wotte@dre.vanderbilt.edu>
 *
 * $Id$
 *
 * ABC for the EM/NM bridges.
 */

#ifndef DANCE_PLAN_LAUNCHER_BASE
#define DANCE_PLAN_LAUNCHER_BASE

#include "Deployment/Deployment_DeploymentPlanC.h"
#include "Deployment/Deployment_ConnectionC.h"

namespace DAnCE
{
  class Plan_Launcher_Base
  {
  public:
    virtual const char * launch_plan (const ::Deployment::DeploymentPlan &plan,
                                      CORBA::Object_out am,
                                      CORBA::Object_out app) = 0;
    
    virtual CORBA::Object_ptr prepare_plan (const ::Deployment::DeploymentPlan &plan) = 0;
  
    virtual CORBA::Object_ptr start_launch (CORBA::Object_ptr app_mgr,
                                            const ::Deployment::Properties &properties,
                                            ::Deployment::Connections_out connections) = 0;
    
    virtual void finish_launch (CORBA::Object_ptr application,
                                const ::Deployment::Connections &provided_connections,
                                bool start) = 0;
    
    virtual void start (CORBA::Object_ptr application) = 0;
  
    /// Tears down a plan given the UUID
    virtual void teardown_application (CORBA::Object_ptr app_mgr,
                                       CORBA::Object_ptr app) = 0;
    
    virtual void destroy_app_manager (CORBA::Object_ptr app_mgr) = 0;

    virtual void create_external_connections(const ::Deployment::DeploymentPlan &plan,
                                             Deployment::Connections  &conn) = 0;
  };
}

#endif
