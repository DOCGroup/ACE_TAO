// $Id$

#include "orbsvcs/orbsvcs/Event/ReactorTask.h"
#include "orbsvcs/orbsvcs/Event/Timer_Module.h"

#if ! defined (__ACE_INLINE__)
#include "Timer_Module.i"
#endif /* __ACE_INLINE__ */

ACE_RCSID(Event, Timer_Module, "$Id$")

// ****************************************************************

TAO_EC_Timer_Module::~TAO_EC_Timer_Module (void)
{
}

// ****************************************************************

TAO_EC_ST_Timer_Module::TAO_EC_ST_Timer_Module (ACE_Reactor* r)
  :  reactor_ (r)
{
}

TAO_EC_ST_Timer_Module::~TAO_EC_ST_Timer_Module (void)
{
}

void
TAO_EC_ST_Timer_Module::activate (void)
{
}

void
TAO_EC_ST_Timer_Module::shutdown (void)
{
}

RtecScheduler::handle_t
TAO_EC_ST_Timer_Module::rt_info (RtecScheduler::Preemption_Priority)
{
  // @@ TODO......
  return 0;
}

int
TAO_EC_ST_Timer_Module::schedule_timer (RtecScheduler::Preemption_Priority,
                                        ACE_Event_Handler* eh,
                                        void* act,
                                        const ACE_Time_Value& delta,
                                        const ACE_Time_Value& interval)
{
  return this->reactor_->schedule_timer (eh, act, delta, interval);
}

int
TAO_EC_ST_Timer_Module::cancel_timer (RtecScheduler::Preemption_Priority,
                                      int id,
                                      const void*& act)
{
  return this->reactor_->cancel_timer (id, &act);
}

int
TAO_EC_ST_Timer_Module::register_handler (RtecScheduler::Preemption_Priority,
                                          ACE_Event_Handler* eh,
                                          ACE_HANDLE handle)
{
  return this->reactor_->register_handler (eh, handle);
}

ACE_Reactor*
TAO_EC_ST_Timer_Module::reactor (RtecScheduler::Preemption_Priority)
{
  return this->reactor_;
}

// ****************************************************************

TAO_EC_RPT_Timer_Module::TAO_EC_RPT_Timer_Module (void)
  : shutdown_ (0)
{
  for (int i = 0; i < ACE_Scheduler_MAX_PRIORITIES; ++i)
    this->reactorTasks[i] = 0;
}

TAO_EC_RPT_Timer_Module::~TAO_EC_RPT_Timer_Module (void)
{
  this->shutdown ();

  for (int i = 0; i < ACE_Scheduler_MAX_PRIORITIES; ++i)
    {
      if (this->reactorTasks[i] != 0)
	{
	  delete this->reactorTasks[i];
	  this->reactorTasks[i] = 0;
	}
    }
}

void TAO_EC_RPT_Timer_Module::activate (void)
{
  for (int i = 0; i < ACE_Scheduler_MAX_PRIORITIES; ++i)
    {
      if (this->reactorTasks[i] != 0)
	continue;

      // Convert ACE_Scheduler_Rate (it's really a period, not a rate!)
      // to a form we can easily work with.
      ACE_Time_Value period_tv;
      ORBSVCS_Time::TimeT_to_Time_Value (period_tv, ACE_Scheduler_Rates[i]);

      RtecScheduler::Period period = period_tv.sec () * 10000000 +
                                     period_tv.usec () * 10;

      ACE_NEW (this->reactorTasks[i], ReactorTask);

      if (!this->shutdown_)
	{
	  this->reactorTasks[i]->thr_mgr (this->ThrMgr ());
	  if (this->reactorTasks[i]->open_reactor (period) == -1)
	    {
	      ACE_ERROR ((LM_ERROR, "%p\n",
			  "EC (%t) Timer_Module - open reactor"));
	    }
	}
    }
}

void
TAO_EC_RPT_Timer_Module::shutdown (void)
{
  if (this->shutdown_)
    return;

  this->shutdown_ = 1;
  for (int i = 0; i < ACE_Scheduler_MAX_PRIORITIES; ++i)
    {
      if (this->reactorTasks[i] != 0)
	this->reactorTasks[i]->shutdown_task ();
    }

  if (this->ThrMgr ()->wait () == -1)
    ACE_ERROR ((LM_DEBUG, "%p\n", "EC (%t) Timer_Module wait"));
}

RtecScheduler::handle_t
TAO_EC_RPT_Timer_Module::rt_info (RtecScheduler::Preemption_Priority priority)
{
  return this->GetReactorTask (priority)->rt_info ();
}

int
TAO_EC_RPT_Timer_Module::schedule_timer (RtecScheduler::Preemption_Priority priority,
                                         ACE_Event_Handler* eh,
                                         void* act,
                                         const ACE_Time_Value& delta,
                                         const ACE_Time_Value& interval)
{
  return this->GetReactorTask (priority)->get_reactor ().schedule_timer (eh, act, delta, interval);
}

int
TAO_EC_RPT_Timer_Module::cancel_timer (RtecScheduler::Preemption_Priority priority,
                                       int id,
                                       const void*& act)
{
  return this->GetReactorTask (priority)->get_reactor ().cancel_timer (id, &act);
}

int
TAO_EC_RPT_Timer_Module::register_handler (RtecScheduler::Preemption_Priority priority,
                                           ACE_Event_Handler* eh,
                                           ACE_HANDLE handle)
{
  return this->GetReactorTask (priority)->get_reactor ().register_handler (eh, handle);
}

ACE_Reactor*
TAO_EC_RPT_Timer_Module::reactor (RtecScheduler::Preemption_Priority priority)
{
  return &this->GetReactorTask (priority)->get_reactor ();
}
