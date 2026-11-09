#include "Hello.h"
#include "tao/Dynamic_TP/DTP_POA_Strategy.h"
#include "ace/Get_Opt.h"
#include "ace/OS_NS_stdio.h"
#include "ace/Thread_Manager.h"

#include <memory>

struct Server_Options
{
  ACE_TString ior_output_file {ACE_TEXT ("server.ior")};
  int sleep_sec {1};
};

int
parse_args (int argc, ACE_TCHAR *argv[], Server_Options &options)
{
  ACE_Get_Opt get_opts (argc, argv, ACE_TEXT ("o:s:"));
  int c {};

  while ((c = get_opts ()) != -1)
    switch (c)
      {
      case 'o':
        options.ior_output_file = get_opts.opt_arg ();
        break;
      case 's':
        options.sleep_sec = static_cast<int> (ACE_OS::strtol (get_opts.opt_arg (), nullptr, 0));
        break;
      case '?':
      default:
        ACE_ERROR_RETURN ((LM_ERROR,
                           ACE_TEXT ("usage:  %s ")
                           ACE_TEXT ("-o <iorfile> ")
                           ACE_TEXT ("-s (# sec to sleep)\n"),
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

      Server_Options options;
      if (parse_args (argc, argv, options) != 0)
        return 1;

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

      CORBA::PolicyList const policies {};

      PortableServer::POA_var const mp2 = root_poa->create_POA ("MyPOA2", poa_manager.in (), policies);

      std::unique_ptr<Hello> hello_impl {std::make_unique<Hello> (orb.in ())};
      PortableServer::ServantBase_var owner_transfer (hello_impl.release ());

      PortableServer::ObjectId_var const id =
        root_poa->activate_object (owner_transfer.in ());

      CORBA::Object_var const object = root_poa->id_to_reference (id.in ());

      Test::Hello_var const hello =
        Test::Hello::_narrow (object.in ());

      CORBA::String_var const ior =
        orb->object_to_string (hello.in ());

      // Output the IOR to the <ior_output_file>
      {
        std::unique_ptr<FILE, int (*) (FILE *)> const output_file (
          ACE_OS::fopen (options.ior_output_file.c_str (), "w"), ACE_OS::fclose);
        if (!output_file)
          ACE_ERROR_RETURN ((LM_ERROR,
                             ACE_TEXT ("Cannot open output file %s for writing IOR: %C"),
                             options.ior_output_file.c_str (),
                             ior.in ()),
                            1);
        ACE_OS::fprintf (output_file.get (), "%s", ior.in ());
      }

      ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("Server calling poa_manager::activate()\n")));
      poa_manager->activate ();

      ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("Server calling hello->sleep_sec()\n")));
      // Set the sleep time for the Hello object
      hello->sleep_sec (options.sleep_sec);

      ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("Server calling orb::run()\n")));
      orb->run ();

      ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("(%P|%t) server - event loop finished\n")));
      // Wait for all CSD task threads exit.
      ACE_Thread_Manager::instance ()->wait ();

      root_poa->destroy (true, true);

      orb->destroy ();
    }
  catch (CORBA::Exception const &ex)
    {
      ex._tao_print_exception (ACE_TEXT ("Exception caught:"));
      return 1;
    }

  return 0;
}
