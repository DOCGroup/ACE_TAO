//$Id$
#include "../Scheduler.h"
#include "tao/RTScheduling/RTScheduler_Manager.h"
#include "tao/ORB.h"
#include "Thread_Action.h"
#include "ace/Thread_Manager.h"
#include "ace/SString.h"

int
main (int argc, char* argv [])
{
  CORBA::ORB_var orb;
  RTScheduling::Current_var current;
		    
  const char * name = 0;
  CORBA::Policy_ptr sched_param = 0;
  CORBA::Policy_ptr implicit_sched_param = 0;

  Test_Thread_Action thread_action;

  ACE_TRY_NEW_ENV
    {
      orb = CORBA::ORB_init (argc,
			     argv,
			     ""
			     ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      CORBA::Object_var manager_obj = orb->resolve_initial_references ("RTSchedulerManager"
								       ACE_ENV_ARG_PARAMETER);
      ACE_CHECK_RETURN (-1);

      TAO_RTScheduler_Manager_var manager = TAO_RTScheduler_Manager::_narrow (manager_obj.in ()
									      ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      TAO_Scheduler* scheduler;
      ACE_NEW_RETURN (scheduler,
		      TAO_Scheduler (orb.in ()),
		      -1);
      RTScheduling::Scheduler_var safe_scheduler = scheduler;

      manager->rtscheduler (scheduler);

      CORBA::Object_var current_obj = orb->resolve_initial_references ("RTScheduler_Current"
								       ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;
      
      current = RTScheduling::Current::_narrow (current_obj.in ()
						ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;
      
      ACE_TRY_EX (block1)
	{

	  ACE_DEBUG ((LM_DEBUG,
		      "Invoking DT spawn without calling begin_scheduling_segment...\n"));

	  ACE_CString data ("Harry Potter");
	  current->spawn (&thread_action,
			  const_cast<char *> (data.c_str ()),
			  name,
			  sched_param,
			  implicit_sched_param,
			  0,
			  0
			  ACE_ENV_ARG_PARAMETER);
	  ACE_TRY_CHECK_EX (block1);
	}
      ACE_CATCH (CORBA::BAD_INV_ORDER, thr_ex)
	{
	  ACE_DEBUG ((LM_DEBUG,
		      "Spawn should be in the context of a Scheduling Segment - Expected Exception\n"));
	}
      ACE_CATCHANY
	{
	  ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION,
			       "\n");
	}
      ACE_ENDTRY;
      
      ACE_DEBUG ((LM_DEBUG,
		  "Start - Scheduling Segment...\n"));

      current->begin_scheduling_segment ("Potter",
					 sched_param,
					 implicit_sched_param
					 ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;
      
      size_t count = 0;
      ACE_OS::memcpy (&count,
		      current->id ()->get_buffer (),
		      current->id ()->length ());
      
      ACE_DEBUG ((LM_DEBUG,
		  "The Current DT Guid is %d\n",
		  count));
      
      //Initialize data to be passed to the Thread_Action::do method
      Data spawn_data;
      spawn_data.data = "Harry Potter";
      spawn_data.current = RTScheduling::Current::_duplicate (current.in ());
      
      ACE_DEBUG ((LM_DEBUG,
		  "Spawning a new DT...\n"));
      RTScheduling::DistributableThread_var dt =
      current->spawn (&thread_action,
		      &spawn_data,
		      "Chamber of Secrets",
		      sched_param,
		      implicit_sched_param,
		      0,
		      0
		      ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;
      
      current->end_scheduling_segment (name
				       ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;
      ACE_DEBUG ((LM_DEBUG,
		  "End - Scheduling Segment %d\n",
		  count));
     
    } 
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION,
			   "Caught Exception\n");
    }
  ACE_ENDTRY; 
  
  ACE_Thread_Manager::instance ()->wait ();
  
  return 0;
}

