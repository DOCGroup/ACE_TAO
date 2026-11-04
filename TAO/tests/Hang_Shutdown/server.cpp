#include "test_i.h"
#include "ace/Get_Opt.h"
#include "ace/OS_NS_stdio.h"
#include <memory>

namespace Test
{
  struct Server_Options
  {
    ACE_TString ior_output_file {ACE_TEXT ("server.ior")};
  };

  int
  parse_args (int argc, ACE_TCHAR *argv[], Server_Options &options)
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

  static int
  try_main (int argc,
            ACE_TCHAR *argv[])
  {
    try
      {
        Server_Options options;
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

        if (parse_args (argc, argv, options) != 0)
          return 1;

        test_i *test_impl {};
        // ServantBase_var takes ownership of this reference-counted TAO servant.
        ACE_NEW_RETURN (test_impl,
                        test_i (),
                        1);
        PortableServer::ServantBase_var const owner_transfer (test_impl);

        PortableServer::ObjectId_var const id =
          root_poa->activate_object (test_impl);

        CORBA::Object_var const object = root_poa->id_to_reference (id.in ());

        Hang_var const test =
          Hang::_narrow (object.in ());

        CORBA::String_var const ior =
          orb->object_to_string (test.in ());

        // If the ior_output_file exists, output the ior to it
        {
          std::unique_ptr<FILE, int (*) (FILE *)> output_file (
            ACE_OS::fopen (options.ior_output_file.c_str (), ACE_TEXT ("w")),
            ACE_OS::fclose);
          if (!output_file)
            ACE_ERROR_RETURN ((LM_ERROR,
                               "Cannot open output file for writing IOR: %s",
                               ACE_TEXT_ALWAYS_CHAR (options.ior_output_file.c_str ())),
                              1);
          ACE_OS::fprintf (output_file.get (), "%s", ior.in ());
        }

        poa_manager->activate ();

        ACE_Time_Value tv (10);

        orb->run (&tv);

        ACE_DEBUG ((LM_DEBUG,
                    "(%P|%t) server - event loop finished\n"));

        root_poa->destroy (true, true);

        orb->destroy ();
      }
    catch (CORBA::Exception const &ex)
      {
        ex._tao_print_exception ("Caught CORBA exception\n");
        return -1;
      }

    return 0;
  }
}

int
ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
  return Test::try_main (argc, argv);
}
