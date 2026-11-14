#include "sender.h"
#include "tao/debug.h"
#include "ace/Get_Opt.h"
#include "ace/High_Res_Timer.h"
#include "ace/OS_NS_stdio.h"
#include <cstddef>
#include <memory>

using SENDER = ACE_Unmanaged_Singleton<Sender, ACE_Null_Mutex>;

// Borrowed by the callback; ACE_TMAIN owns the file for its duration.
static FILE *output_file {};

static ACE_TCHAR const output_file_name[] = ACE_TEXT ("output");

int
Sender_StreamEndPoint::get_callback (char const *,
                                     TAO_AV_Callback *&callback)
{
  // Create and return the sender application callback to AVStreams
  // for further upcalls.
  callback = &this->callback_;
  return 0;
}

int
Sender_StreamEndPoint::set_protocol_object (char const *,
                                            TAO_AV_Protocol_Object *object)
{
  // Set the sender protocol object corresponding to the transport
  // protocol selected.
  SENDER::instance ()->protocol_object (object);
  return 0;
}

int
Sender_Callback::receive_frame (ACE_Message_Block *frame,
                                TAO_AV_frame_info *,
                                ACE_Addr const &)
{
  //
  // Upcall from the AVStreams when there is data to be received from
  // the sender.
  //
  ACE_DEBUG ((LM_DEBUG,
              "Sender_Callback::receive_frame for frame %d\n",
              this->frame_count_++));

  while (frame != nullptr)
    {
      // Write the received data to the file.
      size_t const result = std::fwrite (frame->rd_ptr (),
                                              1,
                                              frame->length (),
                                              output_file);

      if (result != frame->length ())
        ACE_ERROR_RETURN ((LM_ERROR,
                           "Sender_Callback::fwrite failed\n"),
                          -1);

      frame = frame->cont ();
    }

  if (SENDER::instance ()->eof ())
    SENDER::instance ()->shutdown ();
  return 0;
}

void
Sender::protocol_object (TAO_AV_Protocol_Object *object)
{
  // Set the sender protocol object corresponding to the transport
  // protocol selected.
  this->protocol_object_ = object;
}

bool
Sender::eof () const
{
  return this->eof_;
}

void
Sender::shutdown ()
{
  try
    {
      // File reading is complete, destroy the stream.
      AVStreams::flowSpec stop_spec;
      this->streamctrl_->destroy (stop_spec);

      // Shut the orb down.
      TAO_AV_CORE::instance ()->orb ()->shutdown (false);
    }
  catch (CORBA::Exception const &ex)
    {
      ex._tao_print_exception ("shutdown\n");
    }
}

int
Sender::parse_args (int argc,
                    ACE_TCHAR *argv[])
{
  // Parse command line arguments
  ACE_Get_Opt opts (argc, argv, ACE_TEXT("f:p:r:d"));

  int c;
  while ((c = opts ()) != -1)
    {
      switch (c)
        {
        case 'f':
          this->options_.filename = ACE_TEXT_ALWAYS_CHAR (opts.opt_arg ());
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
          ACE_DEBUG ((LM_DEBUG, "Unknown Option\n"));
          return -1;
        }
    }
  return 0;
}

/// Resolve the receiver object reference from the Naming Service.
int
Sender::bind_to_receiver ()
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
  this->input_file_.reset (std::fopen (this->options_.filename.c_str (), "rb"));

  if (!this->input_file_)
    ACE_ERROR_RETURN ((LM_DEBUG,
                       "Cannot open input file %C\n",
                       this->options_.filename.c_str ()),
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

  // Initialize the  QoS
  AVStreams::streamQoS_var the_qos (new AVStreams::streamQoS);

  // Create the forward flow specification to describe the flow.
  TAO_Forward_FlowSpec_Entry entry ("Data_Receiver",
                                    "IN",
                                    "USER_DEFINED",
                                    "",
                                    this->options_.protocol.c_str (),
                                    nullptr);

  AVStreams::flowSpec flow_spec (1);
  flow_spec.length (2);
  flow_spec [0] = CORBA::string_dup (entry.entry_to_string ());

    // Create the forward flow specification to describe the flow.
  TAO_Forward_FlowSpec_Entry entry1 ("Data_Receiver1",
                                    "OUT",
                                    "USER_DEFINED",
                                    "",
                                    this->options_.protocol.c_str (),
                                    nullptr);

  flow_spec [1] = CORBA::string_dup (entry1.entry_to_string ());

  // Register the sender mmdevice object with the ORB
  TAO_MMDevice *sender_mmdevice {};
  ACE_NEW_RETURN (sender_mmdevice,
                  TAO_MMDevice (&this->endpoint_strategy_),
                  -1);

  // Servant Reference Counting to manage lifetime
  PortableServer::ServantBase_var safe_mmdevice =
    sender_mmdevice;

  AVStreams::MMDevice_var mmdevice =
    sender_mmdevice->_this ();

  ACE_NEW_RETURN (this->streamctrl_,
                  TAO_StreamCtrl,
                  -1);

  PortableServer::ServantBase_var safe_streamctrl =
    this->streamctrl_;

  // Bind/Connect the sender and receiver MMDevices.
  CORBA::Boolean bind_result =
    this->streamctrl_->bind_devs (mmdevice.in (),
                                  this->receiver_mmdevice_.in (),
                                  the_qos.inout (),
                                  flow_spec);

  if (!bind_result)
    ACE_ERROR_RETURN ((LM_ERROR,
                       "streamctrl::bind_devs failed\n"),
                      -1);

  return 0;
}

/// Send input data at the configured frame rate.
int
Sender::pace_data ()
{
  // The time that should lapse between two consecutive frames sent.
  ACE_Time_Value inter_frame_time;

  // The time between two consecutive frames.
  inter_frame_time.set (1.0 / this->options_.frame_rate);

  if (TAO_debug_level > 0)
    ACE_DEBUG ((LM_DEBUG,
                "Frame Rate = %d / second\n"
                "Inter Frame Time = %lu (msec)\n",
                this->options_.frame_rate,
                inter_frame_time.msec ()));

  try
    {
      // The time taken for sending a frame and preparing for the next frame
      ACE_High_Res_Timer elapsed_timer;

      // Continue to send data till the file is read to the end.
      while (true)
        {
          // Read from the file into a message block.
          size_t const n = std::fread (this->mb_.wr_ptr (),
                                            1,
                                            this->mb_.size (),
                                            this->input_file_.get ());

          if (n == 0)
            {
              // At end of file break the loop and end the sender.
              ACE_DEBUG ((LM_DEBUG, "Handle_Start:End of file\n"));
              this->eof_ = true;
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
                            "Elapsed Time = %lu\n",
                            elapsed_time.msec ()));

              // Check to see if the inter frame time has elapsed.
              if (elapsed_time < inter_frame_time)
                {
                  // Inter frame time has not elapsed.

                  // Calculate the time to wait before the next frame needs to be sent.
                  ACE_Time_Value wait_time (inter_frame_time - elapsed_time);

                  if (TAO_debug_level > 0)
                    ACE_DEBUG ((LM_DEBUG,
                                "Wait Time = %lu\n",
                                wait_time.msec ()));

                  // Run the orb for the wait time so the sender can
                  // continue other orb requests.
                  TAO_AV_CORE::instance ()->orb ()->run (wait_time);
                }
            }

          // Start timer before sending the frame.
          elapsed_timer.start ();

          // Send frame.
          int const result = this->protocol_object_->send_frame (&this->mb_);

          if (result < 0)
            ACE_ERROR_RETURN ((LM_ERROR,
                               "send failed:%p",
                               "Sender::pace_data send\n"),
                              -1);

          ACE_DEBUG ((LM_DEBUG,
                      "Sender::pace_data frame %d was sent successfully\n",
                      ++this->frame_count_));

          // Reset the message block.
          this->mb_.reset ();
        }

        // File reading is complete, destroy the stream.
        AVStreams::flowSpec stop_spec;
        this->streamctrl_->destroy (stop_spec);
    }
  catch (CORBA::Exception const &)
    {
      return -1;
    }
  return 0;
}

int
ACE_TMAIN (int argc,
           ACE_TCHAR *argv[])
{
  using File_Ptr = std::unique_ptr<FILE, int (*) (FILE *)>;
  File_Ptr output_file_owner (nullptr, std::fclose);

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

      // Initialize the AVStreams components.
      TAO_AV_CORE::instance ()->init (orb.in (), root_poa.in ());

      // Initialize the Sender.
      int result = SENDER::instance ()->init (argc, argv);

      if (result < 0)
        ACE_ERROR_RETURN ((LM_ERROR,
                           "Sender::init failed\n"),
                          -1);

      // Make sure we have a valid <output_file>
      output_file_owner.reset (ACE_OS::fopen (output_file_name, "wb"));
      output_file = output_file_owner.get ();
      if (!output_file_owner)
        ACE_ERROR_RETURN ((LM_DEBUG,
                           "Cannot open output file %s\n",
                           output_file_name),
                          -1);

      else
        ACE_DEBUG ((LM_DEBUG,
                    "File Opened Successfully\n"));

      // Start sending data.
      result = SENDER::instance ()->pace_data ();
      ACE_Time_Value tv (3, 0);
      orb->run (tv);
    }
  catch (CORBA::Exception const &ex)
    {
      ex._tao_print_exception ("Sender Failed\n");
      return -1;
    }

  SENDER::close ();  // Explicitly finalize the Unmanaged_Singleton.

  return 0;
}

#if defined (ACE_HAS_EXPLICIT_STATIC_TEMPLATE_MEMBER_INSTANTIATION)
template ACE_Unmanaged_Singleton<Sender, ACE_Null_Mutex> *ACE_Unmanaged_Singleton<Sender, ACE_Null_Mutex>::singleton_;
#endif /* ACE_HAS_EXPLICIT_STATIC_TEMPLATE_MEMBER_INSTANTIATION */
