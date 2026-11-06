#include "test_i.h"
#include "ace/Get_Opt.h"
#include "ace/OS_NS_stdio.h"
#include <memory>

static int
parse_args (int argc, ACE_TCHAR *argv[], ACE_TCHAR const*& output_file)
{
  ACE_Get_Opt get_opts (argc, argv, ACE_TEXT("o:"));
  int c {};

  while ((c = get_opts ()) != -1)
    switch (c)
      {
      case 'o':
        output_file = get_opts.opt_arg ();
        break;

      case '?':
      default:
        ACE_ERROR_RETURN ((LM_ERROR,
                           ACE_TEXT ("usage:  %s "
                           "-o <iorfile>"
                           "\n"),
                           argv [0]),
                          -1);
      }
  // Indicates successful parsing of the command line
  return 0;
}

int
ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
  ACE_TCHAR const* output_file {};
  try
    {
      CORBA::ORB_var orb =
        CORBA::ORB_init (argc, argv);

      // We do the command line parsing first
      if (parse_args (argc, argv, output_file) != 0)
        return 1;

      CORBA::Object_var poa_object =
        orb->resolve_initial_references ("RootPOA");

      if (CORBA::is_nil (poa_object.in ()))
        ACE_ERROR_RETURN ((LM_ERROR,
                           ACE_TEXT (" (%P|%t) Unable to initialize the POA.\n")),
                          1);

      PortableServer::POA_var root_poa =
        PortableServer::POA::_narrow (poa_object.in ());

      PortableServer::POAManager_var poa_manager =
        root_poa->the_POAManager ();

      // Instantiate the Simple_Server implementation class.
      Simple_Server_i display_impl (orb.in ());

      Simple_Server_var server =
        display_impl._this ();

      CORBA::String_var ior =
        orb->object_to_string (server.in ());

      ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("Activated as <%C>\n"), ior.in ()));

      if (output_file != nullptr)
        {
          // Write the IOR to the requested output file.
          std::unique_ptr<FILE, int (*) (FILE *)> const output_stream (
            ACE_OS::fopen (output_file, "w"), ACE_OS::fclose);
          if (!output_stream)
            ACE_ERROR_RETURN ((LM_ERROR,
                               ACE_TEXT ("Cannot open output file for writing IOR: %s"),
                               output_file),
                              1);
          ACE_OS::fprintf (output_stream.get (), "%s", ior.in ());
        }

      poa_manager->activate ();

      orb->run ();
    }
  catch (CORBA::Exception const& ex)
    {
      ex._tao_print_exception ("Caught exception:");
      return 1;
    }
  return 0;
}
