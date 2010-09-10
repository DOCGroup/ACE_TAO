/**
 * @file Configured.h
 * @author William R. Otte <wotte@dre.vanderbilt.edu>
 * 
 * $Id$
 *
 */

#ifndef EVENT_CONFIGURED_H
#define EVENT_CONFIGURED_H

#include "LocalityManager/Scheduler/Deployment_Scheduler_export.h"
#include "LocalityManager/Scheduler/Events/Action_Base.h"
#include "Deployment/Deployment_DeploymentPlanC.h"

namespace DAnCE
{
  /**
   * @class
   */
  class Deployment_Scheduler_Export Instance_Configured :
    public virtual Action_Base
  {
  public:
    Instance_Configured (::Deployment::DeploymentPlan & plan,
                         ::CORBA::ULong instanceRef,
                         const char *instance_type,
                         Event_Future holder);
    
    virtual ~Instance_Configured (void);

  protected:
    virtual void invoke_pre_interceptor (Plugin_Manager::INTERCEPTORS::const_iterator &);
    
    virtual void invoke (::DAnCE::InstanceDeploymentHandler_ptr);
    
    virtual void invoke_post_interceptor (Plugin_Manager::INTERCEPTORS::const_iterator &);
    
    virtual void create_unexpected_exception (const std::string &,
                                              const std::string &);
    
    virtual void create_valid_result (Event_Result &);
    
  private:
    ::Deployment::DeploymentPlan &plan_;
    ::CORBA::ULong instanceRef_;
  };  
}

#endif
