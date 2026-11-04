#include "test_i.h"
#include "ace/Get_Opt.h"
#include "ace/OS_NS_stdio.h"
#include "tao/Strategies/advanced_resource.h"
#include <memory>

int
parse_args (int argc, ACE_TCHAR *argv[], ACE_TString &ior_output_file)
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

      ACE_TString ior_output_file (ACE_TEXT ("server.ior"));
      if (parse_args (argc, argv, ior_output_file) != 0)
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

      std::unique_ptr<test_i> test_impl = std::make_unique<test_i> ();
      PortableServer::ServantBase_var const owner_transfer (test_impl.release ());

      PortableServer::ObjectId_var const id =
        root_poa->activate_object (owner_transfer.in ());

      CORBA::Object_var const object = root_poa->id_to_reference (id.in ());

      test_var const test =
        test::_narrow (object.in ());

      CORBA::String_var const ior =
        orb->object_to_string (test.in ());

      // If the ior_output_file exists, output the ior to it
      {
        std::unique_ptr<FILE, int (*) (FILE *)> output_file (
          ACE_OS::fopen (ior_output_file.c_str (), ACE_TEXT ("w")), ACE_OS::fclose);
        if (!output_file)
          ACE_ERROR_RETURN ((LM_ERROR,
                             "Cannot open output file for writing IOR: %s",
                             ior_output_file.c_str ()),
                            1);
        ACE_OS::fprintf (output_file.get (), "%s", ior.in ());
      }

      poa_manager->activate ();

      orb->run ();

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
