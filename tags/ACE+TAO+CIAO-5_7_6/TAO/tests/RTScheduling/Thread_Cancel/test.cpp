// $Id$

#include "Thread_Task.h"
#include "ace/OS_NS_unistd.h"
#include "../Scheduler.h"
#include "tao/RTScheduling/RTScheduler_Manager.h"
#include "tao/ORB_Core.h"

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

      Thread_Task task;

      task.activate_task (orb.in ());

      ACE_DEBUG ((LM_DEBUG,
                  "Waiting for Threads to Activate...\n"));
      ACE_OS::sleep (5);
      ACE_DEBUG ((LM_DEBUG,
                  "Threads Activated\n"));

      ACE_DEBUG ((LM_DEBUG,
                  "Cancelling Threads.....\n"));

      CORBA::Object_var current_obj = orb->resolve_initial_references ("RTScheduler_Current");

      RTScheduling::Current_var current = RTScheduling::Current::_narrow (current_obj.in ());

      for (int i = 0; i < 4; i++)
        {
          RTScheduling::DistributableThread_var DT = current->lookup ((task.guids ())[i]);

          DT->cancel ();
        }

      orb->run ();

      ACE_Thread_Manager::instance ()->wait ();
    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception ("Caught exception:");
      return 1;
    }

  return 0;
}
