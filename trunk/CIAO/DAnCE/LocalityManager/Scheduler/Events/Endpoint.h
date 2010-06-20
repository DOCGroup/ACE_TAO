/**
 * @file Endpoint.h
 * @author William R. Otte <wotte@dre.vanderbilt.edu>
 * 
 * $Id$
 *
 */

#ifndef EVENT_ENDPOINT_H
#define EVENT_ENDPOINT_H

#include "LocalityManager/Scheduler/Deployment_Scheduler_export.h"
#include "LocalityManager/Scheduler/Deployment_Event.h"
#include "Deployment/Deployment_DeploymentPlanC.h"

namespace DAnCE
{
  /**
   * @class
   */
  class Deployment_Scheduler_Export Endpoint_Reference :
    public virtual Deployment_Event
  {
  public:
    Endpoint_Reference (::Deployment::DeploymentPlan & plan,
                        ::CORBA::ULong connectionRef,
                        const char *instance_type,
                        Event_Future holder);
    
    virtual ~Endpoint_Reference (void);

    virtual int call (void);

  private:
    ::Deployment::DeploymentPlan &plan_;
    ::CORBA::ULong connectionRef_;
  };  
}

#endif
