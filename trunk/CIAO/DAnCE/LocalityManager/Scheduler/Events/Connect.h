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
#include "LocalityManager/Scheduler/Deployment_Event.h"
#include "Deployment/Deployment_DeploymentPlanC.h"

namespace DAnCE
{
  /**
   * @class
   */
  class Deployment_Scheduler_Export Connect_Instance :
    public virtual Deployment_Event
  {
  public:
    Connect_Instance (::Deployment::DeploymentPlan & plan,
                      ::CORBA::ULong connectionRef,
                      const ::CORBA::Any &provided_ref,
                      const char *instance_type,
                      Event_Future holder);
    
    virtual ~Connect_Instance (void);

    virtual int call (void);

  private:
    ::Deployment::DeploymentPlan &plan_;
    ::CORBA::ULong connectionRef_;
    ::CORBA::Any provided_ref_;
  };  
}

#endif
