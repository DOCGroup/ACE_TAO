//$Id$
#include "testC.h"

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
  
      server->one_way (ior);
  
      ACE_DEBUG ((LM_DEBUG,
		  "IOR = %s\n",
		  server->two_way (ior)));
  
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



