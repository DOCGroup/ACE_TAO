// $Id$

#include "test_dsi.h"
#include "ace/Get_Opt.h"
#include "ace/Task.h"

ACE_RCSID(DSI_Gateway, server, "$Id$")

const ACE_TCHAR *ior = ACE_TEXT("file://test.ior");
int niterations = 5;
int do_shutdown = 0;
const ACE_TCHAR *ior_output_file = ACE_TEXT("gateway.ior");

int
parse_args (int argc, ACE_TCHAR *argv[])
{
  ACE_Get_Opt get_opts (argc, argv, ACE_TEXT("xk:i:o:"));
  int c;

  while ((c = get_opts ()) != -1)
    switch (c)
      {
      case 'x':
        do_shutdown = 1;
        break;

      case 'k':
        ior = get_opts.opt_arg ();
        break;

      case 'i':
        niterations = ACE_OS::atoi (get_opts.opt_arg ());
        break;

      case 'o':
              ior_output_file = get_opts.opt_arg ();
              break;

      case '?':
      default:
        ACE_ERROR_RETURN ((LM_ERROR,
                           "usage:  %s "
                           "-x "
                           "-k <ior> "
                           "-i <niterations> "
                                             "-o <iorfile> "
                           "\n",
                           argv [0]),
                          -1);
      }

  // Indicates sucessful parsing of the command line
  return 0;
}

int
ACE_TMAIN(int argc, ACE_TCHAR *argv[])
{
  try
    {
      CORBA::ORB_var orb =
        CORBA::ORB_init (argc, argv);

      CORBA::Object_var poa_object =
        orb->resolve_initial_references("RootPOA");

      if (CORBA::is_nil (poa_object.in ()))
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             " (%P|%t) Unable to initialize the POA.\n"),
                            1);
        }

      PortableServer::POA_var root_poa =
        PortableServer::POA::_narrow (poa_object.in ());

      PortableServer::POAManager_var poa_manager =
        root_poa->the_POAManager ();

      poa_manager->activate ();

      if (parse_args (argc, argv) != 0)
        {
          return 1;
        }

      CORBA::Object_var object =
        orb->string_to_object (ior);

      DSI_Simple_Server server_impl (orb.in (),
                                     object.in (),
                                     root_poa.in ());
      PortableServer::ObjectId_var oid =
        root_poa->activate_object (&server_impl);

      CORBA::Object_var server =
        root_poa->id_to_reference (oid.in ());

      CORBA::String_var ior =
              orb->object_to_string (server.in ());

      ACE_DEBUG ((LM_DEBUG, "Activated as <%C>\n", ior.in ()));

      // If the ior_output_file exists, output the ior to it
      if (ior_output_file != 0)
              {
                FILE *output_file= ACE_OS::fopen (ior_output_file, "w");

                if (output_file == 0)
            {
                    ACE_ERROR_RETURN ((LM_ERROR,
                                       "Cannot open output file for writing IOR: %s",
                                       ior_output_file),
                                      1);
            }

                ACE_OS::fprintf (output_file, "%s", ior.in ());
                ACE_OS::fclose (output_file);
              }

      orb->run ();

      ACE_DEBUG ((LM_DEBUG, "event loop finished\n"));
    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception ("Gateway: exception caught - ");
      return 1;
    }

  return 0;
}
