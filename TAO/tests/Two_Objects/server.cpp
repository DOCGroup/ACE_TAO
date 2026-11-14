#include "worker.h"
#include "Object_Factory_i.h"
#include "ace/Get_Opt.h"
#include "ace/Task.h"
#include <memory>

struct Server_Options
{
  ACE_TString ior_output_file {ACE_TEXT ("test.ior")};
  int message_length {100};
  int thread_count {2};
};

static int
parse_args (int argc, ACE_TCHAR *argv[], Server_Options &options)
{
  ACE_Get_Opt get_opts (argc, argv, ACE_TEXT ("o:n:l:t:"));
  int option {};

  while ((option = get_opts ()) != -1)
    switch (option)
      {
      case 'o':
        options.ior_output_file = get_opts.opt_arg ();
        break;

      case 'l':
        options.message_length = ACE_OS::atoi (get_opts.opt_arg ());
        break;

      case 'n':
        options.thread_count = ACE_OS::atoi (get_opts.opt_arg ());
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
ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
  try
    {
      // Initialize the ORB
      CORBA::ORB_var const orb =
        CORBA::ORB_init (argc, argv);

      // Get initial reference to the Root POA
      CORBA::Object_var const poa_object =
        orb->resolve_initial_references ("RootPOA");

      // Narrow down to the appropriate type
      PortableServer::POA_var const root_poa =
        PortableServer::POA::_narrow (poa_object.in ());

      if (CORBA::is_nil (root_poa.in ()))
        ACE_ERROR_RETURN ((LM_ERROR,
                           " (%P|%t) Panic: nil RootPOA\n"),
                          1);

      // Get referencee to the POA manager
      PortableServer::POAManager_var const poa_manager =
        root_poa->the_POAManager ();

      // Parse the arguments
      Server_Options options;
      if (parse_args (argc, argv, options) != 0)
        return 1;

      ACE_DEBUG ((LM_DEBUG,
                  ACE_TEXT ("ior file = %s\t#threads = %d\tmsglen = %d\n"),
                  options.ior_output_file.c_str (),
                  options.thread_count,
                  options.message_length));

      // Create the factory servant
      Object_Factory_i *factory_impl {};
      ACE_NEW_THROW_EX (factory_impl,
                        Object_Factory_i (orb.in (), options.message_length),
                        CORBA::NO_MEMORY ());
      PortableServer::ServantBase_var safe (factory_impl);

      // _this method registers the object withe the POA and returns
      // an object reference
      PortableServer::ObjectId_var const id =
        root_poa->activate_object (factory_impl);

      CORBA::Object_var const object_act = root_poa->id_to_reference (id.in ());

      Two_Objects_Test::Object_Factory_var const factory =
        Two_Objects_Test::Object_Factory::_narrow (object_act.in ());

      // Convert the object reference to a string so that it can
      // be saved in a file and used by clinet programs later
      CORBA::String_var const ior =
        orb->object_to_string (factory.in ());

      {
        std::unique_ptr<FILE, int (*) (FILE *)> const output_file (
          ACE_OS::fopen (options.ior_output_file.c_str (), ACE_TEXT ("w")),
          ACE_OS::fclose);
        if (!output_file)
          ACE_ERROR_RETURN ((LM_ERROR,
                             ACE_TEXT ("Cannot open output file for writing IOR: %s\n"),
                             options.ior_output_file.c_str ()),
                            1);

        ACE_OS::fprintf (output_file.get (), "%s", ior.in ());
      }

      // Activate the POA manager
      poa_manager->activate ();

      // Instantiate the specified # of worker threads
      Worker worker (orb.in ());

      if (worker.activate (THR_NEW_LWP | THR_JOINABLE,
                           options.thread_count) != 0)
        ACE_ERROR_RETURN ((LM_ERROR,
                           "Cannot activate server threads\n"),
                          1);

      // Wait for all threads to get done
      worker.thr_mgr ()->wait ();

      ACE_DEBUG ((LM_DEBUG, "(%P|%t) event loop finished\n"));

      root_poa->destroy (true, true);

      orb->destroy ();
    }
  catch (CORBA::Exception const &ex)
    {
      ex._tao_print_exception ("Exception caught:");
      return 1;
    }

  return 0;
}
