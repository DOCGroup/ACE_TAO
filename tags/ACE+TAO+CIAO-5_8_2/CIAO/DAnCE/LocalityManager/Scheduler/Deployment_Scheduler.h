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
#include "LocalityManager/Scheduler/Deployment_Event.h"

namespace DAnCE
{
  class Deployment_Scheduler_Export Deployment_Scheduler :
    public ACE_Task_Base
  {
  public:
    Deployment_Scheduler (void);
    virtual ~Deployment_Scheduler (void);

    /// Schedule an event for execution
    int schedule_event (Deployment_Event *event);

    void activate_scheduler (size_t threads);

    void terminate_scheduler (void);

    bool multithreaded (void);

    bool work_pending (void);

    void perform_work (void);

  protected:
    /// Scheduler event loop
    virtual int svc (void);

  private:
    bool multithread_;

    ACE_Activation_Queue event_queue_;
  };
}

#if defined (__ACE_INLINE__)
#include "LocalityManager/Scheduler/Deployment_Scheduler.inl"
#endif

#endif
