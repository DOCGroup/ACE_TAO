// $Id$

#include "test_i.h"
#include "ace/Get_Opt.h"
#include "ace/Sched_Params.h"

ACE_RCSID(Reliable, server, "$Id$")

const char *ior_output_file = "test.ior";

int
main (int argc, char *argv[])
{
  int policy = ACE_SCHED_FIFO;
  int flags  = THR_SCHED_FIFO|THR_NEW_LWP|THR_JOINABLE;
  int priority =
    (ACE_Sched_Params::priority_min (policy)
     + ACE_Sched_Params::priority_max (policy)) / 2;

  // Enable FIFO scheduling, e.g., RT scheduling class on Solaris.
  int result =
    ACE_OS::sched_params (ACE_Sched_Params (policy,
                                            priority,
                                            ACE_SCOPE_PROCESS));

  if (result != 0)
    {
      if (ACE_OS::last_error () == EPERM)
        {
          ACE_DEBUG ((LM_DEBUG,
                      "server (%P|%t): user is not superuser, "
                      "test runs in time-shared class\n"));
          policy = ACE_SCHED_OTHER;
          flags = THR_NEW_LWP|THR_JOINABLE;
        }
      else
        ACE_ERROR_RETURN ((LM_ERROR,
                           "server (%P|%t): sched_params failed\n"),
                          1);
    }

  ACE_hthread_t self;
  ACE_OS::thr_self (self);

  if (ACE_OS::thr_getprio (self, priority) == 0)
    ACE_DEBUG ((LM_DEBUG, 
                "server (%P|%t): thread priority = %d.\n", 
                priority));

  ACE_TRY_NEW_ENV
    {
      CORBA::ORB_var orb =
        CORBA::ORB_init (argc, 
                         argv, 
                         "", 
                         ACE_TRY_ENV);
      ACE_TRY_CHECK;

      CORBA::Object_var poa_object =
        orb->resolve_initial_references("RootPOA");

      if (CORBA::is_nil (poa_object.in ()))
        ACE_ERROR_RETURN ((LM_ERROR,
                           " (%P|%t) Unable to initialize the POA.\n"),
                          1);

      PortableServer::POA_var root_poa =
        PortableServer::POA::_narrow (poa_object.in (), 
                                      ACE_TRY_ENV);
      ACE_TRY_CHECK;

      PortableServer::POAManager_var poa_manager =
        root_poa->the_POAManager (ACE_TRY_ENV);
      ACE_TRY_CHECK;

      Test_i server_impl (orb.in ());

      Test_var server =
        server_impl._this (ACE_TRY_ENV);
      ACE_TRY_CHECK;

      CORBA::String_var ior =
	      orb->object_to_string (server.in (), 
                               ACE_TRY_ENV);
      ACE_TRY_CHECK;

      ACE_DEBUG ((LM_DEBUG, 
                  "Activated as <%s>\n", 
                  ior.in ()));

	    FILE *output_file= ACE_OS::fopen (ior_output_file, "w");

	    if (output_file == 0)
	      ACE_ERROR_RETURN ((LM_ERROR,
			                     "Cannot open output file for writing IOR: %s",
			                     ior_output_file),
			                    1);

	    ACE_OS::fprintf (output_file, 
                       "%s", 
                       ior.in ());
	    ACE_OS::fclose (output_file);

      poa_manager->activate (ACE_TRY_ENV);
      ACE_TRY_CHECK;

      if (orb->run (ACE_TRY_ENV) == -1)
        ACE_ERROR_RETURN ((LM_ERROR, 
                           "%p\n", 
                           "orb->run"), 
                          -1);
      ACE_TRY_CHECK;

      ACE_DEBUG ((LM_DEBUG, "event loop finished\n"));

      root_poa->destroy (1,
                         1,
                         ACE_TRY_ENV);
      ACE_TRY_CHECK;
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION,
                           "server");
      return 1;
    }
  ACE_ENDTRY;

  return 0;
}

