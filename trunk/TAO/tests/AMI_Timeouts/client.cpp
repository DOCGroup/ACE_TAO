// $Id$

// ============================================================================
//
// = LIBRARY
//    TAO/tests/AMI
//
// = FILENAME
//    server.cpp
//
// = DESCRIPTION
//    A client, which uses the AMI callback model and timeouts.
//
// = AUTHOR
//    Michael Kircher <Michael.Kircher@mchp.siemens.de>
//
// ============================================================================

#include "ace/Get_Opt.h"
#include "ace/Task.h"

#include "timeoutC.h"

#include "timeout_i.h"
#include "timeout_client.h"

ACE_RCSID(AMI, client, "$Id$")

const char *ior = "file://test.ior";
unsigned int msec = 50;

int
parse_args (int argc, char *argv[])
{
  ACE_Get_Opt get_opts (argc, argv, "dk:t:");
  int c;

  while ((c = get_opts ()) != -1)
    switch (c)
      {
      case 'd':
        TAO_debug_level++;
        break;
      case 'k':
        ior = get_opts.optarg;
        break;
      case 't':
        msec = ACE_OS::atoi (get_opts.optarg);
        break;
      case '?':
      default:
        ACE_ERROR_RETURN ((LM_ERROR,
                           "usage:  %s "
                           "-d "
                           "-k <ior> "
                           "-t <timeout in ms> "
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

      if (parse_args (argc, argv) != 0)
        return 1;

      CORBA::Object_var object =
        orb->string_to_object (ior, ACE_TRY_ENV);
      ACE_TRY_CHECK;

      Timeout_var timeout_var =
        Timeout::_narrow (object.in (), ACE_TRY_ENV);
      ACE_TRY_CHECK;

      if (CORBA::is_nil (timeout_var.in ()))
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "Object reference <%s> is nil\n",
                             ior),
                            1);
        }

      // Activate POA to handle the call back.

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

      poa_manager->activate (ACE_TRY_ENV);
      ACE_TRY_CHECK;

      // Instantiate reply handler
      TimeoutHandler_i timeoutHandler_i;

      AMI_TimeoutHandler_var timeoutHandler_var =
        timeoutHandler_i._this (ACE_TRY_ENV);
      ACE_TRY_CHECK;

      // Instantiate client
      TimeoutClient* client = new TimeoutClient (orb.in (),
                                                 timeout_var.in (),
                                                 timeoutHandler_var.in (),
                                                 &timeoutHandler_i,
                                                 msec);

      client->activate ();

      // ORB loop.
      orb->run ();  // Fetch responses

      ACE_DEBUG ((LM_DEBUG, "ORB finished\n"));

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
