// $Id$

#include "Deployment_Scheduler.h"
#include "Logger/Log_Macros.h"

namespace DAnCE
{
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
            DANCE_ERROR (1, (LM_ERROR, DLINFO
                             ACE_TEXT ("Deployment_Scheduler::svc - ")
                             ACE_TEXT ("Failed to retrieve deployment event from queue")));
          }
      }
  }
}
