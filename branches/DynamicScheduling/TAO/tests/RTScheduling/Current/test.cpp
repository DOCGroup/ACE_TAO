//$Id$
#include "Thread_Task.h"


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
      
      Thread_Task task;
      
      task.activate_task (orb.in ());
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

