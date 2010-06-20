/**
 * @file Passivate.h
 * @author William R. Otte <wotte@dre.vanderbilt.edu>
 * 
 * $Id$
 *
 */

#ifndef EVENT_PASSIVATE_H
#define EVENT_PASSIVATE_H

#include "LocalityManager/Scheduler/Deployment_Scheduler_export.h"
#include "LocalityManager/Scheduler/Deployment_Event.h"
#include "Deployment/Deployment_DeploymentPlanC.h"

namespace DAnCE
{
  /**
   * @class
   */
  class Deployment_Scheduler_Export Passivate_Instance :
    public virtual Deployment_Event
  {
  public:
    Passivate_Instance (::Deployment::DeploymentPlan & plan,
                        ::CORBA::ULong instanceRef,
                        const CORBA::Any &ref,
                        const char *instance_type,
                        Event_Future holder);
    
    virtual ~Passivate_Instance (void);

    virtual int call (void);

  private:
    ::Deployment::DeploymentPlan &plan_;
    ::CORBA::ULong instanceRef_;
    const ::CORBA::Any &ref_;
  };  
}

#endif
