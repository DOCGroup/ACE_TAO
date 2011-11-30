// $Id$

#include "sender.h"
#include "tao/debug.h"
#include "ace/Get_Opt.h"
#include "ace/High_Res_Timer.h"

// Create a singleton instance of the Sender.

// An Unmanaged_Singleton is used to avoid static object destruction
// order related problems since the underlying singleton object
// contains references to static TypeCodes.
typedef ACE_Unmanaged_Singleton<Sender, ACE_Null_Mutex> SENDER;

int
Sender_StreamEndPoint::get_callback (const char *,
                                     TAO_AV_Callback *&callback)
{
  // Create and return the sender application callback to AVStreams
  // for further upcalls.
  callback = &this->callback_;

  // Get the stream controller for this stream.
  try
    {
      CORBA::Any_ptr streamctrl_any =
        this->get_property_value ("Related_StreamCtrl");

      AVStreams::StreamCtrl_ptr streamctrl;
      *streamctrl_any >>= streamctrl;

      // Store reference to the streamctrl
      SENDER::instance ()->streamctrl (streamctrl);
    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception ("Sender_StreamEndPoint::get_callback failed");

      return -1;
    }

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
    frame_rate_ (10.0),
    mb_ (BUFSIZ),
    protocol_object_ (0)
{
}

void
Sender::protocol_object (TAO_AV_Protocol_Object *object)
{
  // Set the sender protocol object corresponding to the transport
  // protocol selected.
  this->protocol_object_ = object;
}

void
Sender::streamctrl (AVStreams::StreamCtrl_ptr streamctrl)
{
  // Set the sender protocol object corresponding to the transport
  // protocol selected.
  this->streamctrl_ = streamctrl;
}

int
Sender::parse_args (int argc,
                    ACE_TCHAR *argv[])
{
  // Parse command line arguments
  ACE_Get_Opt opts (argc, argv, ACE_TEXT("f:r:d"));

  int c;
  while ((c= opts ()) != -1)
    {
      switch (c)
        {
        case 'f':
          this->filename_ = ACE_TEXT_ALWAYS_CHAR (opts.opt_arg ());
          break;
        case 'r':
          this->frame_rate_ = (double)ACE_OS::atoi (opts.opt_arg ());
          break;
        case 'd':
          TAO_debug_level++;
          break;
        default:
          ACE_DEBUG ((LM_DEBUG, "Unknown Option\n"));
          return -1;
        }
    }
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
    ACE_ERROR_RETURN ((LM_ERROR,
                       "Cannot open input file %C\n",
                       this->filename_.c_str ()),
                      -1);
  else
    ACE_DEBUG ((LM_DEBUG,
                "File opened successfully\n"));

  // Register the sender mmdevice object with the ORB
  ACE_NEW_RETURN (this->sender_mmdevice_,
                  TAO_MMDevice (&this->endpoint_strategy_),
                  -1);

  // Servant Reference Counting to manage lifetime
  PortableServer::ServantBase_var safe_mmdevice =
    this->sender_mmdevice_;

  AVStreams::MMDevice_var mmdevice =
    this->sender_mmdevice_->_this ();

  CosNaming::Name name (1);
  name.length (1);
  name [0].id =
    CORBA::string_dup ("Sender");

  // Register the sender object with the naming server.
  this->naming_client_->rebind (name,
                                mmdevice.in ());

  return 0;
}

// Method to send data at the specified rate
int
Sender::pace_data (void)
{
  // The time that should lapse between two consecutive frames sent.
  ACE_Time_Value inter_frame_time;

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

      // If we have a receiver, send to it.
      while (this->protocol_object_ == 0)
        {
          // Run the orb for the wait time so the sender can
          // continue other orb requests.
          ACE_Time_Value wait_time (5);
          TAO_AV_CORE::instance ()->orb ()->run (wait_time);
        }

      // Continue to send data till the file is read to the end.
      while (1)
        {
          // Read from the file into a message block.
          size_t n = ACE_OS::fread (this->mb_.wr_ptr (),
                                    1,
                                    this->mb_.size (),
                                    this->input_file_);

          if (n == 0 && ferror (this->input_file_))
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
                      "Sender::pace_data frame %d was sent succesfully\n",
                      ++this->frame_count_));

          // Reset the message block.
          this->mb_.reset ();

        } // end while

      // If a stream was setup, destroy it.
      if (this->streamctrl_)
        {
          // File reading is complete, destroy the stream.
          AVStreams::flowSpec stop_spec;
          this->streamctrl_->destroy (stop_spec);
        }
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

      SENDER::instance ()->pace_data ();

      ACE_Time_Value tv (10);
      orb->run (tv);
      // Hack for now....
      ACE_OS::sleep (1);
    }
  catch (const CORBA::Exception& ex)
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
