/**
 * @file Remove.h
 * @author William R. Otte <wotte@dre.vanderbilt.edu>
 * 
 * $Id$
 *
 */

#ifndef EVENT_REMOVE_H
#define EVENT_REMOVE_H

#include "LocalityManager/Scheduler/Deployment_Scheduler_export.h"
#include "LocalityManager/Scheduler/Deployment_Event.h"
#include "Deployment/Deployment_DeploymentPlanC.h"

namespace DAnCE
{
  /**
   * @class
   */
  class Deployment_Scheduler_Export Remove_Instance :
    public virtual Deployment_Event
  {
  public:
    Remove_Instance (::Deployment::DeploymentPlan & plan,
                     ::CORBA::ULong instanceRef,
                     const CORBA::Any &reference,
                     const char *instance_type,
                     Event_Future holder);
    
    virtual ~Remove_Instance (void);

    virtual int call (void);

  private:
    ::Deployment::DeploymentPlan & plan_;
    ::CORBA::ULong instanceRef_;
    ::CORBA::Any reference_;
  };  
}

#endif
