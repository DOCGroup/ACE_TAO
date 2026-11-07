#include "ace/Get_Opt.h"
#include "ace/OS_NS_stdio.h"
#include "ace/SString.h"
#include <memory>
#include "test_i.h"
#include "tao/ORBInitializer_Registry.h"
#include "Server_ORBInitializer.h"

struct Options
{
  ACE_TString ior_output_file { ACE_TEXT ("test.ior") };
};

int
parse_args (int argc, ACE_TCHAR *argv[], Options &options)
{
  ACE_Get_Opt get_opts (argc, argv, ACE_TEXT ("o:"));
  int c {};

  while ((c = get_opts ()) != -1)
    {
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
    }
  // Indicates successful parsing of the command line
  return 0;
}

int
ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
  try
    {
      // create initializer
      Server_ORBInitializer *temp_initializer {};
      Options options {};

      ACE_NEW_RETURN (temp_initializer,
                      Server_ORBInitializer,
                      -1);
      PortableInterceptor::ORBInitializer_var const initializer (temp_initializer);

      PortableInterceptor::register_orb_initializer (initializer.in ());
      // Now create an ORB
      CORBA::ORB_var const orb = CORBA::ORB_init (argc, argv);

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

      poa_manager->activate ();

      if (parse_args (argc, argv, options) != 0)
        return 1;

      Visual_i server_impl (orb.in ());

      PortableServer::ObjectId_var const id =
        root_poa->activate_object (&server_impl);

      CORBA::Object_var const test_obj =
        root_poa->id_to_reference (id.in ());

      Test_Interceptors::Visual_var const server =
        Test_Interceptors::Visual::_narrow (test_obj.in ());

      CORBA::String_var const ior =
        orb->object_to_string (server.in ());

      ACE_DEBUG ((LM_DEBUG,
                  "Test_Interceptors::Visual: <%C>\n",
                  ior.in ()));

      // Write the IOR before entering the event loop.
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
