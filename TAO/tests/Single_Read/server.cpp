#include "ace/Get_Opt.h"
#include "ace/SString.h"
#include "test_i.h"
#include "ace/OS_NS_stdio.h"
#include <memory>

struct Server_Options
{
  ACE_TString ior_output_file {ACE_TEXT ("ior")};
  ACE_TString client_done_file {ACE_TEXT ("client_done.ior")};
};

static int
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
        options.client_done_file = get_opts.opt_arg ();
        break;
      case '?':
      default:
        ACE_ERROR_RETURN ((LM_ERROR,
                           ACE_TEXT ("usage: %s -o <iorfile> -s <file_in_servant>\n"),
                           argv[0]),
                          -1);
      }

  return 0;
}

int
ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
  try
    {
      Server_Options options;
      CORBA::ORB_var const orb =
        CORBA::ORB_init (argc,
                         argv);

      CORBA::Object_var const poa_object =
        orb->resolve_initial_references ("RootPOA");

      PortableServer::POA_var const root_poa =
        PortableServer::POA::_narrow (poa_object.in ());

      PortableServer::POAManager_var const poa_manager =
        root_poa->the_POAManager ();

      if (parse_args (argc, argv, options) != 0)
        return -1;

      test_i servant (orb.in (), options.client_done_file);

      PortableServer::ObjectId_var const id =
        root_poa->activate_object (&servant);

      CORBA::Object_var const object = root_poa->id_to_reference (id.in ());

      test_var const server =
        test::_narrow (object.in ());

      CORBA::String_var const ior =
        orb->object_to_string (server.in ());

      ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("Activated as <%C>\n"), ior.in ()));

      {
        std::unique_ptr<FILE, int (*) (FILE *)> const output_file (
          ACE_OS::fopen (options.ior_output_file.c_str (), ACE_TEXT ("w")), ACE_OS::fclose);
        if (!output_file)
          ACE_ERROR_RETURN ((LM_ERROR,
                             ACE_TEXT ("Cannot open output file for writing IOR: %s"),
                             options.ior_output_file.c_str ()),
                            -1);
        ACE_OS::fprintf (output_file.get (), "%s", ior.in ());
      }

      poa_manager->activate ();

      orb->run ();

      ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("event loop finished\n")));

      root_poa->destroy (true, true);
    }
  catch (CORBA::Exception const& ex)
    {
      ex._tao_print_exception ("Exception caught:");
      return -1;
    }

  return 0;
}
