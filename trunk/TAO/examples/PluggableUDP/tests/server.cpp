// $Id$

// ============================================================================
//
// = LIBRARY
//    TAO/examples/Simple/UDP
//
// = FILENAME
//    server.cpp
//
// = DESCRIPTION
//    Implementation of the server running the UDP object.
//
// = AUTHOR
//    Michael Kircher <Michael.Kircher@mchp.siemens.de>
//
// ============================================================================

#include "ace/Get_Opt.h"
#include "tao/corba.h"
#include "UDP_i.h"

ACE_RCSID(AMI, server, "$Id$")

const char *ior_output_file = 0;

int
parse_args (int argc, char *argv[])
{
  ACE_Get_Opt get_opts (argc, argv, "o:d");
  int c;

  while ((c = get_opts ()) != -1)
    switch (c)
      {
      case 'o':
        ior_output_file = get_opts.optarg;
        break;
      case 'd':
        TAO_debug_level++;
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
main (int argc, char *argv[])
{
  ACE_TRY_NEW_ENV
    {
      CORBA::ORB_var orb =
        CORBA::ORB_init (argc, argv, "", ACE_TRY_ENV);
      ACE_TRY_CHECK;

      CORBA::Object_var poa_object =
        orb->resolve_initial_references("RootPOA");
      if (CORBA::is_nil (poa_object.in ()))
        ACE_ERROR_RETURN ((LM_ERROR,
                           " (%P|%t) Unable to initialize the POA.\n"),
                          1);

      PortableServer::POA_var root_poa =
        PortableServer::POA::_narrow (poa_object.in (), ACE_TRY_ENV);
      ACE_TRY_CHECK;

      PortableServer::POAManager_var poa_manager =
        root_poa->the_POAManager (ACE_TRY_ENV);
      ACE_TRY_CHECK;

      // Install a persistent POA in order to achieve a persistent IOR
      // for our object.
      //*
      CORBA::PolicyList policies;
      policies.length (2);
      policies[0] =
        root_poa->create_lifespan_policy(PortableServer::PERSISTENT,
                                         ACE_TRY_ENV);
      ACE_TRY_CHECK;
      policies[1] =
        root_poa->create_id_assignment_policy (PortableServer::USER_ID,
                                               ACE_TRY_ENV);
      ACE_TRY_CHECK;


      PortableServer::POA_var persistent_poa =
        root_poa->create_POA("persistent",
                             poa_manager.in (),
                             policies,
                             ACE_TRY_ENV);
      ACE_TRY_CHECK;

      policies[0]->destroy (ACE_TRY_ENV);
      ACE_TRY_CHECK;

      policies[1]->destroy (ACE_TRY_ENV);
      ACE_TRY_CHECK;

      //*/
      if (parse_args (argc, argv) != 0)
        return 1;

      UDP_i udp_i;

      udp_i.orb (orb.in ());

      //*
      PortableServer::ObjectId_var id =
        PortableServer::string_to_ObjectId ("UDP_Object");

      persistent_poa->activate_object_with_id (id.in (),
                                               &udp_i,
                                               ACE_TRY_ENV);
      ACE_TRY_CHECK;

      CORBA::Object_var obj =
        persistent_poa->id_to_reference (id.in (),
                                         ACE_TRY_ENV);
      ACE_TRY_CHECK;


      UDP_var udp_var = UDP::_narrow (obj, ACE_TRY_ENV);
      //*/
      //UDP_var udp_var = udp_i._this (ACE_TRY_ENV);
      ACE_TRY_CHECK;
      if (CORBA::is_nil (udp_var.in ()))
        ACE_DEBUG ((LM_DEBUG,
                    "Failed to narrow correct object reference.\n"));

      CORBA::String_var ior =
        orb->object_to_string (udp_var.in (), ACE_TRY_ENV);
      ACE_TRY_CHECK;

      ACE_DEBUG ((LM_DEBUG, "Activated as <%s>\n", ior.in ()));

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

      poa_manager->activate (ACE_TRY_ENV);
      ACE_TRY_CHECK;


      /*
      // Instantiate reply handler
      UDP_i udpHandler_i;

      udpHandler_i.orb (orb.in ());

      UDP_var udpHandler_var =
        udpHandler_i._this (ACE_TRY_ENV);
      ACE_TRY_CHECK;

      // Instantiate client
      UDP_Client_i* client = new UDP_Client_i (orb,
                                               udp_var.in (),
                                               udpHandler_var.in ());

      client->activate ();
      */

      orb->run ();

      ACE_DEBUG ((LM_DEBUG, "event loop finished\n"));

      //delete client;
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION,
                           "Catched exception:");
      return 1;
    }
  ACE_ENDTRY;

  return 0;
}
