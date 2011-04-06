
//=============================================================================
/**
 *  @file    client.cpp
 *
 *  $Id$
 *
 *  This is the client for the UDP test.
 *
 *
 *  @author Michael Kircher <Michael.Kircher@mchp.siemens.de>
 */
//=============================================================================


#include "UDPC.h"

#include "UDP_i.h"
#include "UDP_Client_i.h"

#include "tao/debug.h"

#include "ace/Get_Opt.h"
#include "ace/Task.h"

// The following include file forces DIOP to be linked into the
// executable and initialized for static builds.
#include "tao/Strategies/advanced_resource.h"

const ACE_TCHAR *ior = ACE_TEXT("file://test.ior");
unsigned int msec = 500;
unsigned int iterations = 1;

int
parse_args (int argc, ACE_TCHAR *argv[])
{
  ACE_Get_Opt get_opts (argc, argv, ACE_TEXT("dk:t:i:"));
  int c;

  while ((c = get_opts ()) != -1)
    switch (c)
      {
      case 'd':
        ++TAO_debug_level;
        break;
      case 'k':
        ior = get_opts.opt_arg ();
        break;
      case 't':
        msec = ACE_OS::atoi (get_opts.opt_arg ());
        break;
      case 'i':
        iterations = ACE_OS::atoi (get_opts.opt_arg ());
        break;
      case '?':
      default:
        ACE_ERROR_RETURN ((LM_ERROR,
                           "usage:  %s "
                           "-d "
                           "-k <ior> "
                           "-t <timeout in ms> "
                           "-i <iterations> "
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
      CORBA::ORB_var orb =
        CORBA::ORB_init (argc, argv);

      if (parse_args (argc, argv) != 0)
        return 1;

      CORBA::Object_var object =
        orb->string_to_object (ior);

      UDP_var udp_var =
        UDP::_narrow (object.in ());

      if (CORBA::is_nil (udp_var.in ()))
        ACE_ERROR_RETURN ((LM_ERROR,
                           "Object reference <%s> is nil.\n",
                           ior),
                          1);

      // Activate POA to handle the call back.

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

      poa_manager->activate ();

      // Instantiate reply handler
      UDP_i udp_i (orb.in ());

      PortableServer::ObjectId_var id =
        root_poa->activate_object (&udp_i);

      CORBA::Object_var object_act = root_poa->id_to_reference (id.in ());

      UDP_var udpHandler_var =
        UDP::_narrow (object_act.in ());

      // Instantiate client
      UDP_Client_i *client = new UDP_Client_i (orb.in (),
                                               udp_var.in (),
                                               udpHandler_var.in (),
                                               msec,
                                               iterations);

      // let the client run in a separate thread
      client->activate ();

      // ORB loop, will be shut down by our client thread

      orb->run ();

      ACE_DEBUG ((LM_DEBUG, "event loop finished\n"));

      root_poa->destroy (1,  // ethernalize objects
                         0);  // wait for completion

      orb->destroy ();

      client->wait ();
      // it is save to delete the client, because the client was actually
      // the one calling orb->shutdown () triggering the end of the ORB
      // event loop.
      delete client;
    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception ("Caught exception:");
      return 1;
    }

  return 0;
}
