// $Id$

#include "ace/Functor.h"

#include "orbsvcs/Scheduler_Factory.h"
#include "ReactorTask.h"
#include "Timer_Module.h"

#if ! defined (__ACE_INLINE__)
#include "Timer_Module.i"
#endif /* __ACE_INLINE__ */

ACE_RCSID(Event, Timer_Module, "$Id$")

#include "tao/Timeprobe.h"

#if defined (ACE_ENABLE_TIMEPROBES)
static const char *TAO_Timer_Module_Timeprobe_Description[] =
{
  "Timer_Module - start execute",
  "Timer_Module - end execute"
};

enum
{
  // Timeprobe description table start key
  TAO_EC_TIMER_MODULE_START_EXECUTE = 5400,
  TAO_EC_TIMER_MODULE_END_EXECUTE
};

// Setup Timeprobes
ACE_TIMEPROBE_EVENT_DESCRIPTIONS (TAO_Timer_Module_Timeprobe_Description,
                                  TAO_EC_TIMER_MODULE_START_EXECUTE);

#endif /* ACE_ENABLE_TIMEPROBES */

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
  this->reactor_->cancel_timer (&this->timeout_handler_);
}

RtecScheduler::handle_t
TAO_EC_ST_Timer_Module::rt_info (RtecScheduler::Preemption_Priority_t)
{
  // @@ TODO......
  return 0;
}

int
TAO_EC_ST_Timer_Module::schedule_timer (RtecScheduler::Preemption_Priority_t,
                                        ACE_Command_Base* act,
                                        const ACE_Time_Value& delta,
                                        const ACE_Time_Value& interval)
{
  return this->reactor_->schedule_timer (&this->timeout_handler_,
                                         ACE_static_cast(void*,act),
                                         delta, interval);
}

int
TAO_EC_ST_Timer_Module::cancel_timer (RtecScheduler::Preemption_Priority_t,
                                      int id,
                                      ACE_Command_Base*& act)
{
  const void *vp;

  int result =
    this->reactor_->cancel_timer (id, &vp);
  if (result == 0)
    {
      ACE_ERROR ((LM_ERROR, "TAO_EC_ST_Timer_Module::cancel_timer: "
                  "Tried to cancel nonexistent timer.\n"));
      act = 0;
    }
  else
    act = ACE_reinterpret_cast (ACE_Command_Base *,
                                ACE_const_cast (void *, vp));

  return result;
}

int
TAO_EC_ST_Timer_Module::register_handler (RtecScheduler::Preemption_Priority_t,
                                          ACE_Event_Handler* eh,
                                          ACE_HANDLE handle)
{
  return this->reactor_->register_handler (eh, handle);
}

ACE_Reactor*
TAO_EC_ST_Timer_Module::reactor (RtecScheduler::Preemption_Priority_t)
{
  return this->reactor_;
}

// ****************************************************************

TAO_EC_RPT_Timer_Module::
  TAO_EC_RPT_Timer_Module (RtecScheduler::Scheduler_ptr scheduler)
  : shutdown_ (0)
{
  if (CORBA::is_nil (scheduler))
    {
      this->scheduler_ = 
        RtecScheduler::Scheduler::_duplicate (ACE_Scheduler_Factory::server ());
    }
  else
    {
      this->scheduler_ = 
        RtecScheduler::Scheduler::_duplicate (scheduler);
    }
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

      RtecScheduler::Period_t period = period_tv.sec () * 10000000 +
                                       period_tv.usec () * 10;

      ACE_NEW (this->reactorTasks[i],
               ReactorTask (this->scheduler_.in ()));

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
      this->reactorTasks[i]->get_reactor ().cancel_timer (&this->timeout_handler_);
    }

  if (this->ThrMgr ()->wait () == -1)
    ACE_ERROR ((LM_DEBUG, "%p\n", "EC (%t) Timer_Module wait"));
}

RtecScheduler::handle_t
TAO_EC_RPT_Timer_Module::rt_info (RtecScheduler::Preemption_Priority_t priority)
{
  return this->GetReactorTask (priority)->rt_info ();
}

int
TAO_EC_RPT_Timer_Module::schedule_timer (RtecScheduler::Preemption_Priority_t priority,
                                         ACE_Command_Base* act,
                                         const ACE_Time_Value& delta,
                                         const ACE_Time_Value& interval)
{
  ACE_Reactor& reactor = this->GetReactorTask (priority)->get_reactor ();
  return reactor.schedule_timer (&this->timeout_handler_,
                                 ACE_static_cast(void*,act),
                                 delta, interval);
}

int
TAO_EC_RPT_Timer_Module::cancel_timer (RtecScheduler::Preemption_Priority_t priority,
                                       int id,
                                       ACE_Command_Base*& act)
{
  const void* vp;
  ACE_Reactor& reactor = this->GetReactorTask (priority)->get_reactor ();

  int result =
    reactor.cancel_timer (id, &vp);
  if (result == 0)
    {
      ACE_ERROR ((LM_ERROR, "TAO_EC_ST_Timer_Module::cancel_timer: "
                  "Tried to cancel nonexistent timer.\n"));
      act = 0;
    }
  else
    act = ACE_reinterpret_cast (ACE_Command_Base *,
                                ACE_const_cast (void *, vp));

  return result;
}

int
TAO_EC_RPT_Timer_Module::register_handler (RtecScheduler::Preemption_Priority_t priority,
                                           ACE_Event_Handler* eh,
                                           ACE_HANDLE handle)
{
  return this->GetReactorTask (priority)->get_reactor ().register_handler (eh, handle);
}

ACE_Reactor*
TAO_EC_RPT_Timer_Module::reactor (RtecScheduler::Preemption_Priority_t priority)
{
  return &this->GetReactorTask (priority)->get_reactor ();
}

// ****************************************************************

int
TAO_EC_Timeout_Handler::handle_timeout (const ACE_Time_Value &,
                                        const void *vp)
{
  ACE_Command_Base *act = ACE_static_cast(ACE_Command_Base*,
                                          ACE_const_cast(void*,vp));

  if (act == 0)
    ACE_ERROR_RETURN ((LM_ERROR, "ACE_ES_Priority_Timer::handle_timeout: "
                       "received act == 0!!!.\n"), 0);

  {
//    ACE_FUNCTION_TIMEPROBE (TAO_EVENT_CHANNEL_ES_PRIORITY_QUEUE_START_EXECUTE);

    act->execute ();
  }

  return 0;
}
