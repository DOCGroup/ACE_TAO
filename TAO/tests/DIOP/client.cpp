
//=============================================================================
/**
 *  @file    client.cpp
 *
 *  This is the client for the UDP test.
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
#include "ace/SString.h"

// The following include file forces DIOP to be linked into the
// executable and initialized for static builds.
#include "tao/Strategies/advanced_resource.h"

struct Client_Options
{
  ACE_TString ior {ACE_TEXT ("file://test.ior")};
  unsigned int msec {500};
  unsigned int iterations {1};
};

int
parse_args (int argc, ACE_TCHAR *argv[], Client_Options &options)
{
  ACE_Get_Opt get_opts (argc, argv, ACE_TEXT("dk:t:i:"));
  int c {};

  while ((c = get_opts ()) != -1)
    switch (c)
      {
      case 'd':
        ++TAO_debug_level;
        break;
      case 'k':
        options.ior = get_opts.opt_arg ();
        break;
      case 't':
        options.msec = ACE_OS::atoi (get_opts.opt_arg ());
        break;
      case 'i':
        options.iterations = ACE_OS::atoi (get_opts.opt_arg ());
        break;
      case '?':
      default:
        ACE_ERROR_RETURN ((LM_ERROR,
                           ACE_TEXT ("usage: %s -d -k <ior> -t <timeout in ms> "
                                     "-i <iterations>\n"),
                           argv [0]),
                          -1);
      }
  // Indicates successful parsing of the command line
  return 0;
}

int
ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
  Client_Options options;
  try
    {
      CORBA::ORB_var const orb =
        CORBA::ORB_init (argc, argv);

      if (parse_args (argc, argv, options) != 0)
        return 1;

      CORBA::Object_var const object =
        orb->string_to_object (options.ior.c_str ());

      UDP_var const udp_var =
        UDP::_narrow (object.in ());

      if (CORBA::is_nil (udp_var.in ()))
        ACE_ERROR_RETURN ((LM_ERROR,
                           "Object reference <%s> is nil.\n",
                           options.ior.c_str ()),
                          1);

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
      UDP_i udp_i (orb.in ());

      PortableServer::ObjectId_var const id =
        root_poa->activate_object (&udp_i);

      CORBA::Object_var const object_act = root_poa->id_to_reference (id.in ());

      UDP_var const udpHandler_var =
        UDP::_narrow (object_act.in ());

      // Instantiate client
      // The task must remain alive until its worker thread has been joined below.
      UDP_Client_i *client {};
      ACE_NEW_RETURN (client,
                      UDP_Client_i (orb.in (), udp_var.in (), udpHandler_var.in (),
                                    options.msec, options.iterations),
                      1);

      // let the client run in a separate thread
      client->activate ();

      // ORB loop, will be shut down by our client thread

      orb->run ();

      ACE_DEBUG ((LM_DEBUG, "event loop finished\n"));

      root_poa->destroy (true,  // ethernalize objects
                         false);  // wait for completion

      orb->destroy ();

      client->wait ();
      delete client;
    }
  catch (CORBA::Exception const &ex)
    {
      ex._tao_print_exception ("Caught exception:");
      return 1;
    }

  return 0;
}
