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
#include "DAnCE/Plan_Launcher/Plan_Launcher_Export.h"

namespace DAnCE
{
  class DAnCE_Plan_Launcher_Export Plan_Launcher_Base
  {
  public:
    virtual ~Plan_Launcher_Base (void) {};
    
    /// Fully launches a nominated plan, returning valid object references to
    /// the ApplicationManager and Application objects if successful. 
    virtual const char * launch_plan (const ::Deployment::DeploymentPlan &plan,
                                      CORBA::Object_out am,
                                      CORBA::Object_out app) = 0;
    
    /// Invokes prepareplan for the provided plan, returning a reference to
    /// the ApplicationManager. 
    virtual CORBA::Object_ptr prepare_plan (const ::Deployment::DeploymentPlan &plan) = 0;
  
    /// Invokes startLaunch on the provided ApplicationManager reference.
    virtual CORBA::Object_ptr start_launch (CORBA::Object_ptr app_mgr,
                                            const ::Deployment::Properties &properties,
                                            ::Deployment::Connections_out connections) = 0;
    
    /// Invokes finishLaunch on the provided Application reference. 
    virtual void finish_launch (CORBA::Object_ptr application,
                                const ::Deployment::Connections &provided_connections,
                                bool start) = 0;
    
    /// Invokes start on the provided application reference. 
    virtual void start (CORBA::Object_ptr application) = 0;
  
    /// Tears down a plan given an applicationmanafger and Application
    /// reference pair. 
    virtual void teardown_application (CORBA::Object_ptr app_mgr,
                                       CORBA::Object_ptr app) = 0;
    
    /// Instructs the Manager to destroy the ApplicationManager. 
    virtual void destroy_app_manager (CORBA::Object_ptr app_mgr) = 0;
    
    /// Helper method to resolve externalReference endpoints into valid object references.
    virtual void create_external_connections(const ::Deployment::DeploymentPlan &plan,
                                             Deployment::Connections  &conn) = 0;
  };
}

#endif
