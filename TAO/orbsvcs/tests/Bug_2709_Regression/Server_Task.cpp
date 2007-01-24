// $Id$

#include "ace/OS_NS_unistd.h"
#include "TestImpl.h"
#include "Server_Task.h"

//////////////////////////////////////////////////////////////////////////

Server_Task::Server_Task (const char *output,
  CORBA::ORB_ptr sorb,
  ACE_Manual_Event &me,
  ACE_Thread_Manager *thr_mgr) :
  ACE_Task_Base(thr_mgr),
  output_(output),
  me_(me),
  sorb_(sorb)
{
}

int
Server_Task::svc (void)
{
  ACE_TRY_NEW_ENV
    {
      CORBA::Object_var poa_object =
        sorb_->resolve_initial_references ("RootPOA" ACE_ENV_ARG_PARAMETER);

      if (CORBA::is_nil (poa_object.in ()))
        ACE_ERROR_RETURN ((LM_ERROR, " (%P|%t) Unable to initialize the POA.\n"), 1);

      PortableServer::POA_var root_poa =
        PortableServer::POA::_narrow (poa_object.in () ACE_ENV_ARG_PARAMETER);

      PortableServer::POAManager_var poa_manager =
        root_poa->the_POAManager ();

      poa_manager->activate ();

      TestImpl * server_impl = 0;
      ACE_NEW_RETURN (server_impl, TestImpl (sorb_.in ()), 1);

      PortableServer::ObjectId_var id =
        root_poa->activate_object (server_impl ACE_ENV_ARG_PARAMETER);

      CORBA::Object_var test_obj =
        root_poa->id_to_reference (id.in () ACE_ENV_ARG_PARAMETER);

      Test_var server = Test::_narrow (test_obj.in () ACE_ENV_ARG_PARAMETER);

      CORBA::String_var ior =
        sorb_->object_to_string (test_obj.in () ACE_ENV_ARG_PARAMETER);

      // Output the IOR to the <ior_output_file>
      FILE *output_file= ACE_OS::fopen (output_, "w");
      if (output_file != 0)
        {
          ACE_OS::fprintf (output_file, "%s", ior.in ());
          ACE_OS::fclose (output_file);

          ACE_DEBUG ((LM_ERROR, "(%P): Server's IOR was written to file: %s.\n", output_) );
        }

      // Signal the main thread before we call orb->run ();
      this->me_.signal ();

      this->sorb_->run ();

      ACE_DEBUG ((LM_DEBUG, "event loop finished\n"));

      root_poa->destroy (1, 1 ACE_ENV_ARG_PARAMETER);
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION,
                          "Exception caught in server task:");
      return 1;
    }
  ACE_ENDTRY;

  return 0;
}
