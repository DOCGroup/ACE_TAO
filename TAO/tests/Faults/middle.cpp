#include "ace/Get_Opt.h"
#include "test_i.h"
#include "ace/OS_NS_stdio.h"
#include <memory>

struct Middle_Options
{
  ACE_TString ior_output_file {ACE_TEXT ("middle.ior")};
  ACE_TString ior {ACE_TEXT ("file://test.ior")};
  int iterations {10};
};

int
parse_args (int argc, ACE_TCHAR *argv[], Middle_Options &options)
{
  ACE_Get_Opt get_opts (argc, argv, ACE_TEXT("o:k:i:"));
  int c {};

  while ((c = get_opts ()) != -1)
    switch (c)
      {
      case 'o':
        options.ior_output_file = get_opts.opt_arg ();
        break;

      case 'k':
        options.ior = get_opts.opt_arg ();
        break;

      case 'i':
        options.iterations = ACE_OS::atoi (get_opts.opt_arg ());
        break;

      case '?':
      default:
        ACE_ERROR_RETURN ((LM_ERROR,
                           "usage:  %s "
                           "-o <iorfile>"
                           "-k <ior> "
                           "-i <niterations> "
                           "\n",
                           argv [0]),
                          -1);
      }
  // Indicates successful parsing of the command line
  return 0;
}

int
ACE_TMAIN(int argc, ACE_TCHAR *argv[])
{
  Middle_Options options;
  try
    {
      CORBA::ORB_var const orb =
        CORBA::ORB_init (argc, argv);

      CORBA::Object_var const poa_object =
        orb->resolve_initial_references("RootPOA");

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

      CORBA::Object_var const object =
        orb->string_to_object (ACE_TEXT_ALWAYS_CHAR (options.ior.c_str ()));

      Simple_Server_var const server =
        Simple_Server::_narrow (object.in ());

      if (CORBA::is_nil (server.in ()))
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             ACE_TEXT ("Object reference <%s> is nil.\n"),
                             options.ior.c_str ()),
                            1);
        }

      Middle_i middle_impl (orb.in (), server.in ());

      PortableServer::ObjectId_var id =
        root_poa->activate_object (&middle_impl);

      CORBA::Object_var const object_act = root_poa->id_to_reference (id.in ());

      Simple_Server_var const middle =
        Simple_Server::_narrow (object_act.in ());

      CORBA::String_var const ior =
        orb->object_to_string (middle.in ());

      ACE_DEBUG ((LM_DEBUG, "Activated as <%C>\n", ior.in ()));

      // If the ior_output_file exists, output the ior to it
      if (!options.ior_output_file.empty ())
        {
          std::unique_ptr<FILE, int (*) (FILE *)> output_file (
            ACE_OS::fopen (options.ior_output_file.c_str (), ACE_TEXT ("w")),
            ACE_OS::fclose);
          if (!output_file)
            ACE_ERROR_RETURN ((LM_ERROR,
                               ACE_TEXT ("Cannot open output file for writing IOR: %s"),
                               options.ior_output_file.c_str ()),
                              1);
          ACE_OS::fprintf (output_file.get (), "%s", ior.in ());
        }

      orb->run ();

      ACE_DEBUG ((LM_DEBUG, "middle - event loop finished\n"));

      root_poa->destroy (true, true);
    }
  catch (CORBA::Exception const &ex)
    {
      ex._tao_print_exception ("Exception caught in middle:");
      return 1;
    }

  return 0;
}
