// $Id$

#include "Task_Manager.h"
#include "ReactorTask.h"

#if ! defined (__ACE_INLINE__)
#include "Task_Manager.i"
#endif /* __ACE_INLINE__ */

ACE_Task_Manager::ACE_Task_Manager()
{
  for (int x=0; x < ACE_Scheduler_MAX_PRIORITIES; x++)
    {
      reactorTasks[x] = 0;
    }
}

void ACE_Task_Manager::initialize()
{
  for (int x=0; x < ACE_Scheduler_MAX_PRIORITIES; x++)
    {
      // Convert ACE_Scheduler_Rate (it's really a period, not a rate!)
      // to a form we can easily work with.
      ACE_Time_Value period_tv;
      ORBSVCS_Time::TimeT_to_Time_Value (period_tv, ACE_Scheduler_Rates[x]);

      RtecScheduler::Period period = period_tv.sec () * 10000000 +
                                     period_tv.usec () * 10;

      reactorTasks[x] = new ReactorTask;

      if (reactorTasks[x] == 0 ||
          reactorTasks[x]->open_reactor (period) == -1)
        {
          ACE_ERROR ((LM_ERROR, "%p.\n", "ACE_ORB::initialize_reactors"));
          return;
        }
    }
}

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)
template class ACE_Singleton<ACE_Task_Manager,ACE_SYNCH_MUTEX>;
#elif defined(ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)
#pragma instantiate ACE_Singleton<ACE_Task_Manager,ACE_SYNCH_MUTEX>
#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */
