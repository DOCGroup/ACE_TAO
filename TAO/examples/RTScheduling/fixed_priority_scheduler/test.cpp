//$Id$
#include "Thread_Task.h"
#include "FP_Scheduler.h"
#include "tao/RTScheduling/RTScheduler_Manager.h"
#include "ace/Sched_Params.h"
#include "tao/ORB_Core.h"

void
check_supported_priorities (void)
{
  // Check that we have sufficient priority range to run this
  // test, i.e., more than 1 priority level.
  int max_priority =
    ACE_Sched_Params::priority_max (ACE_SCHED_RR);
  int min_priority =
    ACE_Sched_Params::priority_min (ACE_SCHED_RR);

  if (max_priority == min_priority)
    {
      ACE_DEBUG ((LM_DEBUG,
                  "Not enough priority levels on this platform"
                  " to run the test, aborting \n"));
      ACE_OS::exit (2);
    }
  else ACE_DEBUG ((LM_DEBUG,
		   "max = %d, min = %d\n")); 
};

int
main (int argc, char* argv [])
{
  ACE_TRY
    {
      CORBA::ORB_var orb =
	CORBA::ORB_init (argc,
			 argv,
			 ""
			 ACE_ENV_ARG_PARAMETER);
      ACE_CHECK_RETURN (-1);

      //check_supported_priorities ();

      CORBA::Object_ptr manager_obj = orb->resolve_initial_references ("RTSchedulerManager"
								       ACE_ENV_ARG_PARAMETER);
      ACE_CHECK_RETURN (-1);
      
      TAO_RTScheduler_Manager_var manager = TAO_RTScheduler_Manager::_narrow (manager_obj
									      ACE_ENV_ARG_PARAMETER);
      ACE_CHECK_RETURN (-1);
      
      Fixed_Priority_Scheduler scheduler (orb.in ());
      
      manager->rtscheduler (&scheduler);
      
      CORBA::Object_var object =
	orb->resolve_initial_references ("RTScheduler_Current" 
					 ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;
      
      RTScheduling::Current_var current =
	RTScheduling::Current::_narrow (object.in () ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      
      long flags = THR_NEW_LWP | THR_JOINABLE;
      flags |= 
	orb->orb_core ()->orb_params ()->scope_policy () |
	orb->orb_core ()->orb_params ()->sched_policy ();
      

		
      int min_priority = ACE_Sched_Params::priority_min (ACE_SCHED_RR);
      int max_priority = ACE_Sched_Params::priority_max (ACE_SCHED_RR);				


      ACE_DEBUG ((LM_DEBUG, "max_priority = %d, min_priority = %d\n",
                  max_priority, min_priority));
      
      // Set the main thread to max priority...
      int priority = 99;
      
      if (ACE_OS::sched_params (ACE_Sched_Params (ACE_SCHED_RR,
						  priority,
						  ACE_SCOPE_PROCESS)) != 0)
	{
	  if (ACE_OS::last_error () == EPERM)
	    {
	      ACE_DEBUG ((LM_DEBUG,
			  "(%P|%t): user is not superuser, "
			  "test runs in time-shared class\n"));
	    }
	  else
	    ACE_ERROR_RETURN ((LM_ERROR,
			       "(%P|%t): sched_params failed\n"),-1);
	}

      ACE_Barrier barrier (3);
      
      Thread_Task task1 (current.in ());
      
      FP_Scheduling::SegmentSchedulingParameterPolicy_ptr sched_param =
	scheduler.create_segment_scheduling_parameter (20);
      
      task1.activate_task (sched_param,
			   flags,
			   &barrier);
      
      Thread_Task task2 (current.in ());
      
      sched_param =
	scheduler.create_segment_scheduling_parameter (80);
      
      task2.activate_task (sched_param,
			   flags,
			   &barrier);
      
      ACE_DEBUG ((LM_DEBUG, "Waiting for tasks to synch...\n"));
      barrier.wait ();
      ACE_DEBUG ((LM_DEBUG, "Tasks have synched...\n"));
      
      orb->run ();
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

