
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


#include "timeout_client.h"

#include "tao/debug.h"

#include "ace/Get_Opt.h"

const ACE_TCHAR *ior = ACE_TEXT("file://test.ior");
unsigned int msec = 50;

int
parse_args (int argc, ACE_TCHAR *argv[])
{
  ACE_Get_Opt get_opts (argc, argv, ACE_TEXT("dk:t:"));
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
ACE_TMAIN(int argc, ACE_TCHAR *argv[])
{
  try
    {
      CORBA::ORB_var orb =
        CORBA::ORB_init (argc, argv);

      if (parse_args (argc, argv) != 0)
        return 1;

      CORBA::Object_var object =
        orb->string_to_object (ior);

      TimeoutObj_var timeout_var =
        TimeoutObj::_narrow (object.in ());

      if (CORBA::is_nil (timeout_var.in ()))
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "Object reference <%s> is nil.\n",
                             ior),
                            1);
        }

      // Activate POA to handle the call back.

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

      poa_manager->activate ();

      // Instantiate reply handler
      TimeoutHandler_i timeoutHandler_i;

      PortableServer::ObjectId_var id =
        root_poa->activate_object (&timeoutHandler_i);

      CORBA::Object_var object_act = root_poa->id_to_reference (id.in ());

      AMI_TimeoutObjHandler_var timeoutHandler_var =
        AMI_TimeoutObjHandler::_narrow (object_act.in ());

      // Instantiate client
      TimeoutClient client (orb.in (),
                            timeout_var.in (),
                            timeoutHandler_var.in (),
                            &timeoutHandler_i,
                            msec);

      client.activate ();

      // ORB loop.
      orb->run ();  // Fetch responses

      root_poa->destroy (1,  // ethernalize objects
                         0  // wait for completion
                        );

      // Wait for all the threads to finish before destroying the
      // ORB.
      (void) client.thr_mgr ()->wait ();

      orb->destroy ();

      ACE_DEBUG ((LM_DEBUG, "ORB finished\n"));

    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception ("Caught exception:");
      return 1;
    }

  return 0;
}
