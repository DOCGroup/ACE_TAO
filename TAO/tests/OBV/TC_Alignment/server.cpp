#include "test_i.h"

#include "ace/Get_Opt.h"
#include "ace/OS_NS_stdio.h"

#include <memory>

struct Server_Options
{
  ACE_TCHAR const *ior_output_file { ACE_TEXT ("test.ior") };
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
        options.ior_output_file = get_opts.optarg;
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
      Server_Options options;
      CORBA::ORB_var const orb = CORBA::ORB_init (argc, argv);

      CORBA::Object_var const poa_object =
        orb->resolve_initial_references ("RootPOA");

      if (CORBA::is_nil (poa_object.in ()))
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             " (%P|%t) Unable to initialize the POA.\n"),
                            1);
        }

      PortableServer::POA_var const root_poa =
        PortableServer::POA::_narrow (poa_object.in ());
      if (CORBA::is_nil (root_poa.in ()))
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             ACE_TEXT (" (%P|%t) Unable to narrow RootPOA.\n")),
                            1);
        }

      PortableServer::POAManager_var const poa_manager =
        root_poa->the_POAManager ();

      if (parse_args (argc, argv, options) != 0)
        return 1;

      test_i servant (orb.in ());

      PortableServer::ObjectId_var const id =
        root_poa->activate_object (&servant);

      CORBA::Object_var const object = root_poa->id_to_reference (id.in ());

      test_var const server =
        test::_narrow (object.in ());
      if (CORBA::is_nil (server.in ()))
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             ACE_TEXT ("Unable to narrow test object.\n")),
                            1);
        }

      CORBA::String_var const ior =
        orb->object_to_string (server.in ());

      ACE_DEBUG ((LM_DEBUG,
                  "Activated as <%C>\n",
                  ior.in ()));

      // Close the IOR file before entering the event loop.
      {
        std::unique_ptr<FILE, int (*) (FILE *)> const output_file (
          ACE_OS::fopen (options.ior_output_file, "w"), ACE_OS::fclose);

        if (!output_file)
          {
            ACE_ERROR_RETURN ((LM_ERROR,
                               ACE_TEXT ("Can't open output file for writing IOR: %s"),
                               options.ior_output_file),
                              1);
          }

        if (ACE_OS::fprintf (output_file.get (), "%s", ior.in ()) < 0)
          {
            ACE_ERROR_RETURN ((LM_ERROR,
                               ACE_TEXT ("Failed to write IOR to output file\n")),
                              1);
          }
        }

      poa_manager->activate ();

      orb->run ();

      root_poa->destroy (true, true);
      orb->destroy ();
    }
  catch (CORBA::Exception const& ex)
    {
      ex._tao_print_exception ("Server: exception caught - ");
      return 1;
    }

  return 0;
}
