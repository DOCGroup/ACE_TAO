//$Id$
#include "Thread_Task.h"
#include "../Scheduler.h"
#include "tao/RTScheduling/RTScheduler_Manager.h"


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
      
      task.activate_task (orb.in (),
			  4);

      orb->run ();

      orb->destroy ();
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



