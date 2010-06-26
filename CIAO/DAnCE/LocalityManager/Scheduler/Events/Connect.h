/**
 * @file Connect.h
 * @author William R. Otte <wotte@dre.vanderbilt.edu>
 * 
 * $Id$
 *
 */

#ifndef EVENT_CONNECT_H
#define EVENT_CONNECT_H

#include "LocalityManager/Scheduler/Deployment_Scheduler_export.h"
#include "LocalityManager/Scheduler/Events/Action_Base.h"
#include "Deployment/Deployment_DeploymentPlanC.h"

namespace DAnCE
{
  /**
   * @class
   */
  class Deployment_Scheduler_Export Connect_Instance :
    public virtual Action_Base
  {
  public:
    Connect_Instance (::Deployment::DeploymentPlan & plan,
                      ::CORBA::ULong connectionRef,
                      const ::CORBA::Any &provided_ref,
                      const char *instance_type,
                      Event_Future holder);
    
    virtual ~Connect_Instance (void);

  protected:
    virtual void invoke_pre_interceptor (Plugin_Manager::INTERCEPTORS::const_iterator &);
    
    virtual void invoke (::DAnCE::InstanceDeploymentHandler_ptr);
    
    virtual void invoke_post_interceptor (Plugin_Manager::INTERCEPTORS::const_iterator &);
    
    virtual void create_unexpected_exception (const std::string &,
                                              const std::string &);
    
    virtual void create_valid_result (Event_Result &);

  private:
    ::Deployment::DeploymentPlan &plan_;
    ::CORBA::ULong connectionRef_;
    ::CORBA::Any provided_ref_;
  };  
}

#endif
