// $Id$

#include "orbsvcs/AV/AVStreams_i.h"
#include "orbsvcs/AV/FlowSpec_Entry.h"
#include "tao/PortableServer/PortableServer.h"
#include "tao/Strategies/advanced_resource.h"
#include "tao/debug.h"
#include "ace/Get_Opt.h"
#include "ace/INET_Addr.h"

ACE_RCSID(Latency, ping, "$Id$")

const char *ping_ior = CORBA::string_dup ("file://ping.ior");
const char *pong_ior = CORBA::string_dup ("file://pong.ior");
const char *ping_address = CORBA::string_dup ("224.9.9.2:12345");
const char *pong_address = CORBA::string_dup ("224.9.9.2:23456");
const char *protocol = CORBA::string_dup ("UDP");

int milliseconds = 30000;

int
parse_args (int argc, char *argv[])
{
  ACE_Get_Opt get_opts (argc, argv, "f:g:s:r:t:p:d");
  int c;

  while ((c = get_opts ()) != -1)
    switch (c)
      {
      case 'f':
        ping_ior = get_opts.opt_arg ();
        break;

      case 'g':
        pong_ior = get_opts.opt_arg ();
        break;

      case 'r':
        ping_address = get_opts.opt_arg ();
        break;

      case 's':
        pong_address = get_opts.opt_arg ();
        break;

      case 't':
        milliseconds = ACE_OS::atoi (get_opts.opt_arg ());
        break;

      case 'p':
        protocol = get_opts.opt_arg ();
        break;

      case 'd':
        TAO_debug_level++;
        break;

      case '?':
      default:
        ACE_ERROR_RETURN ((LM_ERROR,
                           "usage:  %s "
                           "-f <ping_ior> "
                           "-g <ping_ior> "
                           "-s <ping_address> "
                           "-r <pong_address> "
                           "-t <milliseconds> "
                           "-p protocols "
                           "\n",
                           argv [0]),
                          -1);
      }


  // Indicates sucessful parsing of the command line
  return 0;
}

int main (int argc, char *argv[])
{
  ACE_TRY_NEW_ENV
    {

      parse_args (argc, argv);
      CORBA::ORB_var orb = CORBA::ORB_init (argc,
                                            argv);

      CORBA::Object_var obj
        = orb->resolve_initial_references ("RootPOA" TAO_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      PortableServer::POA_var poa
        = PortableServer::POA::_narrow (obj.in ());

      PortableServer::POAManager_var mgr
        = poa->the_POAManager ();

      mgr->activate ();

      TAO_AV_CORE::instance ()->init (orb.in (),
                                      poa.in ()
                                      TAO_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      // Connect the two streams and run them...
      AVStreams::flowSpec flow_spec (2);
      flow_spec.length (2);

      ACE_INET_Addr ping_addr;
      ping_addr.set (ping_address);
      TAO_Forward_FlowSpec_Entry ping ("Ping",
                                       "IN",
                                       "UNS:ping",
                                       "",
                                       protocol,
                                       &ping_addr);
      flow_spec[0] = CORBA::string_dup (ping.entry_to_string ());

      ACE_INET_Addr pong_addr;
      pong_addr.set (pong_address);
      TAO_Forward_FlowSpec_Entry pong ("Pong",
                                       "OUT",
                                       "UNS:pong",
                                       "",
                                       protocol,
                                       &pong_addr);
      flow_spec[1] = CORBA::string_dup (pong.entry_to_string ());

      TAO_StreamCtrl stream_control_impl;

      AVStreams::StreamCtrl_var stream_control =
        stream_control_impl._this (TAO_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;

      obj = orb->string_to_object (ping_ior TAO_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;
      AVStreams::MMDevice_var ping_sender =
        AVStreams::MMDevice::_narrow (obj.in () TAO_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      obj = orb->string_to_object (pong_ior TAO_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;
      AVStreams::MMDevice_var pong_sender =
        AVStreams::MMDevice::_narrow (obj.in () TAO_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      AVStreams::streamQoS_var the_qos =
        new AVStreams::streamQoS;

      stream_control->bind_devs (pong_sender.in (),
                                 ping_sender.in (),
                                 the_qos.inout (),
                                 flow_spec
                                 TAO_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      flow_spec.length (0);
      stream_control->start (flow_spec TAO_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      ACE_Time_Value tv (100, 0);
      orb->run (tv TAO_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      ACE_DEBUG ((LM_DEBUG, "event loop finished\n"));

     // flow_spec.length (0);
     // stream_control->stop (flow_spec TAO_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

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

// ****************************************************************

// @@ TODO
#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)

#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)

#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */
