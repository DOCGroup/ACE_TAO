//$Id$
#include "tao/RTScheduling/Scheduler.h"
#include "tao/RTScheduling/RTScheduler_Manager.h"
#include "testC.h"

int
main (int argc, char* argv [])
{
  ACE_TRY
    {
      
      CORBA::ORB_var orb = CORBA::ORB_init (argc,
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

      TAO_Scheduler scheduler;

      manager->rtscheduler (&scheduler);
      
      const char *ior = "file://test.ior";
      
      CORBA::Object_var object = 
	orb->string_to_object (ior
			       ACE_ENV_ARG_PARAMETER)
	ACE_CHECK;
  
      test_var server = test::_narrow (object.in ()
				       ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      if (CORBA::is_nil (server.in ()))
	{
	  ACE_ERROR_RETURN ((LM_ERROR,
			     "ERROR: Object reference <%s> is nil\n",
			     ior),
			    1);
	}
  
	  CORBA::Object_ptr current_obj = orb->resolve_initial_references ("RTScheduler_Current");
  
	  RTScheduling::Current_var current = RTScheduling::Current::_narrow (current_obj
						   ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  const char * name = 0;
  CORBA::Policy_ptr sched_param = 0;
  CORBA::Policy_ptr implicit_sched_param = 0;
  
  current->begin_scheduling_segment (name,
				     sched_param,
				     implicit_sched_param
				     ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;
  
  server->one_way (ior);
  
  ACE_DEBUG ((LM_DEBUG,
	      "IOR = %s\n",
	      server->two_way (ior)));

  current->end_scheduling_segment (name
				   ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;
  
  orb->run ();

    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION,
                           "Caught exception:");
      
    }
  ACE_ENDTRY; 

  return 0;
}







