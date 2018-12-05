#include "sender.h"
#include "tao/debug.h"
#include "ace/Get_Opt.h"
#include "ace/High_Res_Timer.h"

static u_long peak_bandwidth = 18400;

typedef ACE_Unmanaged_Singleton<Sender, ACE_Null_Mutex> SENDER;
// Create a singleton instance of the Sender.

// The time that should lapse between two consecutive frames sent.
ACE_Time_Value inter_frame_time;

CORBA::Boolean
Sender_StreamEndPoint::modify_QoS (AVStreams::streamQoS &new_qos,
                                   const AVStreams::flowSpec &the_flows)
{
  ACE_DEBUG ((LM_DEBUG,
              "Sender_StreamEndPoint::modify_QoS\n"));

  int result =  this->change_qos (new_qos, the_flows);

  if (result != 0)
    return 0;

  return 1;
}

int
Sender_StreamEndPoint::get_callback (const char *,
                                     TAO_AV_Callback *&callback)
{
  // Create and return the sender application callback to AVStreams
  // for further upcalls.
  callback = &this->callback_;

  TAO_Negotiator *negotiator;
  ACE_NEW_RETURN (negotiator,
                  TAO_Negotiator,
                  -1);
  AVStreams::Negotiator_var negotiator_obj =
    negotiator->_this ();

  this->set_negotiator (negotiator_obj.in ());

  return 0;
}

int
Sender_StreamEndPoint::set_protocol_object (const char *,
                                            TAO_AV_Protocol_Object *object)
{
  // Set the sender protocol object corresponding to the transport
  // protocol selected.
  SENDER::instance ()->protocol_object (object);
  return 0;
}

Sender::Sender (void)
  : sender_mmdevice_ (0),
    streamctrl_ (0),
    frame_count_ (0),
    filename_ ("input"),
    input_file_ (0),
    protocol_ ("UDP"),
    frame_rate_ (1.0),
    mb_ (BUFSIZ),
    address_ (0),
    peer_addr_ (0)
{
}

void
Sender::protocol_object (TAO_AV_Protocol_Object *object)
{
  // Set the sender protocol object corresponding to the transport
  // protocol selected.
  this->protocol_object_ = object;
}

int
Sender::parse_args (int argc,
                    ACE_TCHAR *argv[])
{
  // Parse command line arguments
  ACE_Get_Opt opts (argc, argv, ACE_TEXT("f:p:r:l:a:d"));

  int c;
  while ((c= opts ()) != -1)
    {
      switch (c)
        {
        case 'f':
          this->filename_ = ACE_TEXT_ALWAYS_CHAR (opts.opt_arg ());
          break;
        case 'p':
            this->protocol_ = ACE_TEXT_ALWAYS_CHAR (opts.opt_arg ());
#ifdef ACE_HAS_RAPI
            if (this->protocol_ != ACE_CString ("QoS_UDP"))
              ACE_ERROR_RETURN ((LM_ERROR,
                                 "When rapi=1 protocol must be QoS_UDP\n"),
                                -1);
#else
            if (this->protocol_ == ACE_CString ("QoS_UDP"))
              ACE_ERROR_RETURN ((LM_ERROR,
                                 "When rapi=0 protocol must not be QoS_UDP\n"),
                                -1);
#endif //ACE_HAS_RAPI
            break;
        case 'r':
          this->frame_rate_ = (double)ACE_OS::atoi (opts.opt_arg ());
          break;
        case 'd':
          TAO_debug_level++;
          break;
        case 'l':
          this->address_ = opts.opt_arg ();
          break;
        case 'a':
          this->peer_addr_ = opts.opt_arg ();
          break;
        default:
          ACE_DEBUG ((LM_DEBUG, "Unknown Option\n"));
          return -1;
        }
    }
  return 0;
}

void
Sender::fill_qos (AVStreams::streamQoS &qos)
{
  peak_bandwidth += 100;

  ACE_DEBUG ((LM_DEBUG,
              "Sender::fill_qos %d\n",
              peak_bandwidth));

  qos.length (1);
  qos [0].QoSType =  CORBA::string_dup ("Data_Receiver");

#ifdef ACE_HAS_RAPI
  qos [0].QoSParams.length (10);

  qos [0].QoSParams [0].property_name = CORBA::string_dup ("Service_Type");
  qos [0].QoSParams [0].property_value <<= (CORBA::Short) 1;//ACE_SERVICETYPE_CONTROLLEDLOAD;

  qos [0].QoSParams [1].property_name = CORBA::string_dup ("Token_Rate");
  qos [0].QoSParams [1].property_value <<= (CORBA::ULong) 9200 ;

  qos [0].QoSParams [2].property_name = CORBA::string_dup ("Token_Bucket_Size");
  qos [0].QoSParams [2].property_value <<= (CORBA::ULong) 708;

  qos [0].QoSParams [3].property_name = CORBA::string_dup ("Peak_Bandwidth");
  qos [0].QoSParams [3].property_value <<= (CORBA::ULong) peak_bandwidth;


  qos [0].QoSParams [4].property_name = CORBA::string_dup ("Latency");
  qos [0].QoSParams [4].property_value <<= (CORBA::ULong) 0;

  qos [0].QoSParams [5].property_name = CORBA::string_dup ("Delay_Variation");
  qos [0].QoSParams [5].property_value <<= (CORBA::ULong) 0;

  qos [0].QoSParams [6].property_name = CORBA::string_dup ("Max_SDU_Size");
  qos [0].QoSParams [6].property_value <<= (CORBA::ULong) 368;

  qos [0].QoSParams [7].property_name = CORBA::string_dup ("Minimum_Policed_Size");
  qos [0].QoSParams [7].property_value <<= (CORBA::ULong) 368;

  qos [0].QoSParams [8].property_name = CORBA::string_dup ("TTL");
  qos [0].QoSParams [8].property_value <<= (CORBA::ULong) 25;

  qos [0].QoSParams [9].property_name = CORBA::string_dup ("Priority");
  qos [0].QoSParams [9].property_value <<= (CORBA::ULong) 1;
#else
  qos [0].QoSParams.length (1);
  qos [0].QoSParams [0].property_name = CORBA::string_dup ("Diffserv_Codepoint");
  qos [0].QoSParams [0].property_value <<= (CORBA::Long) 63;
#endif //ACE_HAS_RAPI


}

// Method to get the object reference of the receiver
int
Sender::bind_to_receiver (void)
{
  CosNaming::Name name (1);
  name.length (1);
  name [0].id =
    CORBA::string_dup ("Receiver");

  // Resolve the receiver object reference from the Naming Service
  CORBA::Object_var receiver_mmdevice_obj =
    this->naming_client_->resolve (name);

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
  // Initialize the endpoint strategy with the orb and poa.
  int result =
    this->endpoint_strategy_.init (TAO_AV_CORE::instance ()->orb (),
                                   TAO_AV_CORE::instance ()->poa ());
  if (result != 0)
    return result;

  // Initialize the naming services
  result =
    this->naming_client_.init (TAO_AV_CORE::instance ()->orb ());
  if (result != 0)
    return result;

  // Parse the command line arguments
  result =
    this->parse_args (argc,
                      argv);
  if (result != 0)
    return result;

  // Open file to read.
  this->input_file_ =
    ACE_OS::fopen (this->filename_.c_str (),
                   "r");

  if (this->input_file_ == 0)
    ACE_ERROR_RETURN ((LM_DEBUG,
                       "Cannot open input file %C\n",
                       this->filename_.c_str ()),
                      -1);
  else
    ACE_DEBUG ((LM_DEBUG,
                "File opened successfully\n"));

  // Resolve the object reference of the receiver from the Naming Service.
  result = this->bind_to_receiver ();

  if (result != 0)
    ACE_ERROR_RETURN ((LM_ERROR,
                       "(%P|%t) Error binding to the naming service\n"),
                      -1);

  this->flowname_ = "Data_Receiver";

  // Set the address of the ftp client.
  ACE_INET_Addr* addr;
  if (this->address_ != 0)
    ACE_NEW_RETURN (addr,
                    ACE_INET_Addr (this->address_),
                    -1);
  else
    {
      char buf [BUFSIZ];
      ACE_OS::hostname (buf,
                        BUFSIZ);
      ACE_NEW_RETURN (addr,
                      ACE_INET_Addr ("5000",
                                     buf),
                      -1);
    }

  // Create the forward flow specification to describe the flow.
  TAO_Forward_FlowSpec_Entry entry (this->flowname_.c_str (),
                                    "IN",
                                    "USER_DEFINED",
                                    "",
                                    this->protocol_.c_str (),
                                    addr);

  ACE_INET_Addr* peer_addr;
  if (this->peer_addr_ != 0)
    ACE_NEW_RETURN (peer_addr,
                    ACE_INET_Addr (this->peer_addr_),
                    -1);
  else
    {
      char buf [BUFSIZ];
      ACE_OS::hostname (buf,
                        BUFSIZ);
      ACE_NEW_RETURN (peer_addr,
                      ACE_INET_Addr ("5050",
                                     buf),
                      -1);
    }

  entry.set_peer_addr (peer_addr);

  AVStreams::flowSpec flow_spec (1);
  flow_spec.length (1);
  flow_spec [0] = CORBA::string_dup (entry.entry_to_string ());

  // Register the sender mmdevice object with the ORB
  ACE_NEW_RETURN (this->sender_mmdevice_,
                  TAO_MMDevice (&this->endpoint_strategy_),
                  -1);

  // Servant Reference Counting to manage lifetime
  PortableServer::ServantBase_var safe_mmdevice =
    this->sender_mmdevice_;

  AVStreams::MMDevice_var mmdevice =
    this->sender_mmdevice_->_this ();

  ACE_NEW_RETURN (this->streamctrl_,
                  TAO_StreamCtrl,
                  -1);

  PortableServer::ServantBase_var safe_streamctrl =
    this->streamctrl_;

  AVStreams::streamQoS qos;

  //this->fill_qos (qos);

  // Bind/Connect the sender and receiver MMDevices.
  CORBA::Boolean bind_result =
    this->streamctrl_->bind_devs (mmdevice.in (),
                                  this->receiver_mmdevice_.in (),
                                  qos,
                                  flow_spec);

  if (bind_result == 0)
    ACE_ERROR_RETURN ((LM_ERROR,
                       "streamctrl::bind_devs failed\n"),
                      -1);

  return 0;
}

// Method to send data at the specified rate
int
Sender::pace_data (void)
{


  // The time between two consecutive frames.
  inter_frame_time.set (1.0 / this->frame_rate_);

  if (TAO_debug_level > 0)
    ACE_DEBUG ((LM_DEBUG,
                "Frame Rate = %f / second\n"
                "Inter Frame Time = %d (msec)\n",
                this->frame_rate_,
                inter_frame_time.msec ()));

  try
    {
      // The time taken for sending a frame and preparing for the next frame
      ACE_High_Res_Timer elapsed_timer;

      // Continue to send data till the file is read to the end.
      while (1)
        {
          // Read from the file into a message block.
          int n = ACE_OS::fread (this->mb_.wr_ptr (),
                                 1,
                                 this->mb_.size (),
                                 this->input_file_);

          if (n < 0)
            ACE_ERROR_RETURN ((LM_ERROR,
                               "Sender::pace_data fread failed\n"),
                              -1);

          if (n == 0)
            {
              // At end of file break the loop and end the sender.
              if (TAO_debug_level > 0)
                ACE_DEBUG ((LM_DEBUG,"Handle_Start:End of file\n"));
              break;
            }

          this->mb_.wr_ptr (n);

          if (this->frame_count_ > 1)
            {
              //
              // Second frame and beyond
              //

              // Stop the timer that was started just before the previous frame was sent.
              elapsed_timer.stop ();

              // Get the time elapsed after sending the previous frame.
              ACE_Time_Value elapsed_time;
              elapsed_timer.elapsed_time (elapsed_time);

              if (TAO_debug_level > 0)
                ACE_DEBUG ((LM_DEBUG,
                            "Elapsed Time = %d\n",
                            elapsed_time.msec ()));

              // Check to see if the inter frame time has elapsed.
              if (elapsed_time < inter_frame_time)
                {
                  // Inter frame time has not elapsed.

                  // Calculate the time to wait before the next frame needs to be sent.
                  ACE_Time_Value wait_time (inter_frame_time - elapsed_time);

                  if (TAO_debug_level > 0)
                    ACE_DEBUG ((LM_DEBUG,
                                "Wait Time = %d\n",
                                wait_time.msec ()));

                  // Run the orb for the wait time so the sender can
                  // continue other orb requests.
                  TAO_AV_CORE::instance ()->orb ()->run (wait_time);
                }
            }

          // Start timer before sending the frame.
          elapsed_timer.start ();

          // Send frame.
          int result =
            this->protocol_object_->send_frame (&this->mb_);

          if (result < 0)
            ACE_ERROR_RETURN ((LM_ERROR,
                               "send failed:%p",
                               "Sender::pace_data send\n"),
                              -1);

          ACE_DEBUG ((LM_DEBUG,
                      " Sender::pace_data frame %d was sent successfully\n",
                      ++this->frame_count_));

          // Reset the message block.
          this->mb_.reset ();

          if (this->frame_count_ == 2)
            {
              TAO_Forward_FlowSpec_Entry entry (this->flowname_.c_str (),
                                                "IN",
                                                "USER_DEFINED",
                                                "",
                                                this->protocol_.c_str (),
                                                0);
              AVStreams::flowSpec flow_spec (1);
              flow_spec.length (1);
              flow_spec [0] = CORBA::string_dup (entry.entry_to_string ());

              AVStreams::streamQoS qos;
              this->fill_qos (qos);
              this->streamctrl_->modify_QoS (qos,
                                             flow_spec);
            }

        } // end while

      // File reading is complete, destroy the stream.
      AVStreams::flowSpec stop_spec;
      this->streamctrl_->destroy (stop_spec);

      // Shut the orb down.
      TAO_AV_CORE::instance ()->orb ()->shutdown (0);

    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception ("Sender::pace_data Failed\n");
      return -1;
    }
  return 0;
}

int
ACE_TMAIN (int argc,
      ACE_TCHAR *argv[])
{
  try
    {
      CORBA::ORB_var orb =
        CORBA::ORB_init (argc, argv);

      CORBA::Object_var obj
        = orb->resolve_initial_references ("RootPOA");

      // Get the POA_var object from Object_var
      PortableServer::POA_var root_poa
        = PortableServer::POA::_narrow (obj.in ());

      PortableServer::POAManager_var mgr
        = root_poa->the_POAManager ();

      mgr->activate ();

      // Initialize the AV Stream components.
      TAO_AV_CORE::instance ()->init (orb.in (),
                                      root_poa.in ());

      // Initialize the Sender.
      int result = 0;
      result = SENDER::instance ()->init (argc,
                                          argv);

      if (result < 0)
        ACE_ERROR_RETURN ((LM_ERROR,
                           "Sender::init failed\n"),
                          -1);

      // Start sending data.
      result = SENDER::instance ()->pace_data ();
    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception ("Sender Failed\n");
      return -1;
    }

  SENDER::close (); // Explicitly finalize the Unmanaged_Singleton.

  return 0;
}

#if defined (ACE_HAS_EXPLICIT_STATIC_TEMPLATE_MEMBER_INSTANTIATION)
template ACE_Unmanaged_Singleton<Sender, ACE_Null_Mutex> *ACE_Unmanaged_Singleton<Sender, ACE_Null_Mutex>::singleton_;
#endif /* ACE_HAS_EXPLICIT_STATIC_TEMPLATE_MEMBER_INSTANTIATION */
