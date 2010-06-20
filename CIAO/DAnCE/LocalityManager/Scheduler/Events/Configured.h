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
#include "LocalityManager/Scheduler/Deployment_Event.h"
#include "Deployment/Deployment_DeploymentPlanC.h"

namespace DAnCE
{
  /**
   * @class
   */
  class Deployment_Scheduler_Export Instance_Configured :
    public virtual Deployment_Event
  {
  public:
    Instance_Configured (::Deployment::DeploymentPlan & plan,
                         ::CORBA::ULong instanceRef,
                         const char *instance_type,
                         Event_Future holder);
    
    virtual ~Instance_Configured (void);

    virtual int call (void);

  private:
    ::Deployment::DeploymentPlan &plan_;
    ::CORBA::ULong instanceRef_;
  };  
}

#endif
