//$Id$
#include "Thread_Task.h"
#include "../Scheduler.h"
#include "tao/RTScheduling/RTScheduler_Manager.h"
#include "tao/ORB_Core.h"

int
main (int argc, char* argv [])
{
  ACE_TRY_NEW_ENV
    {
      CORBA::ORB_var orb =
	CORBA::ORB_init (argc,
			 argv,
			 ""
			 ACE_ENV_ARG_PARAMETER);
      ACE_CHECK_RETURN (-1);

      CORBA::Object_ptr manager_obj = orb->resolve_initial_references ("RTSchedulerManager"
								       ACE_ENV_ARG_PARAMETER);
      ACE_CHECK_RETURN (-1);
      
      TAO_RTScheduler_Manager_var manager = TAO_RTScheduler_Manager::_narrow (manager_obj
									      ACE_ENV_ARG_PARAMETER);
      ACE_CHECK_RETURN (-1);
      
      TAO_Scheduler scheduler (orb.in ());
      
      manager->rtscheduler (&scheduler);
      
      Thread_Task task;
      
      task.activate_task (orb.in ());

      ACE_DEBUG ((LM_DEBUG,
		  "Waiting for Threads to Activate...\n"));
      ACE_OS::sleep (10);
      ACE_DEBUG ((LM_DEBUG,
		  "Threads Activated\n"));


      ACE_DEBUG ((LM_DEBUG,
		  "Cancelling Threads.....\n"));

      CORBA::Object_ptr current_obj = orb->resolve_initial_references ("RTScheduler_Current");
      
      RTScheduling::Current_var current = RTScheduling::Current::_narrow (current_obj
									  ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;
            
      for (int i = 0; i < 4; i++)
	{
	  RTScheduling::DistributableThread_var DT = current->lookup ((task.guids ())[i]
								      ACE_ENV_ARG_PARAMETER);
	  ACE_TRY_CHECK;

	  DT->cancel (ACE_ENV_SINGLE_ARG_PARAMETER);
	}

      orb->run ();

      ACE_Thread_Manager::instance ()->wait ();
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION,
                           "Caught exception:");
      return 1;
    }
  ACE_ENDTRY; 
  
  return 0;
}

