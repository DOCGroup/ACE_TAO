#include "ace/Get_Opt.h"
#include "test_i.h"
#include "Server_ORBInitializer.h"

#include "tao/ORBInitializer_Registry.h"
#include "ace/OS_NS_stdio.h"
#include <memory>

ACE_TCHAR const *ior_output_file = ACE_TEXT ("test.ior");

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
      PortableInterceptor::ORBInitializer_ptr temp_initializer {};

      ACE_NEW_RETURN (temp_initializer,
                      Server_ORBInitializer,
                      -1);  // No exceptions yet!
      PortableInterceptor::ORBInitializer_var initializer =
        temp_initializer;

      PortableInterceptor::register_orb_initializer (initializer.in ());

      // Now we can create the ORB
      CORBA::ORB_var const orb = CORBA::ORB_init (argc, argv);

      CORBA::Object_var const poa_object = orb->resolve_initial_references ("RootPOA");

      if (CORBA::is_nil (poa_object.in ()))
        ACE_ERROR_RETURN ((LM_ERROR,
                           " (%P|%t) Unable to initialize the POA.\n"),
                          1);

      PortableServer::POA_var const root_poa = PortableServer::POA::_narrow (poa_object.in ());

      PortableServer::POAManager_var const poa_manager = root_poa->the_POAManager ();

      poa_manager->activate ();

      if (parse_args (argc, argv) != 0)
        return 1;

      CDR_Out_Arg_i server_impl (orb.in ());

      PortableServer::ObjectId_var const id = root_poa->activate_object (&server_impl);

      CORBA::Object_var const test_obj = root_poa->id_to_reference (id.in ());

      Interop::CDR_Out_Arg_var const server =
        Interop::CDR_Out_Arg::_narrow (test_obj.in ());

      CORBA::String_var const ior = orb->object_to_string (server.in ());

      ACE_DEBUG ((LM_DEBUG,
                  "Interop::CDR_Out_Arg: <%C>\n",
                  ior.in ()));

      // If the ior_output_file exists, output the ior to it
      if (ior_output_file != nullptr)
        {
          std::unique_ptr<FILE, int (*) (FILE *)> output_file (
            ACE_OS::fopen (ior_output_file, "w"), ACE_OS::fclose);
          if (!output_file)
            ACE_ERROR_RETURN ((LM_ERROR,
                               "Cannot open output file for writing IOR: %s",
                               ior_output_file),
                              1);
          ACE_OS::fprintf (output_file.get (), "%s", ior.in ());
        }

      orb->run ();

      ACE_DEBUG ((LM_DEBUG, "event loop finished\n"));

      root_poa->destroy (true, true);

      orb->destroy ();
    }
    catch (CORBA::Exception const &ex)
    {
      ex._tao_print_exception ("Caught exception in server:");
      return 1;
    }

  return 0;
}
