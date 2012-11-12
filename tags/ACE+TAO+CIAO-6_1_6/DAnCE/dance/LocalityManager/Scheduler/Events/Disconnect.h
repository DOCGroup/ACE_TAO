/**
 * @file Disconnect.h
 * @author Johnny Willemsen  <jwillemsen@remedy.nl>
 *
 * $Id$
 *
 */

#ifndef EVENT_DISCONNECT_H
#define EVENT_DISCONNECT_H

#include "dance/LocalityManager/Scheduler/Deployment_Scheduler_export.h"
#include "dance/LocalityManager/Scheduler/Events/Action_Base.h"
#include "dance/Deployment/Deployment_DeploymentPlanC.h"

namespace DAnCE
{
  /**
   * @class
   */
  class Deployment_Scheduler_Export Disconnect_Instance :
    public virtual Action_Base
  {
  public:
    Disconnect_Instance (::Deployment::DeploymentPlan & plan,
                         ::CORBA::ULong connectionRef,
                         const char *instance_type,
                         Event_Future holder);

    virtual ~Disconnect_Instance (void);

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
  };
}

#endif
