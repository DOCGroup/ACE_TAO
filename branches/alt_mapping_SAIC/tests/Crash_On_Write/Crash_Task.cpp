//
// $Id$
//

#include "Crash_Task.h"
#include "ace/OS_NS_unistd.h"

Crash_Task::Crash_Task (ACE_Thread_Manager *thr_mgr,
                        const ACE_Time_Value &running_time)
  : ACE_Task_Base (thr_mgr)
  , running_time_ (running_time)
{
}

int
Crash_Task::svc (void)
{
  ACE_DEBUG ((LM_DEBUG, "(%P|%t) Starting crash task\n"));
  ACE_OS::sleep (this->running_time_);
  ACE_DEBUG ((LM_DEBUG, "(%P|%t) Sleep done, crashing the server\n"));
  ACE_DEBUG ((LM_DEBUG, "Aborting\n"));
  ACE::terminate_process (ACE_OS::getpid ());

  return 0;
}
