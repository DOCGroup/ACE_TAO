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
      // @@ Apparently Period is measured in a different unit that the
      // time, beats me.
      ACE_hrtime_t nanosecs;
      ORBSVCS_Time::TimeT_to_hrtime (nanosecs, ACE_Scheduler_Rates[x]);

      RtecScheduler::Period period = nanosecs;
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
