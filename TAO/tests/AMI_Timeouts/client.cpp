
//=============================================================================
/**
 *  @file    client.cpp
 *
 *  A client, which uses the AMI callback model and timeouts.
 *
 *  @author Michael Kircher <Michael.Kircher@mchp.siemens.de>
 */
//=============================================================================

#include "timeout_client.h"

#include "tao/debug.h"

#include "ace/Get_Opt.h"

struct Options
{
  ACE_TCHAR const *ior {ACE_TEXT ("file://test.ior")};
  unsigned int msec {50};
  bool debug {};
};

int
parse_args (int argc, ACE_TCHAR *argv[], Options &options)
{
  ACE_Get_Opt get_opts (argc, argv, ACE_TEXT("dk:t:"));
  int c {};

  while ((c = get_opts ()) != -1)
    switch (c)
      {
      case 'd':
        options.debug = true;
        break;
      case 'k':
        options.ior = get_opts.opt_arg ();
        break;
      case 't':
        options.msec = ACE_OS::atoi (get_opts.opt_arg ());
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
  // Indicates successful parsing of the command line
  return 0;
}

int
ACE_TMAIN(int argc, ACE_TCHAR *argv[])
{
  try
    {
      CORBA::ORB_var const orb =
        CORBA::ORB_init (argc, argv);

      Options options;
      if (parse_args (argc, argv, options) != 0)
        return 1;

      if (options.debug)
        ++TAO_debug_level;

      CORBA::Object_var const object =
        orb->string_to_object (options.ior);

      TimeoutObj_var const timeout_var =
        TimeoutObj::_narrow (object.in ());

      if (CORBA::is_nil (timeout_var.in ()))
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "Object reference <%s> is nil.\n",
                             options.ior),
                            1);
        }

      // Activate POA to handle the call back.

      CORBA::Object_var const poa_object =
        orb->resolve_initial_references ("RootPOA");

      if (CORBA::is_nil (poa_object.in ()))
        ACE_ERROR_RETURN ((LM_ERROR,
                           " (%P|%t) Unable to initialize the POA.\n"),
                          1);

      PortableServer::POA_var const root_poa =
        PortableServer::POA::_narrow (poa_object.in ());

      PortableServer::POAManager_var const poa_manager =
        root_poa->the_POAManager ();

      poa_manager->activate ();

      // Instantiate reply handler
      TimeoutHandler_i timeoutHandler_i;

      PortableServer::ObjectId_var const id =
        root_poa->activate_object (&timeoutHandler_i);

      CORBA::Object_var const object_act = root_poa->id_to_reference (id.in ());

      AMI_TimeoutObjHandler_var const timeoutHandler_var =
        AMI_TimeoutObjHandler::_narrow (object_act.in ());

      // Instantiate client
      TimeoutClient client (orb.in (),
                            timeout_var.in (),
                            timeoutHandler_var.in (),
                            timeoutHandler_i,
                            options.msec);

      client.activate ();

      // ORB loop.
      orb->run ();  // Fetch responses

      root_poa->destroy (true,  // ethernalize objects
                         false);  // wait for completion

      // Wait for all the threads to finish before destroying the
      // ORB.
      (void) client.thr_mgr ()->wait ();

      orb->destroy ();

      ACE_DEBUG ((LM_DEBUG, "ORB finished\n"));
    }
  catch (CORBA::Exception const &ex)
    {
      ex._tao_print_exception ("Caught exception:");
      return 1;
    }

  return 0;
}
