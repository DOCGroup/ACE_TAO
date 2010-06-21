/**
 * @file Install.h
 * @author William R. Otte <wotte@dre.vanderbilt.edu>
 *
 * $Id$
 *
 */

#ifndef EVENT_INSTALL_H
#define EVENT_INSTALL_H

#include "LocalityManager/Scheduler/Deployment_Scheduler_export.h"
#include "LocalityManager/Scheduler/Deployment_Event.h"
#include "Deployment/Deployment_DeploymentPlanC.h"

namespace DAnCE
{
  /**
   * @class
   */
  class Deployment_Scheduler_Export Install_Instance :
    public virtual Deployment_Event
  {
  public:
    Install_Instance (::Deployment::DeploymentPlan & plan,
                      ::CORBA::ULong instanceRef,
                      const char *instance_type,
                      Event_Future holder);

    virtual ~Install_Instance (void);

    virtual int call (void);

  private:
    ::Deployment::DeploymentPlan &plan_;
    ::CORBA::ULong instanceRef_;
  };
}

#endif
