#include "Process_Factory.h"
#include "Server_Task.h"
#include "ace/Get_Opt.h"
#include <memory>
#include <string>

static int
parse_args (int argc,
            ACE_TCHAR *argv[],
            std::basic_string<ACE_TCHAR> &ior_output_file)
{
  ACE_Get_Opt get_opts (argc, argv, ACE_TEXT ("o:"));
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
                           ACE_TEXT ("usage:  %s -o <iorfile>\n"),
                           argv [0]),
                          -1);
      }
  return 0;
}

int
ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
  try
    {
      std::basic_string<ACE_TCHAR> ior_output_file {ACE_TEXT ("test.ior")};
      CORBA::ORB_var const orb {CORBA::ORB_init (argc, argv)};

      CORBA::Object_var const poa_object {orb->resolve_initial_references ("RootPOA")};

      PortableServer::POA_var const root_poa {PortableServer::POA::_narrow (poa_object.in ())};

      if (CORBA::is_nil (root_poa.in ()))
        ACE_ERROR_RETURN ((LM_ERROR,
                           " (%P|%t) Panic: nil RootPOA\n"),
                          1);

      PortableServer::POAManager_var const poa_manager {root_poa->the_POAManager ()};

      if (parse_args (argc, argv, ior_output_file) != 0)
        return 1;

      Process_Factory *process_factory_impl {};
      ACE_NEW_RETURN (process_factory_impl,
                      Process_Factory (orb.in ()),
                      1);
      // The servant var owns the implementation while the POA uses it.
      PortableServer::ServantBase_var owner_transfer (process_factory_impl);

      PortableServer::ObjectId_var const id {root_poa->activate_object (process_factory_impl)};

      CORBA::Object_var const object {root_poa->id_to_reference (id.in ())};

      Test::Process_Factory_var const process {Test::Process_Factory::_narrow (object.in ())};

      CORBA::String_var const ior {orb->object_to_string (process.in ())};

      {
        std::unique_ptr<FILE, int (*) (FILE *)> output_file (
          ACE_OS::fopen (ior_output_file.c_str (), ACE_TEXT ("w")), ACE_OS::fclose);
        if (!output_file)
          ACE_ERROR_RETURN ((LM_ERROR,
                             ACE_TEXT ("Cannot open output file for writing IOR: %s"),
                             ior_output_file.c_str ()),
                            1);
        ACE_OS::fprintf (output_file.get (), "%s", ior.in ());
      }

      poa_manager->activate ();

      Server_Task server_task (orb.in (),
                               ACE_Thread_Manager::instance ());

      if (server_task.activate (THR_NEW_LWP | THR_JOINABLE, 8, 1) == -1)
        {
          ACE_ERROR ((LM_ERROR, "Error activating server task\n"));
        }

      ACE_Thread_Manager::instance ()->wait ();

      ACE_DEBUG ((LM_DEBUG, "(%P|%t) server - event loop finished\n"));

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
