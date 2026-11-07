#include "Receiver.h"
#include "Server_Task.h"
#include "ace/Get_Opt.h"
#include "ace/OS_NS_stdio.h"
#include <memory>
#include <string>

struct Options
{
  std::basic_string<ACE_TCHAR> ior_output_file {ACE_TEXT ("test.ior")};
};

int
parse_args (int argc, ACE_TCHAR *argv[], Options &options)
{
  ACE_Get_Opt get_opts (argc, argv, ACE_TEXT("o:"));
  int option {};

  while ((option = get_opts ()) != -1)
    switch (option)
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
  Options options;
  try
    {
      CORBA::ORB_var const orb =
        CORBA::ORB_init (argc, argv);

      CORBA::Object_var const poa_object =
        orb->resolve_initial_references ("RootPOA");

      if (CORBA::is_nil (poa_object.in ()))
        ACE_ERROR_RETURN ((LM_ERROR,
                           " (%P|%t) Unable to initialize the POA.\n"),
                          1);

      PortableServer::POA_var const root_poa =
        PortableServer::POA::_narrow (poa_object.in ());

      PortableServer::POAManager_var const poa_manager =
        root_poa->the_POAManager ();

      if (parse_args (argc, argv, options) != 0)
        return 1;

      std::unique_ptr<Receiver> servant = std::make_unique<Receiver> (orb.in ());
      Receiver *const receiver_impl = servant.get ();
      // ServantBase_var takes ownership of the servant reference.
      PortableServer::ServantBase_var const receiver_owner_transfer (servant.release ());

      PortableServer::ObjectId_var const id =
        root_poa->activate_object (receiver_impl);

      CORBA::Object_var const object = root_poa->id_to_reference (id.in ());

      Test::Receiver_var const receiver =
        Test::Receiver::_narrow (object.in ());

      CORBA::String_var const ior =
        orb->object_to_string (receiver.in ());

      // If the ior_output_file exists, output the ior to it
      {
        std::unique_ptr<FILE, int (*) (FILE *)> const output_file (
          ACE_OS::fopen (options.ior_output_file.c_str (), "w"), ACE_OS::fclose);
        if (!output_file)
          ACE_ERROR_RETURN ((LM_ERROR,
                             "Cannot open output file for writing IOR: %s",
                             options.ior_output_file.c_str ()),
                            1);
        ACE_OS::fprintf (output_file.get (), "%s", ior.in ());
      }

      poa_manager->activate ();

      Server_Task server_task (orb.in (),
                               ACE_Thread_Manager::instance ());

      if (server_task.activate (THR_NEW_LWP | THR_JOINABLE, 4, 1) == -1)
        {
          ACE_ERROR ((LM_ERROR, "Error activating server task\n"));
        }
      ACE_Thread_Manager::instance ()->wait ();

      ACE_DEBUG ((LM_DEBUG, "Now terminating test\n"));

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
