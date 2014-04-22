// $Id$

#include "pong.h"
#include "orbsvcs/AV/Protocol_Factory.h"
#include "tao/PortableServer/PortableServer.h"
#include "tao/Strategies/advanced_resource.h"
#include "tao/ORB.h"
#include "tao/debug.h"
#include "ace/Get_Opt.h"
#include "ace/High_Res_Timer.h"
#include "ace/Stats.h"
#include "ace/Throughput_Stats.h"

const ACE_TCHAR *ior_output_file = ACE_TEXT ("pong.ior");
const char *protocol = "RTP/UDP";
int milliseconds = 100;
size_t message_size = 64;
int respond = 1;
AVStreams::protocolSpec pong_protocols;
AVStreams::protocolSpec ping_protocols;

ACE_hrtime_t recv_throughput_base = 0;
ACE_Throughput_Stats recv_latency;

ACE_hrtime_t send_throughput_base = 0;
ACE_Throughput_Stats send_latency;


int
parse_args (int argc, ACE_TCHAR *argv[])
{
  ACE_Get_Opt get_opts (argc, argv, ACE_TEXT("xo:s:r:t:b:d"));
  int c;

  while ((c = get_opts ()) != -1)
    switch (c)
      {
      case 'o':
        ior_output_file = get_opts.opt_arg ();
        break;

      case 'r':
        {
          CORBA::ULong l = ping_protocols.length ();
          ping_protocols.length (l + 1);
          ping_protocols[l] = CORBA::string_dup (ACE_TEXT_ALWAYS_CHAR (get_opts.opt_arg ()));
        }
        break;

      case 's':
        {
          CORBA::ULong l = pong_protocols.length ();
          pong_protocols.length (l + 1);
          pong_protocols[l] = CORBA::string_dup (ACE_TEXT_ALWAYS_CHAR (get_opts.opt_arg ()));
        }
        break;

      case 't':
        milliseconds = ACE_OS::atoi (get_opts.opt_arg ());
        break;

      case 'b':
        message_size = ACE_OS::atoi (get_opts.opt_arg ());
        if (message_size < sizeof(ACE_hrtime_t))
          {
            ACE_DEBUG ((LM_DEBUG, "Invalid message size\n"));
            message_size = 64;
          }
        break;

      case 'x':
        respond = 0;
        break;
      case 'd':
        TAO_debug_level++;
        break;

      case '?':
      default:
        ACE_ERROR_RETURN ((LM_ERROR,
                           "usage:  %s "
                           "-o <iorfile> "
                           "-r <protocol=addr> "
                           "-s <protocol=addr> "
                           "-t <milliseconds> "
                           "\n",
                           argv [0]),
                          -1);
      }


  // If no protocols are specified use the default...
  if (pong_protocols.length () == 0)
    {
      pong_protocols.length (1);
      pong_protocols[0] = CORBA::string_dup ("UDP=localhost:23456");
    }

  if (ping_protocols.length () == 0)
    {
      ping_protocols.length (1);
      ping_protocols[0] = CORBA::string_dup ("UDP=localhost:12345");
    }

  // Indicates successful parsing of the command line
  return 0;
}

int ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
  try
    {
      CORBA::ORB_var orb = CORBA::ORB_init (argc, argv);

      parse_args (argc, argv);

      CORBA::Object_var obj
        = orb->resolve_initial_references ("RootPOA");

      PortableServer::POA_var poa
        = PortableServer::POA::_narrow (obj.in ());

      PortableServer::POAManager_var mgr
        = poa->the_POAManager ();

      mgr->activate ();

      TAO_AV_CORE::instance ()->init (orb.in (),
                                      poa.in ());

      Reactive_Strategy *reactive_strategy;
      ACE_NEW_RETURN (reactive_strategy,
                      Reactive_Strategy,
                      1);
      reactive_strategy->init (orb.in (), poa.in ());
      TAO_MMDevice *mmdevice_impl;
      ACE_NEW_RETURN (mmdevice_impl,
                      TAO_MMDevice (reactive_strategy),
                      1);

      AVStreams::MMDevice_var mmdevice =
        mmdevice_impl->_this ();

      CORBA::String_var ior =
        orb->object_to_string (mmdevice.in ());

      ACE_DEBUG ((LM_DEBUG, "Activated as <%C>\n", ior.in () ));

      // If the ior_output_file exists, output the ior to it
      if (ior_output_file != 0)
        {
          FILE *output_file= ACE_OS::fopen (ior_output_file, "w");
          if (output_file == 0)
            ACE_ERROR_RETURN ((LM_ERROR,
                               "Cannot open output file %s for writing IOR: %C",
                               ior_output_file,
                               ior.in ()),
                              1);
          ACE_OS::fprintf (output_file, "%s", ior.in ());
          ACE_OS::fclose (output_file);
        }

      Pong_Recv_FDev* pong_fdev_impl;
      ACE_NEW_RETURN (pong_fdev_impl,
                      Pong_Recv_FDev ("Pong"),
                      1);
      Ping_Send_FDev* ping_fdev_impl;
      ACE_NEW_RETURN (ping_fdev_impl,
                      Ping_Send_FDev ("Ping"),
                      1);

      AVStreams::FDev_var ping_fdev =
        ping_fdev_impl->_this ();
      AVStreams::FDev_var pong_fdev =
        pong_fdev_impl->_this ();

      mmdevice->add_fdev (ping_fdev.in ());
      if (respond == 1)
        {
          mmdevice->add_fdev (pong_fdev.in ());
        }

      orb->run ( );

      ACE_DEBUG ((LM_DEBUG, "event loop finished\n"));

      ACE_DEBUG ((LM_DEBUG, "Calibrating scale factory . . . "));
      ACE_High_Res_Timer::global_scale_factor_type gsf =
        ACE_High_Res_Timer::global_scale_factor ();
      ACE_DEBUG ((LM_DEBUG, "done\n"));

      recv_latency.dump_results (ACE_TEXT("Receive"), gsf);

      send_latency.dump_results (ACE_TEXT("Send"), gsf);

    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception ("Caught exception:");
      return 1;
    }

  return 0;
}

// ****************************************************************

Pong_Recv::Pong_Recv (void)
  :  TAO_FlowConsumer ("Pong",
                       pong_protocols,
                       "UNS:pong")
{
}

int
Pong_Recv::get_callback (const char *,
                         TAO_AV_Callback *&callback)
{
  // ACE_DEBUG ((LM_DEBUG,"Pong_Recv::get_callback\n"));
  callback = &this->callback_;
  return 0;
}

int
Pong_Recv_Callback::handle_stop (void)
{
  // ACE_DEBUG ((LM_DEBUG,"Pong_Recv_Callback::stop"));
  TAO_AV_CORE::instance ()->orb ()->shutdown ();
  return 0;
}

int
Pong_Recv_Callback::receive_frame (ACE_Message_Block *frame,
                                   TAO_AV_frame_info *,
                                   const ACE_Addr &)
{
  // ACE_DEBUG ((LM_DEBUG,"Pong_Recv_Callback::receive_frame\n"));

  ACE_hrtime_t now = ACE_OS::gethrtime ();
  for (const ACE_Message_Block *i = frame;
       i != 0;
       i = frame->cont ())
    {
      ACE_hrtime_t buf[2];

      if (frame->length () < sizeof(buf))
        {
          ACE_DEBUG ((LM_DEBUG, "Unexpected message size\n"));
          return 0;
        }

      ACE_OS::memcpy (buf, i->rd_ptr (), sizeof(buf));

      if (recv_throughput_base == 0)
        {
          recv_throughput_base = now;
        }
      recv_latency.sample (now - recv_throughput_base,
                           now - buf[0]);
    }
  return 0;
}

int
Pong_Recv_Callback::handle_destroy (void)
{
  ACE_DEBUG ((LM_DEBUG,"Pong_Recv_Callback::destroy\n"));
  return 0;
}

// ****************************************************************

Ping_Send::Ping_Send (void)
  :  TAO_FlowProducer ("Ping",
                       ping_protocols,
                       "UNS:ping")
{
}

int
Ping_Send::get_callback (const char *,
                         TAO_AV_Callback *&callback)
{
  // ACE_DEBUG ((LM_DEBUG,"Ping_Send::get_callback\n"));
  callback = &this->callback_;
  return 0;
}

Ping_Send_Callback::Ping_Send_Callback (void)
  :count_ (0)
{
  this->timeout_ = ACE_Time_Value (2);

  this->frame_.size (message_size);
  this->frame_.wr_ptr (message_size);
}

void
Ping_Send_Callback::get_timeout (ACE_Time_Value *&tv,
                                 void *&)
{
  tv = &this->timeout_;
}

int
Ping_Send_Callback::handle_timeout (void *)
{

  this->count_++;

  ACE_DEBUG ((LM_DEBUG, "Ping timeout frame %d\n", this->count_));

  if (this->count_ > 10)
    {
      TAO_AV_CORE::instance ()->orb ()->shutdown ();
      return 0;
    }

  ACE_hrtime_t stamp = ACE_OS::gethrtime ();
  ACE_OS::memcpy (this->frame_.rd_ptr (), &stamp, sizeof(stamp));

  int result = this->protocol_object_->send_frame (&this->frame_);
  if (result < 0)
    ACE_ERROR_RETURN ((LM_ERROR,
                       "Ping_Send_Callback::handle_timeout - send_frame - %p\n",
                       ""),
                      -1);

  if (send_throughput_base == 0)
    {
      send_throughput_base = stamp;
    }
  ACE_hrtime_t now = ACE_OS::gethrtime ();
  send_latency.sample (now - send_throughput_base,
                       now - stamp);

  return 0;
}

int
Ping_Send_Callback::handle_end_stream (void)
{
  return 0;
}
