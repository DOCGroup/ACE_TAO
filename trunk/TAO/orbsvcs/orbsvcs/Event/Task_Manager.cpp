// $Id$

#include "Task_Manager.h"
#include "ReactorTask.h"

#if ! defined (__ACE_INLINE__)
#include "Task_Manager.i"
#endif /* __ACE_INLINE__ */

ACE_Task_Manager::ACE_Task_Manager (void)
{
  for (int i = 0; i < ACE_Scheduler_MAX_PRIORITIES; ++i)
    this->reactorTasks[i] = 0;
}

void ACE_Task_Manager::activate (void)
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
      
      this->reactorTasks[i]->thr_mgr (this->ThrMgr ());

      if (this->reactorTasks[i]->open_reactor (period) == -1)
        {
          ACE_ERROR ((LM_ERROR, "%p\n",
		      "EC (%t) Task_Manager - open reactor"));
        }
    }
}

void
ACE_Task_Manager::shutdown (void)
{
  for (int i = 0; i < ACE_Scheduler_MAX_PRIORITIES; ++i)
    {
      if (this->reactorTasks[i] != 0)
	this->reactorTasks[i]->shutdown_task ();
    }

  if (this->ThrMgr ()->wait () == -1)
    ACE_ERROR ((LM_DEBUG, "%p\n", "EC (%t) Task_Manager wait"));

  for (int j = 0; j < ACE_Scheduler_MAX_PRIORITIES; ++j)
    {
      if (this->reactorTasks[j] != 0)
	{
	  delete this->reactorTasks[j];
	  this->reactorTasks[j] = 0;
	}
    }
}
