//$Id$
#include "../Scheduler.h"
#include "tao/RTScheduling/RTScheduler_Manager.h"
#include "testS.h"

class test_impl :public POA_test
{
public:

	test_impl (RTScheduling::Current_ptr current)
		: current_ (RTScheduling::Current::_duplicate (current))
	{
		
	}

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
    
    
	RTScheduling::DistributableThread_var DT = this->current_->lookup (*(this->current_->id ())
								    ACE_ENV_ARG_PARAMETER);
	ACE_CHECK;
	
	  DT->cancel (ACE_ENV_ARG_PARAMETER);
    
    return CORBA::string_dup (message);
  }
private:
	RTScheduling::Current_var current_;
};

int
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
      ACE_TRY_CHECK;

      PortableServer::POA_var root_poa =
	PortableServer::POA::_narrow (object.in ()
				      ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;
  
      PortableServer::POAManager_var poa_manager =
	root_poa->the_POAManager (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;

      poa_manager->activate (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;
  
	  CORBA::Object_ptr current_obj = orb->resolve_initial_references ("RTScheduler_Current");
    
    RTScheduling::Current_var current = RTScheduling::Current::_narrow (current_obj
									  ACE_ENV_ARG_PARAMETER);
	ACE_TRY_CHECK;

      test_impl* test_i;
      ACE_NEW_RETURN (test_i,
		      test_impl (current.in ()),
		      -1);

      PortableServer::ObjectId_var id;
  
      id = root_poa->activate_object (test_i
				      ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;
  
      CORBA::Object_var server =
	root_poa->id_to_reference (id.in ()
				   ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;
  
      CORBA::String_var ior =
	 orb->object_to_string (server.in ()
				ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

  
  
      ACE_DEBUG ((LM_DEBUG,
		  "IOR = %s\n",
		  ior.in ()));

	  CORBA::Object_ptr manager_obj = orb->resolve_initial_references ("RTSchedulerManager"
								       ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      TAO_RTScheduler_Manager_var manager = TAO_RTScheduler_Manager::_narrow (manager_obj
									      ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      TAO_Scheduler scheduler (orb);

      manager->rtscheduler (&scheduler);


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
