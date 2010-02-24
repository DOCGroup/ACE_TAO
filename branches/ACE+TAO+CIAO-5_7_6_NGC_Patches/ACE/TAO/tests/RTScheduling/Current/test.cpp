// $Id$

#include "Thread_Task.h"
#include "../Scheduler.h"
#include "tao/RTScheduling/RTScheduler_Manager.h"

int
ACE_TMAIN(int argc, ACE_TCHAR *argv[])
{
  try
    {
      CORBA::ORB_var orb =
        CORBA::ORB_init (argc,
                         argv);

      CORBA::Object_var manager_obj = orb->resolve_initial_references ("RTSchedulerManager");

      TAO_RTScheduler_Manager_var manager = TAO_RTScheduler_Manager::_narrow (manager_obj.in ());

      TAO_Scheduler scheduler (orb.in ());

      manager->rtscheduler (&scheduler);

      Thread_Task task (orb.in ());

      task.activate_task (4);

      orb->run ();

      // Wait until all threads are ready.
      ACE_Thread_Manager::instance ()->wait ();

      orb->destroy ();
    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception ("Caught exception:");
      return 1;
    }

  return 0;
}
