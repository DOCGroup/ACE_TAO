
//=============================================================================
/**
 *  @file    client.cpp
 *
 *  $Id$
 *
 *  A client, which uses the AMI callback model and timeouts.
 *
 *
 *  @author Michael Kircher <Michael.Kircher@mchp.siemens.de>
 */
//=============================================================================


#include "timeout_i.h"
#include "timeout_client.h"
#include "timeoutC.h"

#include "tao/debug.h"

#include "ace/Get_Opt.h"
#include "ace/Task.h"

ACE_RCSID (AMI,
           client,
           "$Id$")

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
        ior = get_opts.opt_arg ();
        break;
      case 't':
        msec = ACE_OS::atoi (get_opts.opt_arg ());
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
  TAO_ENV_DECLARE_NEW_ENV;
  ACE_TRY
    {
      CORBA::ORB_var orb =
        CORBA::ORB_init (argc, argv, "" TAO_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      if (parse_args (argc, argv) != 0)
        return 1;

      CORBA::Object_var object =
        orb->string_to_object (ior TAO_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      TimeoutObj_var timeout_var =
        TimeoutObj::_narrow (object.in () TAO_ENV_ARG_PARAMETER);
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
        orb->resolve_initial_references ("RootPOA" TAO_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      if (CORBA::is_nil (poa_object.in ()))
        ACE_ERROR_RETURN ((LM_ERROR,
                           " (%P|%t) Unable to initialize the POA.\n"),
                          1);

      PortableServer::POA_var root_poa =
        PortableServer::POA::_narrow (poa_object.in () TAO_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      PortableServer::POAManager_var poa_manager =
        root_poa->the_POAManager (TAO_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;

      poa_manager->activate (TAO_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;

      // Instantiate reply handler
      TimeoutHandler_i timeoutHandler_i;

      AMI_TimeoutObjHandler_var timeoutHandler_var =
        timeoutHandler_i._this (TAO_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;

      // Instantiate client
      TimeoutClient client (orb.in (),
                            timeout_var.in (),
                            timeoutHandler_var.in (),
                            &timeoutHandler_i,
                            msec);

      client.activate ();

      // ORB loop.
      orb->run (TAO_ENV_SINGLE_ARG_PARAMETER);  // Fetch responses
      ACE_TRY_CHECK;

      root_poa->destroy (1,  // ethernalize objects
                         0  // wait for completion
                         TAO_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      // Wait for all the threads to finish before destroying the
      // ORB.
      (void) client.thr_mgr ()->wait ();

      orb->destroy (TAO_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;

      ACE_DEBUG ((LM_DEBUG, "ORB finished\n"));

    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION,
                           "Caught exception:");
      return 1;
    }
  ACE_ENDTRY;

  return 0;
}
