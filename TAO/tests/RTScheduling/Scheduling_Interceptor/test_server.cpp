//$Id$
#include "testS.h"

class test_impl :public POA_test
{
public:
  virtual void one_way (const char * message)
    ACE_THROW_SPEC ((CORBA::SystemException))
  {
    ACE_DEBUG ((LM_DEBUG,
		"Message = %s\n",
		message));
  }
  
  virtual char * two_way (const char * message)
    ACE_THROW_SPEC ((CORBA::SystemException))
  {
    return (char*) message;
  }
};

main (int argc, char* argv[])
{
  ACE_TRY
    {
      
      CORBA::ORB_var orb =
	CORBA::ORB_init (argc,
			 argv,
			 ""
			 ACE_ENV_ARG_PARAMETER);
      ACE_CHECK_RETURN (-1);

      CORBA::Object_var object =
	orb->resolve_initial_references ("RootPOA"
					 ACE_ENV_ARG_PARAMETER);
      ACE_CHECK_RETURN (-1);

      PortableServer::POA_var root_poa =
	PortableServer::POA::_narrow (object.in ()
				      ACE_ENV_ARG_PARAMETER);
      ACE_CHECK_RETURN (-1);
  
      PortableServer::POAManager_var poa_manager =
	root_poa->the_POAManager (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_CHECK_RETURN (-1);

      poa_manager->activate (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_CHECK_RETURN (-1);
  
      test_impl* test_i;
      ACE_NEW_RETURN (test_i,
		      test_impl,
		      -1);

      PortableServer::ObjectId_var id;
  
      id = root_poa->activate_object (test_i
				      ACE_ENV_ARG_PARAMETER);
      ACE_CHECK;
  
      CORBA::Object_var server =
	root_poa->id_to_reference (id.in ()
				   ACE_ENV_ARG_PARAMETER);
      ACE_CHECK;
  
      CORBA::String_var ior =
	 orb->object_to_string (server.in ()
				ACE_ENV_ARG_PARAMETER);
      ACE_CHECK;

  
  
      ACE_DEBUG ((LM_DEBUG,
		  "IOR = %s\n",
		  ior.in ()));

      const char* filename = "test.ior";
      // Print ior to the file.
      if (filename != 0)
	{
	  FILE *output_file= ACE_OS::fopen (filename, "w");
	  if (output_file == 0)
	    ACE_ERROR_RETURN ((LM_ERROR,
			       "Cannot open output file for writing IOR: %s",
			       filename),
			      -1);
	  ACE_OS::fprintf (output_file, "%s", ior.in ());
	  ACE_OS::fclose (output_file);
	}

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
