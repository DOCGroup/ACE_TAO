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
#include "LocalityManager/Scheduler/Deployment_Event.h"
#include "Deployment/Deployment_DeploymentPlanC.h"

namespace DAnCE
{
  /**
   * @class
   */
  class Deployment_Scheduler_Export Start_Instance :
    public virtual Deployment_Event
  {
  public:
    Start_Instance (::Deployment::DeploymentPlan & plan,
                    ::CORBA::ULong instanceRef,
                    const CORBA::Any &instance_ref,
                    const char *instance_type,
                    Event_Future holder);
    
    virtual ~Start_Instance (void);

    virtual int call (void);

  private:
    ::Deployment::DeploymentPlan &plan_;
    ::CORBA::ULong instanceRef_;
    const CORBA::Any &ref_;
  };  
}

#endif
