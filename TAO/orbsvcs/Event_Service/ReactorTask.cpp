//
// $Id$
//
#include "ace/High_Res_Timer.h"
#include "tao/Timeprobe.h"
#include "orbsvcs/Scheduler_Factory.h"

#include "ReactorTask.h"

ACE_ES_Reactor_Task::ACE_ES_Reactor_Task() :
  //  reactor_ (0, &timer_queue_),
  done_ (0)
{
  // Change the timer mechanism used by the reactor and the timer
  // queue.
#if defined (VXWORKS)
  timer_queue_.gettimeofday (ACE_OS::gettimeofday);
#else
  timer_queue_.gettimeofday (ACE_High_Res_Timer::gettimeofday);
#endif /* VXWORKS */
}

ACE_ES_Reactor_Task::~ACE_ES_Reactor_Task (void)
{
}

int
ACE_ES_Reactor_Task::svc_hook(RtecScheduler::OS_Priority)
{
  // Make ourselves owner of the reactor.
  reactor_.owner (ACE_Thread::self());
  return 0;
}

int
ACE_ES_Reactor_Task::open_reactor (RtecScheduler::Period &period)
{
  // Create a name for ourself using the priority.
  char temp[64];
  ACE_OS::sprintf (temp, "Reactor_Task-%u", period);

  // Open the task.  This will query the scheduler for our qos
  // structure.
  int result = this->open_task (temp);

  switch (result)
    {
    case -1:
      // Error.
      ACE_ERROR ((LM_ERROR, "(%t) Scheduler could not find operation %s.\n",
                  temp));
      return -1;

    case 0:
      // @@ TODO handle exceptions
      {
	TAO_TRY
	  {
	    ACE_Scheduler_Factory::server()->set(rt_info_,
						 0, 0, 0, period,
						 RtecScheduler::VERY_LOW,
						 RtecScheduler::NO_QUANTUM,
						 1, TAO_TRY_ENV);
	    TAO_CHECK_ENV;
	  }
	TAO_CATCHANY
	  {
	    ACE_ERROR_RETURN ((LM_ERROR, "set failed\n"), -1);
	  }
	TAO_ENDTRY;
      }
      break;

    case 1:
      // Found.
      break;
    }

  return this->synch_threads (1);
}

int ACE_ES_Reactor_Task::svc_one()
{
  ACE_TIMEPROBE (" Reactor_Task - waiting for events");
  if (reactor_.handle_events() == -1)
    ACE_ERROR ((LM_ERROR, "(%t) %p.\n", "ACE_ES_Reactor_Task::svc"));
  ACE_TIMEPROBE (" Reactor_Task - events handled");

  if (done_)
    ACE_DEBUG ((LM_DEBUG, "(%t) Timer Task is done.\n"));

  return done_;
}

void ACE_ES_Reactor_Task::threads_closed()
{
  delete this;
}

void ACE_ES_Reactor_Task::shutdown_task()
{
  done_ = 1;
  reactor_.notify();
}

ACE_ES_Reactor_Task::Reactor& 
ACE_ES_Reactor_Task::get_reactor()
{
  return reactor_;
}
