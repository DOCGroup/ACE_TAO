// $Id$

#include "Deployment_Scheduler.h"
#include "Logger/Log_Macros.h"


#if !defined (__ACE_INLINE__)
#include "LocalityManager/Scheduler/Deployment_Scheduler.inl"
#endif

namespace DAnCE
{
  Deployment_Scheduler::Deployment_Scheduler (void)
    : multithread_ (false)
    {
    }

  Deployment_Scheduler::~Deployment_Scheduler (void)
    {
    }

  int
  Deployment_Scheduler::schedule_event (Deployment_Event *event)
  {
    int retval = this->event_queue_.enqueue (event);

    if (retval == -1)
      {
        DANCE_ERROR (1, (LM_ERROR, DLINFO
                         ACE_TEXT ("Deployment_Scheduler::schedule_event - ")
                         ACE_TEXT ("Error: Unable to schedule event for execution\n")));
      }

    return retval;
  }

  void
  Deployment_Scheduler::terminate_scheduler (void)
  {
    this->event_queue_.queue ()->close ();
  }

  int
  Deployment_Scheduler::svc (void)
  {
    for ( ; ; )
      {
        auto_ptr < ACE_Method_Request > de (this->event_queue_.dequeue ());

        if (de.get ())
          {
            DANCE_DEBUG (10, (LM_TRACE, DLINFO
                              ACE_TEXT ("Deployment_Scheduler::svc - ")
                              ACE_TEXT ("Invoking a deployment event\n")));
            de->call ();
          }
        else
          {
            break;
          }
      }

    return 0;
  }
}
