
//=============================================================================
/**
 *  @file    server.cpp
 *
 *  Implementation of the server running the UDP object.
 *
 *  @author Michael Kircher <Michael.Kircher@mchp.siemens.de>
 */
//=============================================================================

#include "ace/Get_Opt.h"

#include <cstdio>
#include <memory>

#include "test_i.h"

// The following include file forces DIOP to be linked into the
// executable and initialized for static builds.
#include "tao/Strategies/advanced_resource.h"

struct Server_Options
{
  ACE_TCHAR const *ior_output_file { ACE_TEXT ("test.ior") };
};

static int
parse_args (Server_Options &options, int argc, ACE_TCHAR *argv[])
{
  ACE_Get_Opt get_opts (argc, argv, ACE_TEXT("o:"));
  int c;

  while ((c = get_opts ()) != -1)
    switch (c)
      {
      case 'o':
        options.ior_output_file = get_opts.opt_arg ();
        break;

      case '?':
      default:
        ACE_ERROR_RETURN ((LM_ERROR,
                           ACE_TEXT ("usage:  %s -o <iorfile>\n"),
                           argv [0]),
                          -1);
      }
  // Indicates successful parsing of the command line
  return 0;
}

int ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
  try
    {
      CORBA::ORB_var orb =
        CORBA::ORB_init (argc,
                         argv,
                         "ORB_Test_Server");

      Server_Options options;
      if (parse_args (options, argc, argv) != 0)
        return 1;

      CORBA::Object_var poa_object =
        orb->resolve_initial_references("RootPOA");

      if (CORBA::is_nil (poa_object.in ()))
        ACE_ERROR_RETURN ((LM_ERROR,
                           " (%P|%t) Unable to initialize the POA.\n"),
                          1);

      PortableServer::POA_var root_poa =
        PortableServer::POA::_narrow (poa_object.in ());

      PortableServer::POAManager_var poa_manager =
        root_poa->the_POAManager ();

      Simple_Server_i server_impl (orb.in ());

      PortableServer::ObjectId_var oidServer =
        root_poa->activate_object (&server_impl);

      CORBA::Object_var objOne =
        root_poa->id_to_reference (oidServer.in ());

      Simple_Server_var server = Simple_Server::_narrow (objOne.in ());

      CORBA::String_var ior = orb->object_to_string (server.in ());

      ACE_DEBUG ((LM_DEBUG, "Activated as <%C>\n", ior.in ()));

      {
        std::unique_ptr<FILE, int (*) (FILE *)> output_file (
          ACE_OS::fopen (options.ior_output_file, ACE_TEXT ("w")),
          ACE_OS::fclose);
        if (!output_file)
          ACE_ERROR_RETURN ((LM_ERROR,
                             ACE_TEXT ("Cannot open output file for writing IOR: %s"),
                             options.ior_output_file),
                            1);
        ACE_OS::fprintf (output_file.get (), "%s", ior.in ());
      }

      poa_manager->activate ();

      orb->run ();

      ACE_DEBUG ((LM_DEBUG, "event loop finished\n"));

      root_poa->destroy (true, true);

      orb->destroy ();
    }
  catch (CORBA::Exception const &ex)
    {
      ex._tao_print_exception ("Caught exception:");
      return 1;
    }

  return 0;
}
