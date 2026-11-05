
//=============================================================================
/**
 *  @file    client.cpp
 *
 *  This is the client for the UDP performance test.
 *
 *  @author Michael Kircher <Michael.Kircher@mchp.siemens.de>
 */
//=============================================================================

#include "ace/Get_Opt.h"
#include "ace/Task.h"

#include <memory>

#include "UDPC.h"

#include "UDP_i.h"
#include "UDP_PerformanceClient.h"

// The following include file forces DIOP to be linked into the
// executable and initialized for static builds.
#include "tao/Strategies/advanced_resource.h"

struct Client_Options
{
  ACE_TCHAR const *ior { ACE_TEXT("file://test.ior") };
  ACE_UINT32 burst_messages { 1000 };
};

static int
parse_args (Client_Options &options, int argc, ACE_TCHAR *argv[])
{
  ACE_Get_Opt get_opts (argc, argv, ACE_TEXT("k:t:"));
  int c;

  while ((c = get_opts ()) != -1)
    switch (c)
      {
      case 'k':
        options.ior = get_opts.opt_arg ();
        break;
      case 't':
        {
          int const burst_messages = ACE_OS::atoi (get_opts.opt_arg ());
          if (burst_messages <= 0)
            ACE_ERROR_RETURN ((LM_ERROR,
                               ACE_TEXT ("The burst message count must be positive.\n")),
                              -1);
          options.burst_messages = static_cast<ACE_UINT32> (burst_messages);
        }
        break;
      case '?':
      default:
        ACE_ERROR_RETURN ((LM_ERROR,
                           ACE_TEXT ("usage:  %s -k <ior> -t <burst_messages>\n"),
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
        CORBA::ORB_init (argc, argv, "PerformanceClient");

      Client_Options options;
      if (parse_args (options, argc, argv) != 0)
        return 1;

      CORBA::Object_var object =
        orb->string_to_object (options.ior);

      UDP_var udp_var =
        UDP::_narrow (object.in ());

      if (CORBA::is_nil (udp_var.in ()))
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             ACE_TEXT ("Object reference <%s> is nil.\n"),
                             options.ior),
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
        PortableServer::POA::_narrow (poa_object.in ());

      PortableServer::POAManager_var poa_manager =
        root_poa->the_POAManager ();

      poa_manager->activate ();

      // Instantiate reply handler
      UDP_i udp_i;

      // let it remember our ORB
      udp_i.orb (orb.in ());

      UDP_var udpHandler_var =
        udp_i._this ();

      // Instantiate client
      std::unique_ptr<UDP_PerformanceClient> client =
        std::make_unique<UDP_PerformanceClient> (orb.in (),
                                                 udp_var.in (),
                                                 udp_i,
                                                 options.burst_messages);

      // let the client run in a separate thread
      client->activate ();

      // ORB loop, will be shut down by our client thread
      orb->run ();  // Fetch responses

      ACE_DEBUG ((LM_DEBUG, "ORB finished\n"));

      // The worker triggers shutdown; join it before destroying the callback servant.
      client->wait ();

      root_poa->destroy (true,  // ethernalize objects
                         false); // wait for completion

      orb->destroy ();
    }
  catch (CORBA::Exception const &ex)
    {
      ex._tao_print_exception ("Caught exception:");
      return 1;
    }

  return 0;
}
