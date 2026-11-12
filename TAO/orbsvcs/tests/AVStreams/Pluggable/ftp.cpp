#include "ftp.h"

#include "tao/debug.h"
#include "ace/High_Res_Timer.h"
#include "ace/OS_NS_stdio.h"

#include <cstddef>

FTP_Client_StreamEndPoint::FTP_Client_StreamEndPoint () = default;

int
FTP_Client_StreamEndPoint::get_callback (char const *,
                                         TAO_AV_Callback *&callback)
{
  callback = &this->callback_;
  return 0;
}

int
FTP_Client_StreamEndPoint::set_protocol_object (char const *,
                                                TAO_AV_Protocol_Object *object)
{
  CLIENT::instance ()->set_protocol_object (object);
  return 0;
}

Client::Client ()
  : client_mmdevice_ (&this->endpoint_strategy_)
{
}

void
Client::set_protocol_object (TAO_AV_Protocol_Object *protocol_object)
{
  this->protocol_object_ = protocol_object;
}

int
Client::parse_args (int argc,
                    ACE_TCHAR *argv[])
{
  ACE_Get_Opt opts (argc, argv, ACE_TEXT ("f:l:a:p:r:sd"));
  int option {};
  while ((option = opts ()) != -1)
    {
      switch (option)
        {
        case 'f':
          this->options_.filename = opts.opt_arg ();
          break;
        case 'l':
          this->options_.address = ACE_TEXT_ALWAYS_CHAR (opts.opt_arg ());
          break;
        case 'a':
          this->options_.peer_address = ACE_TEXT_ALWAYS_CHAR (opts.opt_arg ());
          break;
        case 'p':
          this->options_.protocol = ACE_TEXT_ALWAYS_CHAR (opts.opt_arg ());
          break;
        case 'r':
          this->options_.frame_rate = ACE_OS::atoi (opts.opt_arg ());
          break;
        case 's':
          this->options_.use_sfp = true;
          break;
        case 'd':
          ++TAO_debug_level;
          break;
        default:
          ACE_ERROR ((LM_ERROR, "Unknown option\n"));
          return -1;
        }
    }
  return this->options_.frame_rate > 0 ? 0 : -1;
}

int
Client::bind_to_server ()
{
  if (this->my_naming_client_.init (TAO_AV_CORE::instance ()->orb ()) != 0)
    ACE_ERROR_RETURN ((LM_ERROR,
                       " (%P|%t) Unable to initialize the TAO_Naming_Client.\n"),
                      -1);

  CosNaming::Name server_mmdevice_name (1);
  server_mmdevice_name.length (1);
  server_mmdevice_name[0].id = CORBA::string_dup ("Server_MMDevice");

  CORBA::Object_var server_mmdevice_obj =
    this->my_naming_client_->resolve (server_mmdevice_name);
  this->server_mmdevice_ = AVStreams::MMDevice::_narrow (server_mmdevice_obj.in ());

  if (CORBA::is_nil (this->server_mmdevice_.in ()))
    ACE_ERROR_RETURN ((LM_ERROR,
                       "Could not resolve Server_MMdevice in Naming service\n"),
                      -1);

  return 0;
}

int
Client::init (int argc,
              ACE_TCHAR *argv[])
{
  this->endpoint_strategy_.init (TAO_AV_CORE::instance ()->orb (),
                                 TAO_AV_CORE::instance ()->poa ());

  if (this->parse_args (argc, argv) != 0)
    return -1;

  this->fp_.reset (ACE_OS::fopen (this->options_.filename.c_str (), "rb"));
  if (!this->fp_)
    ACE_ERROR_RETURN ((LM_ERROR,
                       "Cannot open input file %s\n",
                       ACE_TEXT_ALWAYS_CHAR (this->options_.filename.c_str ())),
                      -1);

  if (this->bind_to_server () != 0)
    ACE_ERROR_RETURN ((LM_ERROR,
                       "(%P|%t) Error binding to the naming service\n"),
                      -1);

  std::string const flow_protocol = this->options_.use_sfp ? "sfp:1.0" : "";
  AVStreams::streamQoS_var the_qos (new AVStreams::streamQoS);

  ACE_INET_Addr addr;
  if (!this->options_.address.empty ())
    {
      if (addr.set (this->options_.address.c_str ()) != 0)
        ACE_ERROR_RETURN ((LM_ERROR, "Invalid local address\n"), -1);
    }
  else
    {
      char hostname[BUFSIZ] {};
      if (ACE_OS::hostname (hostname, sizeof hostname) != 0 ||
          addr.set ("5000", hostname) != 0)
        ACE_ERROR_RETURN ((LM_ERROR, "Unable to determine local address\n"), -1);
    }

  std::string const flowname = "Data_" + this->options_.protocol;
  TAO_Forward_FlowSpec_Entry entry (flowname.c_str (),
                                    "IN",
                                    "USER_DEFINED",
                                    flow_protocol.c_str (),
                                    this->options_.protocol.c_str (),
                                    &addr);

  ACE_INET_Addr peer_addr;
  if (!this->options_.peer_address.empty ())
    {
      if (peer_addr.set (this->options_.peer_address.c_str ()) != 0)
        ACE_ERROR_RETURN ((LM_ERROR, "Invalid peer address\n"), -1);
    }
  else
    {
      char hostname[BUFSIZ] {};
      if (ACE_OS::hostname (hostname, sizeof hostname) != 0 ||
          peer_addr.set ("5050", hostname) != 0)
        ACE_ERROR_RETURN ((LM_ERROR, "Unable to determine peer address\n"), -1);
    }
  entry.set_peer_addr (&peer_addr);

  AVStreams::flowSpec flow_spec (1);
  flow_spec.length (1);
  flow_spec[0] = CORBA::string_dup (entry.entry_to_string ());

  AVStreams::MMDevice_var client_mmdevice = this->client_mmdevice_._this ();
  CORBA::Boolean bind_result =
    this->streamctrl_.bind_devs (client_mmdevice.in (),
                                 this->server_mmdevice_.in (),
                                 the_qos.inout (),
                                 flow_spec);
  if (!bind_result)
    ACE_ERROR_RETURN ((LM_ERROR, "streamctrl::bind_devs failed\n"), -1);

  return 0;
}

int
Client::pace_data ()
{
  if (this->options_.frame_rate <= 0)
    ACE_ERROR_RETURN ((LM_ERROR, "Frame rate must be positive\n"), -1);
  if (this->protocol_object_ == nullptr)
    ACE_ERROR_RETURN ((LM_ERROR, "No protocol object was configured\n"), -1);

  double const frame_time = 1.0 / this->options_.frame_rate;
  ACE_Time_Value const inter_frame_time (frame_time);
  ACE_High_Res_Timer last_frame_sent_time;

  if (TAO_debug_level > 0)
    ACE_DEBUG ((LM_DEBUG,
                "Frame Time = %f\nFrame Rate = %d\n",
                frame_time,
                this->options_.frame_rate));

  try
    {
      for (;;)
        {
          this->mb_.reset ();
          size_t const bytes_read = std::fread (this->mb_.wr_ptr (),
                                                     1,
                                                     this->mb_.size (),
                                                     this->fp_.get ());
          if (bytes_read == 0)
            {
              if (std::ferror (this->fp_.get ()))
                ACE_ERROR_RETURN ((LM_ERROR, "Error reading input file\n"), -1);
              break;
            }
          this->mb_.wr_ptr (bytes_read);

          if (this->count_ > 0)
            {
              last_frame_sent_time.stop ();
              ACE_Time_Value elapsed;
              last_frame_sent_time.elapsed_time (elapsed);
              if (elapsed < inter_frame_time)
                {
                  ACE_Time_Value wait_time (inter_frame_time - elapsed);
                  TAO_AV_CORE::instance ()->orb ()->run (wait_time);
                }
            }

          last_frame_sent_time.start ();
          if (this->protocol_object_->send_frame (&this->mb_) < 0)
            ACE_ERROR_RETURN ((LM_ERROR,
                               "send failed:%p",
                               "FTP_Client_Flow_Handler::send\n"),
                              -1);

          ++this->count_;
          if (TAO_debug_level > 0)
            ACE_DEBUG ((LM_DEBUG, "Client::pace_data buffer sent successfully\n"));
        }

      AVStreams::flowSpec stop_spec (1);
      this->streamctrl_.destroy (stop_spec);
      TAO_AV_CORE::instance ()->orb ()->shutdown (false);
    }
  catch (CORBA::Exception const &ex)
    {
      ex._tao_print_exception ("Client::pace_data failed");
      return -1;
    }

  return 0;
}

int
ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
  int result {};
  try
    {
      CORBA::ORB_var orb = CORBA::ORB_init (argc, argv);
      CORBA::Object_var obj = orb->resolve_initial_references ("RootPOA");
      PortableServer::POA_var root_poa = PortableServer::POA::_narrow (obj.in ());
      PortableServer::POAManager_var mgr = root_poa->the_POAManager ();
      mgr->activate ();

      TAO_AV_CORE::instance ()->init (orb.in (), root_poa.in ());
      Client * const client = CLIENT::instance ();
      if (client->init (argc, argv) != 0)
        {
          ACE_ERROR ((LM_ERROR, "client::init failed\n"));
          result = 1;
        }
      else if (client->pace_data () != 0)
        {
          ACE_ERROR ((LM_ERROR, "client::pace_data failed\n"));
          result = 1;
        }

      CLIENT::close ();
      orb->destroy ();
    }
  catch (CORBA::Exception const &ex)
    {
      ex._tao_print_exception ("Client failed");
      CLIENT::close ();
      return -1;
    }

  return result;
}

#if defined (ACE_HAS_EXPLICIT_STATIC_TEMPLATE_MEMBER_INSTANTIATION)
template ACE_Unmanaged_Singleton<Client, ACE_Null_Mutex> *
ACE_Unmanaged_Singleton<Client, ACE_Null_Mutex>::singleton_;
#endif /* ACE_HAS_EXPLICIT_STATIC_TEMPLATE_MEMBER_INSTANTIATION */
