#include "Oneway_Receiver.h"
#include "Crash_Task.h"
#include "ace/Get_Opt.h"
#include <memory>

struct Options
{
  ACE_TCHAR const *ior_output_file {ACE_TEXT ("test.ior")};
};

int
parse_args (int argc, ACE_TCHAR *argv[], Options &options)
{
  ACE_Get_Opt get_opts (argc, argv, ACE_TEXT ("o:"));
  int c {};

  while ((c = get_opts ()) != -1)
    switch (c)
      {
      case 'o':
        options.ior_output_file = get_opts.opt_arg ();
        break;

      case '?':
      default:
        ACE_ERROR_RETURN ((LM_ERROR,
                           "usage:  %s "
                           "-o <iorfile>"
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
      CORBA::ORB_var const orb =
        CORBA::ORB_init (argc, argv);

      CORBA::Object_var const poa_object =
        orb->resolve_initial_references ("RootPOA");

      PortableServer::POA_var const root_poa =
        PortableServer::POA::_narrow (poa_object.in ());

      if (CORBA::is_nil (root_poa.in ()))
        ACE_ERROR_RETURN ((LM_ERROR,
                           " (%P|%t) Panic: nil RootPOA\n"),
                          1);

      PortableServer::POAManager_var const poa_manager =
        root_poa->the_POAManager ();

      Options options {};
      if (parse_args (argc, argv, options) != 0)
        return 1;

      Oneway_Receiver *oneway_receiver_impl {};
      ACE_NEW_RETURN (oneway_receiver_impl,
                      Oneway_Receiver,
                      1);
      // The servant var takes ownership of the servant's initial reference.
      PortableServer::ServantBase_var owner_transfer (oneway_receiver_impl);

      PortableServer::ObjectId_var const id =
        root_poa->activate_object (oneway_receiver_impl);

      CORBA::Object_var const object = root_poa->id_to_reference (id.in ());

      Test::Oneway_Receiver_var const oneway_receiver =
        Test::Oneway_Receiver::_narrow (object.in ());

      CORBA::String_var const ior =
        orb->object_to_string (oneway_receiver.in ());

      // If the ior_output_file exists, output the ior to it
      {
        std::unique_ptr<FILE, int (*) (FILE *)> output_file (
          ACE_OS::fopen (options.ior_output_file, "w"), ACE_OS::fclose);
        if (!output_file)
          ACE_ERROR_RETURN ((LM_ERROR,
                             "Cannot open output file for writing IOR: %s",
                             options.ior_output_file),
                            1);
        ACE_OS::fprintf (output_file.get (), "%s", ior.in ());
      }

      poa_manager->activate ();

      // Create a separate thread to crash the server.
      Crash_Task crash_task (ACE_Thread_Manager::instance (), ACE_Time_Value (10));

      if (crash_task.activate () == -1)
        {
          ACE_ERROR ((LM_ERROR, "Error activating crash task\n"));
          return 1;
        }

      orb->run ();

      /*NOTREACHED*/
      ACE_DEBUG ((LM_DEBUG, "(%P|%t) server - event loop finished\n"));

      root_poa->destroy (true, true);

      orb->destroy ();
    }
  catch (CORBA::Exception const & ex)
    {
      ex._tao_print_exception ("Exception caught:");
      return 1;
    }

  return 0;
}
