// $Id$

#include "ping.h"
#include "orbsvcs/AV/Protocol_Factory.h"
#include "tao/corba.h"
#include "tao/TAO.h"
#include "ace/Get_Opt.h"

ACE_RCSID(Latency, ping, "$Id$")

const char *ior_output_file = "ping.ior";
const char *protocol = "RTP/UDP";
int milliseconds = 100;
AVStreams::protocolSpec recv_protocols;
AVStreams::protocolSpec send_protocols;

Pong_Send_Callback pong_callback;

int
parse_args (int argc, char *argv[])
{
  ACE_Get_Opt get_opts (argc, argv, "o:s:r:t:");
  int c;

  while ((c = get_opts ()) != -1)
    switch (c)
      {
      case 'o':
        ior_output_file = get_opts.optarg;
        break;

      case 'r':
        {
          CORBA::ULong l = recv_protocols.length ();
          recv_protocols.length (l + 1);
          recv_protocols[l] = CORBA::string_dup (get_opts.optarg);
        }
        break;

      case 's':
        {
          CORBA::ULong l = send_protocols.length ();
          send_protocols.length (l + 1);
          send_protocols[l] = CORBA::string_dup (get_opts.optarg);
        }
        break;

      case 't':
        milliseconds = ACE_OS::atoi (get_opts.optarg);
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
  if (recv_protocols.length () == 0)
    {
      recv_protocols.length (1);
      recv_protocols[0] = CORBA::string_dup ("UDP=224.9.9.2:12345");
    }

  if (send_protocols.length () == 0)
    {
      send_protocols.length (1);
      send_protocols[0] = CORBA::string_dup ("UDP=224.9.9.2:23456");
    }

  // Indicates sucessful parsing of the command line
  return 0;
}

int main (int argc, char *argv[])
{
  ACE_TRY_NEW_ENV
    {
      TAO_AV_Core *av_core = TAO_AV_CORE::instance ();
      av_core->init (argc, argv, ACE_TRY_ENV);
      ACE_TRY_CHECK;

      parse_args (argc, argv);

      TAO_ORB_Manager* orb_manager =
        av_core->orb_manager ();

      CORBA::ORB_var orb = orb_manager->orb ();

      CORBA::Object_var poa_object =
        orb->resolve_initial_references("RootPOA", ACE_TRY_ENV);
      ACE_TRY_CHECK;

      PortableServer::POA_var root_poa =
        PortableServer::POA::_narrow (poa_object.in (), ACE_TRY_ENV);
      ACE_TRY_CHECK;

      PortableServer::POAManager_var poa_manager =
        root_poa->the_POAManager (ACE_TRY_ENV);
      ACE_TRY_CHECK;

      poa_manager->activate (ACE_TRY_ENV);
      ACE_TRY_CHECK;

      // Register the video mmdevice object with the ORB

      Reactive_Strategy reactive_strategy (orb_manager);
      TAO_MMDevice mmdevice_impl (&reactive_strategy);
      AVStreams::MMDevice_var mmdevice =
        mmdevice_impl._this (ACE_TRY_ENV);
      ACE_TRY_CHECK;

      CORBA::String_var ior =
        orb->object_to_string (mmdevice.in (), ACE_TRY_ENV);
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

      Ping_Recv_FDev ping_fdev_impl ("Ping");
      Pong_Send_FDev pong_fdev_impl ("Pong");

      AVStreams::FDev_var ping_fdev =
        ping_fdev_impl._this (ACE_TRY_ENV);
      ACE_TRY_CHECK;
      AVStreams::FDev_var pong_fdev =
        pong_fdev_impl._this (ACE_TRY_ENV);
      ACE_TRY_CHECK;

      mmdevice->add_fdev (ping_fdev.in (), ACE_TRY_ENV);
      ACE_TRY_CHECK;
      mmdevice->add_fdev (pong_fdev.in (), ACE_TRY_ENV);
      ACE_TRY_CHECK;

      if (orb->run () == -1)
        ACE_ERROR_RETURN ((LM_ERROR, "%p\n", "orb->run"), -1);
      ACE_DEBUG ((LM_DEBUG, "event loop finished\n"));

      root_poa->destroy (1, 1, ACE_TRY_ENV);
      ACE_TRY_CHECK;
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

// ****************************************************************

Ping_Recv::Ping_Recv (void)
  :  TAO_FlowConsumer ("Ping",
                       recv_protocols,
                       "UNS:ping")
{
}

int
Ping_Recv::get_callback (const char *,
                         TAO_AV_Callback *&callback)
{
  ACE_DEBUG ((LM_DEBUG,"Ping_Recv::get_callback\n"));
  callback = &this->callback_;
  return 0;
}

int
Ping_Recv_Callback::handle_stop (void)
{
  ACE_DEBUG ((LM_DEBUG,"Ping_Recv_Callback::stop"));
  return 0;
}

int
Ping_Recv_Callback::receive_frame (ACE_Message_Block *frame,
                                   TAO_AV_frame_info *,
                                   const ACE_Addr &)
{
  ACE_DEBUG ((LM_DEBUG,"Ping_Recv_Callback::receive_frame\n"));

  for (const ACE_Message_Block *i = frame;
       frame != 0;
       frame = frame->cont ())
    {
      ACE_hrtime_t stamp;

      if (frame->length () < sizeof(stamp))
        return 0;

      ACE_OS::memcpy (&stamp, frame->rd_ptr (), sizeof(stamp));

      pong_callback.send_response (stamp);
    }
  return 0;
}

int
Ping_Recv_Callback::handle_destroy (void)
{
  ACE_DEBUG ((LM_DEBUG,"Ping_Recv_Callback::destroy\n"));
  return 0;
}

// ****************************************************************

Pong_Send::Pong_Send (void)
  :  TAO_FlowProducer ("Pong",
                       send_protocols,
                       "UNS:pong")
{
}

int
Pong_Send::get_callback (const char *,
                         TAO_AV_Callback *&callback)
{
  ACE_DEBUG ((LM_DEBUG,"Pong_Send::get_callback\n"));
  callback = &pong_callback;
  return 0;
}

void
Pong_Send_Callback::get_timeout (ACE_Time_Value *&tv,
                                 void *&)
{
  // @@ ACE_NEW (tv, ACE_Time_Value (0, milliseconds * 1000));
  tv = 0;
}

int
Pong_Send_Callback::handle_timeout (void *arg)
{
  ACE_DEBUG ((LM_DEBUG, "pong timeout (ignored)\n"));
  return 0;
}

int
Pong_Send_Callback::handle_end_stream (void)
{
  return 0;
}

int
Pong_Send_Callback::send_response (ACE_hrtime_t stamp)
{
  ACE_DEBUG ((LM_DEBUG, "pong send response)\n"));

  ACE_hrtime_t buf[2];

  buf[0] = stamp;
  ACE_Message_Block mb (ACE_reinterpret_cast (char*,buf),
                        sizeof(buf));

  buf[1] = ACE_OS::gethrtime ();

  int result = this->protocol_object_->send_frame (&mb);
  if (result < 0)
    ACE_ERROR_RETURN ((LM_ERROR,
                       "FTP_Client_Flow_Handler::send - %p\n",
                       ""),
                      -1);

  return 0;
}

// ****************************************************************

// @@ TODO
#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)

#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)

#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */
