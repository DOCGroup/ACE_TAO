#include "Echo.h"
#include "Server_ORBInitializer.h"
#include "tao/ORBInitializer_Registry.h"
#include "ace/Get_Opt.h"
#include "ace/SString.h"
#include "ace/OS_NS_stdio.h"
#include <memory>

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
      Options options;
      if (parse_args (argc, argv, options) != 0)
        return 1;

      PortableInterceptor::ORBInitializer_var initializer (
        new Server_ORBInitializer);
      PortableInterceptor::register_orb_initializer (initializer.in ());

      CORBA::ORB_var orb = CORBA::ORB_init (argc, argv);

      CORBA::Object_var poa_object =
        orb->resolve_initial_references ("RootPOA");

      PortableServer::POA_var root_poa =
        PortableServer::POA::_narrow (poa_object.in ());

      if (CORBA::is_nil (root_poa.in ()))
        ACE_ERROR_RETURN ((LM_ERROR,
                           " (%P|%t) Panic: nil RootPOA\n"),
                          1);

      PortableServer::POAManager_var poa_manager =
        root_poa->the_POAManager ();

      // Keep the pointer for activate_object while transferring ownership to the var.
      Echo *const echo_impl = new Echo (orb.in ());
      PortableServer::ServantBase_var owner_transfer (echo_impl);

      PortableServer::ObjectId_var id =
        root_poa->activate_object (echo_impl);

      CORBA::Object_var object = root_poa->id_to_reference (id.in ());

      Test::Echo_var echo =
        Test::Echo::_narrow (object.in ());

      CORBA::String_var ior =
        orb->object_to_string (echo.in ());

      // Output the IOR to the <ior_output_file>
      {
        std::unique_ptr<FILE, int (*) (FILE *)> output_file (
          ACE_OS::fopen (options.ior_output_file.c_str (), "w"), ACE_OS::fclose);
        if (!output_file)
          ACE_ERROR_RETURN ((LM_ERROR,
                             "Cannot open output file for writing IOR: %s",
                             options.ior_output_file.c_str ()),
                            1);
        ACE_OS::fprintf (output_file.get (), "%s", ior.in ());
      }

      poa_manager->activate ();

      orb->run ();

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
