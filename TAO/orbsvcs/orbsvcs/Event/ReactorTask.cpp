// $Id$

#include "ace/High_Res_Timer.h"
#include "tao/Timeprobe.h"
#include "orbsvcs/Scheduler_Factory.h"

#include "ReactorTask.h"

ACE_RCSID(Event, ReactorTask, "$Id$")

#if defined (ACE_ENABLE_TIMEPROBES)

static const char *TAO_Reactor_Task_Timeprobe_Description[] =
{
  "Reactor_Task - waiting for events",
  "Reactor_Task - events handled"
};

enum
{
  // Timeprobe description table start key 
  TAO_REACTOR_TASK_WAITING_FOR_EVENTS = 5300,
  TAO_REACTOR_TASK_EVENTS_HANDLED
};

// Setup Timeprobes
ACE_TIMEPROBE_EVENT_DESCRIPTIONS (TAO_Reactor_Task_Timeprobe_Description,
                                  TAO_REACTOR_TASK_WAITING_FOR_EVENTS);

#endif /* ACE_ENABLE_TIMEPROBES */

ACE_ES_Reactor_Task::
    ACE_ES_Reactor_Task (RtecScheduler::Scheduler_ptr scheduler)
      :  ACE_RT_Task (scheduler),
         //  reactor_ (0, &timer_queue_),
         done_ (0)
{
  // Change the timer mechanism used by the reactor and the timer
  // queue.
  timer_queue_.gettimeofday (ACE_OS::gettimeofday);
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
ACE_ES_Reactor_Task::open_reactor (RtecScheduler::Period_t &period)
{
  // Create a name for ourselves using the period.  The period is
  // in 100 ns units; first convert to usec by dividing by 10.
  char temp[64];
  ACE_OS::sprintf (temp, "Reactor_Task-%u.us", period / 10);

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
#if 1
            this->scheduler_->set
              (rt_info_,
               RtecScheduler::VERY_HIGH_CRITICALITY,
               ORBSVCS_Time::zero,
               ORBSVCS_Time::zero,
               ORBSVCS_Time::zero,
               period,
               RtecScheduler::VERY_LOW_IMPORTANCE,
               ORBSVCS_Time::zero,
               1,
               RtecScheduler::OPERATION,
               TAO_TRY_ENV);
#else
            ACE_Scheduler_Factory::server()->set
              (rt_info_,
               RtecScheduler::VERY_HIGH_CRITICALITY,
               ORBSVCS_Time::zero,
               ORBSVCS_Time::zero,
               ORBSVCS_Time::zero,
               period,
               RtecScheduler::VERY_LOW_IMPORTANCE,
               ORBSVCS_Time::zero,
               1,
               RtecScheduler::OPERATION,
               TAO_TRY_ENV);
#endif
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
    ACE_DEBUG ((LM_DEBUG, "EC (%t) Timer Task is done.\n"));

  return done_;
}

void ACE_ES_Reactor_Task::threads_closed()
{
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
