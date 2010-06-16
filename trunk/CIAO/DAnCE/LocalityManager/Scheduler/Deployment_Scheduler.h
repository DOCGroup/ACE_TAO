/**
 * @file Deployment_Scheduler.h
 * @author William R. Otte <wotte@dre.vanderbilt.edu>
 *
 * $Id$
 * Active-object based scheduler for deployment events. 
 */
#ifndef DEPLOYMENT_SCHEDULER_H
#define DEPLOYMENT_SCHEDULER_H

#include "ace/Task.h"
#include "ace/Activation_Queue.h"
#include "LocalityManager/Scheduler/Deployment_Events.h"

namespace DAnCE
{
  class Deployment_Scheduler : public ACE_Task_Base
  {
  public:
    /// Schedule an event for execution
    int schedule_event (Deployment_Event *event);

  protected:
    /// Scheduler event loop
    virtual int svc (void);
    
  private:
    ACE_Activation_Queue event_queue_;
  };
}

#endif
