// $Id$

#include "worker.h"
#include "Object_Factory_i.h"
#include "ace/Get_Opt.h"
#include "ace/streams.h"
#include "ace/Task.h"

ACE_RCSID(Test, server, "$Id$")

int msglen = 100; //default length of reply message is 100 bytes
int nthreads = 2;
const char *ior_output_file = "test.ior";
int timeout = 30; //default timeout for ORB is 30 sec


int
parse_args (int argc, char *argv[])
{
  ACE_Get_Opt get_opts (argc, argv, "o:n:l:t:");
  int c;

  while ((c = get_opts ()) != -1)
    switch (c)
      {
      case 'o':
        ior_output_file = get_opts.opt_arg ();
        break;

      case 'l':
        msglen = ACE_OS::atoi( get_opts.opt_arg ());
        break;

      case 'n':
        nthreads = ACE_OS::atoi (get_opts.opt_arg ());
        break;

      case 't':
        timeout = ACE_OS::atoi (get_opts.opt_arg ());
        break;

      case '?':
      default:
        ACE_ERROR_RETURN ((LM_ERROR,
                           "usage:  %s "
                           "-o <iorfile>"
                           " -n <#of  threads>"
                           " -l <size of message in bytes>"
                           " -t <timeout for ORB in secs>"
                           "\n",
                           argv [0]),
                          -1);
      }
  // Indicates sucessful parsing of the command line
  return 0;
}

int
main (int argc, char *argv[])
{
  ACE_TRY_NEW_ENV
    {
      // Initialize the ORB
      CORBA::ORB_var orb =
        CORBA::ORB_init (argc, argv, "" TAO_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      // Get initial reference to the Root POA
      CORBA::Object_var poa_object =
        orb->resolve_initial_references("RootPOA" TAO_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      // Narrow down to the appropriate type
      PortableServer::POA_var root_poa =
        PortableServer::POA::_narrow (poa_object.in () TAO_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      if (CORBA::is_nil (root_poa.in ()))
        ACE_ERROR_RETURN ((LM_ERROR,
                           " (%P|%t) Panic: nil RootPOA\n"),
                          1);

      // Get referencee to the POA manager
      PortableServer::POAManager_var poa_manager =
        root_poa->the_POAManager (TAO_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;

      // Parse the arguments
      if (parse_args (argc, argv) != 0)
        return 1;

      ACE_DEBUG(( LM_DEBUG, "ior file = %s\t#threads = %d\t"
                  "msglen = %d\tORB timeout = %d sec\n",
                  ior_output_file, nthreads, msglen, timeout ));

      // Create the factory servant
      Object_Factory_i *factory_impl;
      ACE_NEW_THROW_EX (factory_impl,
                        Object_Factory_i (orb.in (), msglen),
                        CORBA::NO_MEMORY ());

      // _this method registers the object withe the POA and returns
      // an object reference
      Two_Objects_Test::Object_Factory_var factory =
        factory_impl->_this (TAO_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;

      // Convert the object reference to a string so that it can
      // be saved in a file and used by clinet programs later
      CORBA::String_var ior =
        orb->object_to_string (factory.in () TAO_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      // If the ior_output_file exists, output the ior to it
      FILE *output_file= ACE_OS::fopen (ior_output_file, "w");

      if (output_file == 0)
        ACE_ERROR_RETURN ((LM_ERROR,
                           "Cannot open output file for writing IOR: %s",
                           ior_output_file),
                          1);

      ACE_OS::fprintf (output_file, "%s", ior.in ());
      ACE_OS::fclose (output_file);

      // Activate the POA manager
      poa_manager->activate (TAO_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;

      // Instantiate the specified # of worker threads
      Worker worker (orb.in (), timeout);

      if (worker.activate (THR_NEW_LWP | THR_JOINABLE,
                           nthreads) != 0)
        ACE_ERROR_RETURN ((LM_ERROR,
                           "Cannot activate server threads\n"),
                          1);

      // Wait for all threads to get done
      worker.thr_mgr ()->wait ();

      ACE_DEBUG ((LM_DEBUG, "(%P|%t) event loop finished\n"));

      root_poa->destroy (1, 1 TAO_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      orb->destroy (TAO_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION,
                           "Exception caught:");
      return 1;
    }
  ACE_ENDTRY;

  return 0;
}
