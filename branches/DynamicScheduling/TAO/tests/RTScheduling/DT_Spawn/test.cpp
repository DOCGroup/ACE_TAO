//$Id$
#include "../Scheduler.h"
#include "tao/RTScheduling/RTScheduler_Manager.h"
#include "Thread_Action.h"

int
main (int argc, char* argv [])
{
  CORBA::ORB_var orb;
  RTScheduling::Current_var current;
		    
  const char * name = 0;
  CORBA::Policy_ptr sched_param = 0;
  CORBA::Policy_ptr implicit_sched_param = 0;

  Test_Thread_Action thread_action;

  TAO_Scheduler scheduler;
      
  ACE_TRY
    {
      orb = CORBA::ORB_init (argc,
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


      manager->rtscheduler (&scheduler);

      CORBA::Object_ptr current_obj = orb->resolve_initial_references ("RTScheduler_Current");
      
      current = RTScheduling::Current::_narrow (current_obj
						ACE_ENV_ARG_PARAMETER);
      ACE_CHECK;
    
      
      
           
      current->spawn (&thread_action,
		      "Harry Potter",
		      name,
		      sched_param,
		      implicit_sched_param,
		      0,
		      0
		      ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;
      

      
      
    }
  ACE_CATCHANY
    {

      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION,
                           "Caught exception:");

      //Start - Nested Scheduling Segment
      current->begin_scheduling_segment ("Potter",
					 sched_param,
					 implicit_sched_param
					 ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;
      
      current->spawn (&thread_action,
		      "Harry Potter",
		      name,
		      sched_param,
		      implicit_sched_param,
		      0,
		      0
		      ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      current->end_scheduling_segment (name
				       ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;
      //  End - Nested Scheduling Segment
      
      
    }
  ACE_ENDTRY; 

  orb->run ();
  
  return 0;
}

