#include "ace/Get_Opt.h"
#include "Sender_i.h"
#include "tao/BiDir_GIOP/BiDirGIOP.h"
#include "tao/AnyTypeCode/Any.h"
#include "Server_Task.h"
#include "tao/ORB_Core.h"
#include "tao/Thread_Lane_Resources.h"
#include "tao/Transport_Cache_Manager.h"
#include "ace/Manual_Event.h"
#include "ace/SString.h"
#include <memory>

struct Server_Options
{
  ACE_TString ior_output_file;
  int iterations {10};
  int client_count {2};
};

int
parse_args (int argc, ACE_TCHAR *argv[], Server_Options &options)
{
  ACE_Get_Opt get_opts (argc, argv, ACE_TEXT("o:i:c:"));
  int c {};

  while ((c = get_opts ()) != -1)
    switch (c)
      {
      case 'o':
        options.ior_output_file = get_opts.opt_arg ();
        break;
      case 'c':
        options.client_count = ACE_OS::atoi (get_opts.opt_arg ());
        break;
      case 'i':
        options.iterations = ACE_OS::atoi (get_opts.opt_arg ());
        break;
      case '?':
      default:
        ACE_ERROR_RETURN ((LM_ERROR,
                           ACE_TEXT ("usage: %s -o <iorfile> -c <no_clients> -i <no_iterations>\n"),
                           argv [0]),
                          -1);
      }
  // Indicates successful parsing of the command line
  return 0;
}

int
ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
  Server_Options options;
  try
    {
      CORBA::ORB_var orb =
        CORBA::ORB_init (argc, argv);

      if (parse_args (argc, argv, options) != 0)
        return 1;

      if (options.client_count <= 0 || options.iterations <= 0)
        ACE_ERROR_RETURN ((LM_ERROR, ACE_TEXT ("Client count and iterations must be positive.\n")), 1);

      CORBA::Object_var poa_object =
        orb->resolve_initial_references ("RootPOA");

      if (CORBA::is_nil (poa_object.in ()))
        ACE_ERROR_RETURN ((LM_ERROR,
                           " (%P|%t) Unable to initialize the POA.\n"),
                          1);

      PortableServer::POA_var root_poa =
        PortableServer::POA::_narrow (poa_object.in ());

      PortableServer::POAManager_var poa_manager =
        root_poa->the_POAManager ();

      // Policies for the childPOA to be created.
      CORBA::PolicyList policies (1);
      policies.length (1);

      CORBA::Any pol;
      pol <<= BiDirPolicy::BOTH;
      policies[0] =
        orb->create_policy (BiDirPolicy::BIDIRECTIONAL_POLICY_TYPE,
                            pol);

      // Create POA as child of RootPOA with the above policies.  This POA
      // will receive request in the same connection in which it sent
      // the request
      PortableServer::POA_var child_poa =
        root_poa->create_POA ("childPOA",
                              poa_manager.in (),
                              policies);

      // Creation of childPOA is over. Destroy the Policy objects.
      for (CORBA::ULong i = 0;
           i < policies.length ();
           ++i)
        {
          policies[i]->destroy ();
        }

      poa_manager->activate ();

      ACE_Manual_Event manual_event;

      // The ServantBase_var below takes ownership of the raw servant pointer.
      Sender_i *sender {};
      ACE_NEW_RETURN (sender,
                      Sender_i (static_cast<CORBA::ULong> (options.client_count),
                                manual_event),
                      -1);

      PortableServer::ServantBase_var owner_transfer (sender);

      PortableServer::ObjectId_var id =
        child_poa->activate_object (sender);

      CORBA::Object_var obj =
        child_poa->id_to_reference (id.in ());

      CORBA::String_var ior =
        orb->object_to_string (obj.in ());

      ACE_DEBUG ((LM_DEBUG, "Activated as <%C>\n", ior.in ()));

      // If the ior_output_file exists, output the ior to it
      if (!options.ior_output_file.empty ())
        {
          std::unique_ptr<FILE, int (*) (FILE *)> output_file (
            ACE_OS::fopen (options.ior_output_file.c_str (), ACE_TEXT ("w")),
            ACE_OS::fclose);
          if (!output_file)
            ACE_ERROR_RETURN ((LM_ERROR,
                               ACE_TEXT ("Cannot open output file for writing IOR: %s"),
                               options.ior_output_file.c_str ()),
                              1);
          ACE_OS::fprintf (output_file.get (), "%s", ior.in ());
        }

      // Get the main thread id..
      ACE_thread_t thr_id = ACE_Thread::self ();

      Server_Task server_task (*sender,
                               manual_event,
                               options.iterations,
                               orb.in (),
                               ACE_Thread_Manager::instance ());

      if (server_task.activate (THR_NEW_LWP | THR_JOINABLE, 4, 1) == -1)
        {
          ACE_ERROR ((LM_ERROR, "Error activating server task\n"));
        }

      // Only the main thread gets to run the ORB
      if (thr_id == ACE_Thread::self ())
        {
          // run the ORB for at most 60 seconds...
          ACE_Time_Value tv (60, 0);

          // Call the ORB run from the main thread
          orb->run (tv);

          ACE_DEBUG ((LM_DEBUG, "(%P|%t) event loop finished\n"));
        }

      ACE_Thread_Manager::instance ()->wait ();

      root_poa->destroy (true, true);
    }
  catch (CORBA::Exception const &ex)
    {
      ex._tao_print_exception ("Caught exception:");
      return 1;
    }

  return 0;
}
