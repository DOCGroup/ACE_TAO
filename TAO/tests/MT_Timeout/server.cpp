#include "Sleep_Service.h"
#include "Server_Task.h"
#include "ace/Get_Opt.h"
#include "ace/OS_NS_stdio.h"
#include <memory>
#include <string>

std::basic_string<ACE_TCHAR> ior_output_file = ACE_TEXT ("test.ior");

int
parse_args (int argc, ACE_TCHAR *argv[])
{
  ACE_Get_Opt get_opts (argc, argv, ACE_TEXT("o:"));
  int c {};

  while ((c = get_opts ()) != -1)
    switch (c)
      {
      case 'o':
        ior_output_file = get_opts.opt_arg ();
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

      if (parse_args (argc, argv) != 0)
        return 1;

      // Keep the raw servant pointer until ownership transfers to ServantBase_var.
      Sleep_Service *sleep_service_impl = nullptr;
      ACE_NEW_RETURN (sleep_service_impl,
                      Sleep_Service (orb.in ()),
                      1);
      PortableServer::ServantBase_var const owner_transfer (sleep_service_impl);

      PortableServer::ObjectId_var const id =
        root_poa->activate_object (sleep_service_impl);

      CORBA::Object_var const object = root_poa->id_to_reference (id.in ());

      Test::Sleep_Service_var const process =
        Test::Sleep_Service::_narrow (object.in ());

      CORBA::String_var const ior =
        orb->object_to_string (process.in ());

      // If the ior_output_file exists, output the ior to it
      std::unique_ptr<FILE, int (*) (FILE *)> output_file (
        ACE_OS::fopen (ior_output_file.c_str (), "w"), ACE_OS::fclose);
      if (!output_file)
        ACE_ERROR_RETURN ((LM_ERROR,
                           "Cannot open output file for writing IOR: %s",
                           ior_output_file.c_str ()),
                          1);
      ACE_OS::fprintf (output_file.get (), "%s", ior.in ());
      output_file.reset ();

      poa_manager->activate ();

      Server_Task server_task (ACE_Thread_Manager::instance (),
                               orb.in (),
                               ACE_Time_Value (240));

      if (server_task.activate (THR_NEW_LWP | THR_JOINABLE, 8, 1) == -1)
        {
          ACE_ERROR ((LM_ERROR, "Error activating server task\n"));
        }

      server_task.thr_mgr ()->wait ();

      ACE_DEBUG ((LM_DEBUG, "(%P|%t) server - event loop finished\n"));

      root_poa->destroy (true, true);

      orb->destroy ();
    }
  catch (CORBA::Exception const& ex)
    {
      ex._tao_print_exception ("Exception caught:");
      return 1;
    }

  return 0;
}
