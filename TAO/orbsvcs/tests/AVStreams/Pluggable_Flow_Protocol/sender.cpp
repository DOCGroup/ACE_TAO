#include "sender.h"

#include "tao/debug.h"
#include "ace/Get_Opt.h"
#include "ace/High_Res_Timer.h"

#include <cstddef>
#include <cstdio>
#include <new>

int
Sender_StreamEndPoint::get_callback (char const *,
                                     TAO_AV_Callback *&callback)
{
  callback = &this->callback_;
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
  ACE_Get_Opt opts (argc, argv, ACE_TEXT ("f:p:r:d"));
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
          break;
        case 'r':
          this->options_.frame_rate = ACE_OS::atoi (opts.opt_arg ());
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
Sender::bind_to_receiver ()
{
  CosNaming::Name name (1);
  name.length (1);
  name[0].id = CORBA::string_dup ("Receiver");

  CORBA::Object_var receiver_mmdevice_obj = this->naming_client_->resolve (name);
  this->receiver_mmdevice_ =
    AVStreams::MMDevice::_narrow (receiver_mmdevice_obj.in ());
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
    ACE_ERROR_RETURN ((LM_ERROR,
                       "(%P|%t) Error binding to the naming service\n"),
                      -1);

  AVStreams::streamQoS_var the_qos (new AVStreams::streamQoS);
  TAO_Forward_FlowSpec_Entry entry ("Data_Receiver",
                                    "IN",
                                    "USER_DEFINED",
                                    "TS",
                                    this->options_.protocol.c_str (),
                                    nullptr);
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
  this->streamctrl_servant_ = this->streamctrl_;

  CORBA::Boolean const bind_result =
    this->streamctrl_->bind_devs (mmdevice.in (),
                                  this->receiver_mmdevice_.in (),
                                  the_qos.inout (),
                                  flow_spec);
  if (!bind_result)
    ACE_ERROR_RETURN ((LM_ERROR, "streamctrl::bind_devs failed\n"), -1);
  return 0;
}

int
Sender::pace_data ()
{
  if (this->protocol_object_ == nullptr)
    ACE_ERROR_RETURN ((LM_ERROR, "No flow protocol object is available\n"), -1);

  ACE_Time_Value inter_frame_time;
  inter_frame_time.set (1.0 / this->options_.frame_rate);
  if (TAO_debug_level > 0)
    ACE_DEBUG ((LM_DEBUG,
                "Frame Rate = %d / second\nInter Frame Time = %d (msec)\n",
                this->options_.frame_rate,
                inter_frame_time.msec ()));

  try
    {
      ACE_High_Res_Timer elapsed_timer;
      for (;;)
        {
          this->mb_.reset ();
          size_t const bytes_read = std::fread (this->mb_.wr_ptr (),
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
              if (elapsed_time < inter_frame_time)
                {
                  ACE_Time_Value wait_time (inter_frame_time - elapsed_time);
                  TAO_AV_CORE::instance ()->orb ()->run (wait_time);
                }
            }

          elapsed_timer.start ();
          if (this->protocol_object_->send_frame (&this->mb_) < 0)
            ACE_ERROR_RETURN ((LM_ERROR,
                               "send failed:%p",
                               "Sender::pace_data send\n"),
                              -1);

          ++this->frame_count_;
          if (TAO_debug_level > 0)
            ACE_DEBUG ((LM_DEBUG,
                        "Sender::pace_data frame %B was sent successfully\n",
                        this->frame_count_));
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
  ACE_High_Res_Timer::global_scale_factor ();
  int result = -1;
  try
    {
      CORBA::ORB_var orb = CORBA::ORB_init (argc, argv);
      CORBA::Object_var obj = orb->resolve_initial_references ("RootPOA");
      PortableServer::POA_var root_poa = PortableServer::POA::_narrow (obj.in ());
      PortableServer::POAManager_var manager = root_poa->the_POAManager ();
      manager->activate ();

      TAO_AV_CORE::instance ()->init (orb.in (), root_poa.in ());
      Sender * const sender = SENDER::instance ();
      result = sender->init (argc, argv);
      if (result == 0)
        result = sender->pace_data ();
      orb->destroy ();
    }
  catch (CORBA::Exception const &ex)
    {
      ex._tao_print_exception ("Sender");
      result = -1;
    }

  SENDER::close ();
  return result;
}

#if defined (ACE_HAS_EXPLICIT_STATIC_TEMPLATE_MEMBER_INSTANTIATION)
template ACE_Unmanaged_Singleton<Sender, ACE_Null_Mutex> *
ACE_Unmanaged_Singleton<Sender, ACE_Null_Mutex>::singleton_;
#endif /* ACE_HAS_EXPLICIT_STATIC_TEMPLATE_MEMBER_INSTANTIATION */
