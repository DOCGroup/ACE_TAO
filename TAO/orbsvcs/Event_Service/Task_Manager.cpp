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
      RtecScheduler::Period tv = ACE_Scheduler_Rates[x];
      reactorTasks[x] = new ReactorTask;
      if (reactorTasks[x] == 0 ||
	  reactorTasks[x]->open_reactor (tv) == -1)
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
