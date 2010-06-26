/**
 * @file Start.h
 * @author William R. Otte <wotte@dre.vanderbilt.edu>
 * 
 * $Id$
 *
 */

#ifndef EVENT_START_H
#define EVENT_START_H

#include "LocalityManager/Scheduler/Deployment_Scheduler_export.h"
#include "LocalityManager/Scheduler/Events/Action_Base.h"
#include "Deployment/Deployment_DeploymentPlanC.h"

namespace DAnCE
{
  /**
   * @class
   */
  class Deployment_Scheduler_Export Start_Instance :
    public virtual Action_Base
  {
  public:
    Start_Instance (::Deployment::DeploymentPlan & plan,
                    ::CORBA::ULong instanceRef,
                    const CORBA::Any &instance_ref,
                    const char *instance_type,
                    Event_Future holder);
    
    virtual ~Start_Instance (void);

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
    const CORBA::Any &ref_;
  };  
}

#endif
