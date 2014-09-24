// $Id$

#include "worker.h"
#include "Object_Factory_i.h"
#include "ace/Get_Opt.h"
#include "ace/Task.h"

int msglen = 100; //default length of reply message is 100 bytes
int nthreads = 2;
const ACE_TCHAR *ior_output_file = ACE_TEXT("test.ior");

int
parse_args (int argc, ACE_TCHAR *argv[])
{
  ACE_Get_Opt get_opts (argc, argv, ACE_TEXT("o:n:l:t:"));
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

      case '?':
      default:
        ACE_ERROR_RETURN ((LM_ERROR,
                           "usage:  %s "
                           "-o <iorfile>"
                           " -n <#of  threads>"
                           " -l <size of message in bytes>"
                           "\n",
                           argv [0]),
                          -1);
      }
  // Indicates successful parsing of the command line
  return 0;
}

int
ACE_TMAIN(int argc, ACE_TCHAR *argv[])
{
  try
    {
      // Initialize the ORB
      CORBA::ORB_var orb =
        CORBA::ORB_init (argc, argv);

      // Get initial reference to the Root POA
      CORBA::Object_var poa_object =
        orb->resolve_initial_references("RootPOA");

      // Narrow down to the appropriate type
      PortableServer::POA_var root_poa =
        PortableServer::POA::_narrow (poa_object.in ());

      if (CORBA::is_nil (root_poa.in ()))
        ACE_ERROR_RETURN ((LM_ERROR,
                           " (%P|%t) Panic: nil RootPOA\n"),
                          1);

      // Get referencee to the POA manager
      PortableServer::POAManager_var poa_manager =
        root_poa->the_POAManager ();

      // Parse the arguments
      if (parse_args (argc, argv) != 0)
        return 1;

      ACE_DEBUG(( LM_DEBUG, "ior file = %s\t#threads = %d\t"
                  "msglen = %d\n",
                  ior_output_file, nthreads, msglen));

      // Create the factory servant
      Object_Factory_i *factory_impl = 0;
      ACE_NEW_THROW_EX (factory_impl,
                        Object_Factory_i (orb.in (), msglen),
                        CORBA::NO_MEMORY ());
      PortableServer::ServantBase_var safe (factory_impl);

      // _this method registers the object withe the POA and returns
      // an object reference
      PortableServer::ObjectId_var id =
        root_poa->activate_object (factory_impl);

      CORBA::Object_var object_act = root_poa->id_to_reference (id.in ());

      Two_Objects_Test::Object_Factory_var factory =
        Two_Objects_Test::Object_Factory::_narrow (object_act.in ());

      // Convert the object reference to a string so that it can
      // be saved in a file and used by clinet programs later
      CORBA::String_var ior =
        orb->object_to_string (factory.in ());

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
      poa_manager->activate ();

      // Instantiate the specified # of worker threads
      Worker worker (orb.in ());

      if (worker.activate (THR_NEW_LWP | THR_JOINABLE,
                           nthreads) != 0)
        ACE_ERROR_RETURN ((LM_ERROR,
                           "Cannot activate server threads\n"),
                          1);

      // Wait for all threads to get done
      worker.thr_mgr ()->wait ();

      ACE_DEBUG ((LM_DEBUG, "(%P|%t) event loop finished\n"));

      root_poa->destroy (1, 1);

      orb->destroy ();
    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception ("Exception caught:");
      return 1;
    }

  return 0;
}
