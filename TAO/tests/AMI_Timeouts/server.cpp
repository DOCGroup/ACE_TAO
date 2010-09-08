
//=============================================================================
/**
 *  @file    server.cpp
 *
 *  $Id$
 *
 *  Implementation of the server running the Timeout object.
 *
 *  @author Michael Kircher <Michael.Kircher@mchp.siemens.de>
 */
//=============================================================================


#include "timeout_client.h"
#include "tao/debug.h"
#include "ace/Get_Opt.h"

const ACE_TCHAR *ior_output_file = 0;

int
parse_args (int argc, ACE_TCHAR *argv[])
{
  ACE_Get_Opt get_opts (argc, argv, ACE_TEXT("o:d"));
  int c;

  while ((c = get_opts ()) != -1)
    switch (c)
      {
      case 'o':
        ior_output_file = get_opts.opt_arg ();
        break;
      case 'd':
        ++TAO_debug_level;
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
        orb->resolve_initial_references ("RootPOA");

      if (CORBA::is_nil (poa_object.in ()))
        ACE_ERROR_RETURN ((LM_ERROR,
                           " (%P|%t) Unable to initialize the POA.\n"),
                          1);

      PortableServer::POA_var root_poa =
        PortableServer::POA::_narrow (poa_object.in ());

      PortableServer::POAManager_var poa_manager =
        root_poa->the_POAManager ();

      if (parse_args (argc, argv) != 0)
        return 1;

      Timeout_i timeout_i (orb.in ());

      PortableServer::ObjectId_var id =
        root_poa->activate_object (&timeout_i);

      CORBA::Object_var object = root_poa->id_to_reference (id.in ());

      TimeoutObj_var timeout_var =
        TimeoutObj::_narrow (object.in ());

      CORBA::String_var ior =
        orb->object_to_string (timeout_var.in ());

      ACE_DEBUG ((LM_DEBUG, "Activated as <%C>\n", ior.in ()));

      // If the ior_output_file exists, output the ior to it
      if (ior_output_file != 0)
        {
          FILE *output_file= ACE_OS::fopen (ior_output_file, "w");
          if (output_file == 0)
            ACE_ERROR_RETURN ((LM_ERROR,
                               "Cannot open output file for writing IOR: %s",
                               ior_output_file),
                              1);
          ACE_OS::fprintf (output_file, "%s", ior.in ());
          ACE_OS::fclose (output_file);
        }

      poa_manager->activate ();

      // Instantiate reply handler
      TimeoutHandler_i timeoutHandler_i;

      PortableServer::ObjectId_var idu =
        root_poa->activate_object (&timeoutHandler_i);

      CORBA::Object_var objectu = root_poa->id_to_reference (idu.in ());

      AMI_TimeoutObjHandler_var timeoutHandler_var =
        AMI_TimeoutObjHandler::_narrow (objectu.in ());

      // Instantiate client
      /*
      TimeoutClient* client = new TimeoutClient (orb,
                                                 timeout_var.in (),
                                                 timeoutHandler_var.in (),
                                                 &timeoutHandler_i);

      client->activate ();
      */

      orb->run ();


      ACE_DEBUG ((LM_DEBUG, "event loop finished\n"));

      root_poa->destroy (1,  // ethernalize objects
                         0  // wait for completion
                        );

      orb->destroy ();

      //delete client;
    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception ("Caught exception:");
      return 1;
    }

  return 0;
}
