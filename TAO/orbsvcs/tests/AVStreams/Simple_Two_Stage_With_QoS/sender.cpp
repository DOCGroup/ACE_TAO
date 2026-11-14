#include "sender.h"

#include "tao/debug.h"
#include "ace/Get_Opt.h"
#include "ace/High_Res_Timer.h"
#include "ace/INET_Addr.h"
#include "ace/OS_NS_stdlib.h"
#include "ace/OS_NS_unistd.h"

#include <new>

CORBA::Boolean
Sender_StreamEndPoint::modify_QoS (AVStreams::streamQoS &new_qos,
                                   AVStreams::flowSpec const &the_flows)
{
  return this->change_qos (new_qos, the_flows) == 0;
}

int
Sender_StreamEndPoint::get_callback (char const *,
                                     TAO_AV_Callback *&callback)
{
  callback = &this->callback_;

  TAO_Negotiator * const negotiator = new (std::nothrow) TAO_Negotiator;
  if (negotiator == nullptr)
    return -1;

  PortableServer::ServantBase_var safe_negotiator = negotiator;
  AVStreams::Negotiator_var negotiator_obj = negotiator->_this ();
  this->set_negotiator (negotiator_obj.in ());
  return 0;
}

int
Sender_StreamEndPoint::set_protocol_object (char const *,
                                            TAO_AV_Protocol_Object *object)
{
  SENDER::instance ()->protocol_object (object);
  return 0;
}

void
Sender::protocol_object (TAO_AV_Protocol_Object *protocol_object)
{
  this->protocol_object_ = protocol_object;
}

int
Sender::parse_args (int argc,
                    ACE_TCHAR *argv[])
{
  ACE_Get_Opt opts (argc, argv, ACE_TEXT ("f:p:r:l:a:d"));
  int option {};
  while ((option = opts ()) != -1)
    {
      switch (option)
        {
        case 'f':
          this->options_.filename = opts.opt_arg ();
          break;
        case 'p':
          this->options_.protocol = ACE_TEXT_ALWAYS_CHAR (opts.opt_arg ());
#if defined (ACE_HAS_RAPI)
          if (this->options_.protocol != "QoS_UDP")
            ACE_ERROR_RETURN ((LM_ERROR,
                               "When rapi=1 protocol must be QoS_UDP\n"),
                              -1);
#else
          if (this->options_.protocol == "QoS_UDP")
            ACE_ERROR_RETURN ((LM_ERROR,
                               "When rapi=0 protocol must not be QoS_UDP\n"),
                              -1);
#endif
          break;
        case 'r':
          this->options_.frame_rate = ACE_OS::atoi (opts.opt_arg ());
          break;
        case 'l':
          this->options_.address = opts.opt_arg ();
          break;
        case 'a':
          this->options_.peer_address = opts.opt_arg ();
          break;
        case 'd':
          ++TAO_debug_level;
          break;
        default:
          ACE_ERROR_RETURN ((LM_ERROR, "Unknown option\n"), -1);
        }
    }

  if (this->options_.frame_rate <= 0)
    ACE_ERROR_RETURN ((LM_ERROR, "Frame rate must be greater than zero\n"), -1);
  return 0;
}

void
Sender::fill_qos (AVStreams::streamQoS &qos)
{
  this->peak_bandwidth_ += 100;
  ACE_DEBUG ((LM_DEBUG, "Sender::fill_qos %u\n", this->peak_bandwidth_));

  qos.length (1);
  qos[0].QoSType = CORBA::string_dup ("Data_Receiver");

#if defined (ACE_HAS_RAPI)
  qos[0].QoSParams.length (10);
  qos[0].QoSParams[0].property_name = CORBA::string_dup ("Service_Type");
  qos[0].QoSParams[0].property_value <<= CORBA::Short (1);
  qos[0].QoSParams[1].property_name = CORBA::string_dup ("Token_Rate");
  qos[0].QoSParams[1].property_value <<= CORBA::ULong (9200);
  qos[0].QoSParams[2].property_name = CORBA::string_dup ("Token_Bucket_Size");
  qos[0].QoSParams[2].property_value <<= CORBA::ULong (708);
  qos[0].QoSParams[3].property_name = CORBA::string_dup ("Peak_Bandwidth");
  qos[0].QoSParams[3].property_value <<= this->peak_bandwidth_;
  qos[0].QoSParams[4].property_name = CORBA::string_dup ("Latency");
  qos[0].QoSParams[4].property_value <<= CORBA::ULong {};
  qos[0].QoSParams[5].property_name = CORBA::string_dup ("Delay_Variation");
  qos[0].QoSParams[5].property_value <<= CORBA::ULong {};
  qos[0].QoSParams[6].property_name = CORBA::string_dup ("Max_SDU_Size");
  qos[0].QoSParams[6].property_value <<= CORBA::ULong (368);
  qos[0].QoSParams[7].property_name = CORBA::string_dup ("Minimum_Policed_Size");
  qos[0].QoSParams[7].property_value <<= CORBA::ULong (368);
  qos[0].QoSParams[8].property_name = CORBA::string_dup ("TTL");
  qos[0].QoSParams[8].property_value <<= CORBA::ULong (25);
  qos[0].QoSParams[9].property_name = CORBA::string_dup ("Priority");
  qos[0].QoSParams[9].property_value <<= CORBA::ULong (1);
#else
  qos[0].QoSParams.length (1);
  qos[0].QoSParams[0].property_name = CORBA::string_dup ("Diffserv_Codepoint");
  qos[0].QoSParams[0].property_value <<= CORBA::Long (63);
#endif
}

int
Sender::bind_to_receiver ()
{
  CosNaming::Name name (1);
  name.length (1);
  name[0].id = CORBA::string_dup ("Receiver");

  CORBA::Object_var receiver_mmdevice_obj = this->naming_client_->resolve (name);
  this->receiver_mmdevice_ = AVStreams::MMDevice::_narrow (receiver_mmdevice_obj.in ());
  if (CORBA::is_nil (this->receiver_mmdevice_.in ()))
    ACE_ERROR_RETURN ((LM_ERROR,
                       "Could not resolve Receiver_MMdevice in Naming service\n"),
                      -1);
  return 0;
}

int
Sender::init (int argc,
              ACE_TCHAR *argv[])
{
  if (this->parse_args (argc, argv) != 0)
    return -1;

  if (this->endpoint_strategy_.init (TAO_AV_CORE::instance ()->orb (),
                                     TAO_AV_CORE::instance ()->poa ()) != 0 ||
      this->naming_client_.init (TAO_AV_CORE::instance ()->orb ()) != 0)
    return -1;

  this->input_file_.reset (ACE_OS::fopen (this->options_.filename.c_str (), "rb"));
  if (!this->input_file_)
    ACE_ERROR_RETURN ((LM_ERROR,
                       ACE_TEXT ("Cannot open input file %s\n"),
                       this->options_.filename.c_str ()),
                      -1);

  if (this->bind_to_receiver () != 0)
    ACE_ERROR_RETURN ((LM_ERROR, "Error binding to the naming service\n"), -1);

  ACE_TCHAR hostname[BUFSIZ] {};
  if (ACE_OS::hostname (hostname, BUFSIZ) != 0)
    ACE_ERROR_RETURN ((LM_ERROR, "Unable to get local hostname\n"), -1);

  ACE_INET_Addr local_address;
  if (this->options_.address.empty ())
    {
      if (local_address.set (5000, hostname, 1, AF_INET) != 0)
        ACE_ERROR_RETURN ((LM_ERROR, "Unable to set local address\n"), -1);
    }
  else if (local_address.set (this->options_.address.c_str ()) != 0)
    ACE_ERROR_RETURN ((LM_ERROR, "Invalid local address\n"), -1);

  ACE_INET_Addr peer_address;
  if (this->options_.peer_address.empty ())
    {
      if (peer_address.set (5050, hostname, 1, AF_INET) != 0)
        ACE_ERROR_RETURN ((LM_ERROR, "Unable to set peer address\n"), -1);
    }
  else if (peer_address.set (this->options_.peer_address.c_str ()) != 0)
    ACE_ERROR_RETURN ((LM_ERROR, "Invalid peer address\n"), -1);

  // The flow entry borrows both addresses until bind_devs has serialized it.
  char const * const flowname = "Data_Receiver";
  TAO_Forward_FlowSpec_Entry entry (flowname,
                                    "IN",
                                    "USER_DEFINED",
                                    "",
                                    this->options_.protocol.c_str (),
                                    &local_address);
  entry.set_peer_addr (&peer_address);

  AVStreams::flowSpec flow_spec (1);
  flow_spec.length (1);
  flow_spec[0] = CORBA::string_dup (entry.entry_to_string ());

  TAO_MMDevice * const sender_mmdevice =
    new (std::nothrow) TAO_MMDevice (&this->endpoint_strategy_);
  if (sender_mmdevice == nullptr)
    ACE_ERROR_RETURN ((LM_ERROR, "Unable to allocate sender MMDevice\n"), -1);
  PortableServer::ServantBase_var safe_mmdevice = sender_mmdevice;
  AVStreams::MMDevice_var mmdevice = sender_mmdevice->_this ();

  this->streamctrl_ = new (std::nothrow) TAO_StreamCtrl;
  if (this->streamctrl_ == nullptr)
    ACE_ERROR_RETURN ((LM_ERROR, "Unable to allocate stream controller\n"), -1);
  this->streamctrl_owner_ = this->streamctrl_;

  AVStreams::streamQoS qos;
  if (!this->streamctrl_->bind_devs (mmdevice.in (),
                                     this->receiver_mmdevice_.in (),
                                     qos,
                                     flow_spec))
    ACE_ERROR_RETURN ((LM_ERROR, "streamctrl::bind_devs failed\n"), -1);
  return 0;
}

int
Sender::pace_data ()
{
  this->inter_frame_time_.set (1.0 / this->options_.frame_rate);
  if (TAO_debug_level > 0)
    ACE_DEBUG ((LM_DEBUG,
                "Frame Rate = %f / second\nInter Frame Time = %d (msec)\n",
                this->options_.frame_rate,
                this->inter_frame_time_.msec ()));

  try
    {
      ACE_High_Res_Timer elapsed_timer;
      for (;;)
        {
          this->mb_.reset ();
          size_t const bytes_read = ACE_OS::fread (this->mb_.wr_ptr (),
                                                   1,
                                                   this->mb_.size (),
                                                   this->input_file_.get ());
          if (bytes_read == 0)
            {
              if (std::ferror (this->input_file_.get ()))
                ACE_ERROR_RETURN ((LM_ERROR, "Sender::pace_data fread failed\n"), -1);
              break;
            }
          this->mb_.wr_ptr (bytes_read);

          if (this->frame_count_ > 0)
            {
              elapsed_timer.stop ();
              ACE_Time_Value elapsed_time;
              elapsed_timer.elapsed_time (elapsed_time);
              if (elapsed_time < this->inter_frame_time_)
                {
                  ACE_Time_Value wait_time (this->inter_frame_time_ - elapsed_time);
                  TAO_AV_CORE::instance ()->orb ()->run (wait_time);
                }
            }

          elapsed_timer.start ();
          if (this->protocol_object_ == nullptr ||
              this->protocol_object_->send_frame (&this->mb_) < 0)
            ACE_ERROR_RETURN ((LM_ERROR, "Sender::pace_data send failed\n"), -1);

          ++this->frame_count_;
          ACE_DEBUG ((LM_DEBUG,
                      "Sender::pace_data frame %d was sent successfully\n",
                      this->frame_count_));

          if (this->frame_count_ == 2)
            {
              TAO_Forward_FlowSpec_Entry entry ("Data_Receiver",
                                                "IN",
                                                "USER_DEFINED",
                                                "",
                                                this->options_.protocol.c_str (),
                                                nullptr);
              AVStreams::flowSpec flow_spec (1);
              flow_spec.length (1);
              flow_spec[0] = CORBA::string_dup (entry.entry_to_string ());

              AVStreams::streamQoS qos;
              this->fill_qos (qos);
              this->streamctrl_->modify_QoS (qos, flow_spec);
            }
        }

      AVStreams::flowSpec stop_spec;
      this->streamctrl_->destroy (stop_spec);
      TAO_AV_CORE::instance ()->orb ()->shutdown (false);
    }
  catch (CORBA::Exception const &ex)
    {
      ex._tao_print_exception ("Sender::pace_data failed");
      return -1;
    }
  return 0;
}

int
ACE_TMAIN (int argc,
           ACE_TCHAR *argv[])
{
  CORBA::ORB_var orb;
  int result = -1;
  try
    {
      orb = CORBA::ORB_init (argc, argv);
      CORBA::Object_var obj = orb->resolve_initial_references ("RootPOA");
      PortableServer::POA_var root_poa = PortableServer::POA::_narrow (obj.in ());
      PortableServer::POAManager_var manager = root_poa->the_POAManager ();
      manager->activate ();

      TAO_AV_CORE::instance ()->init (orb.in (), root_poa.in ());
      Sender * const sender = SENDER::instance ();
      result = sender->init (argc, argv);
      if (result == 0)
        result = sender->pace_data ();
    }
  catch (CORBA::Exception const &ex)
    {
      ex._tao_print_exception ("Sender");
      result = -1;
    }

  SENDER::close ();
  if (!CORBA::is_nil (orb.in ()))
    orb->destroy ();
  return result;
}

#if defined (ACE_HAS_EXPLICIT_STATIC_TEMPLATE_MEMBER_INSTANTIATION)
template ACE_Unmanaged_Singleton<Sender, ACE_Null_Mutex> *
ACE_Unmanaged_Singleton<Sender, ACE_Null_Mutex>::singleton_;
#endif /* ACE_HAS_EXPLICIT_STATIC_TEMPLATE_MEMBER_INSTANTIATION */
