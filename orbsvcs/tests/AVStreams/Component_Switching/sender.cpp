// $Id$

#include "sender.h"
#include "tao/debug.h"
#include "ace/Get_Opt.h"
#include "ace/High_Res_Timer.h"
#include "ace/Event_Handler.h"

#include "tao/Strategies/advanced_resource.h"

// Create a singleton instance of the Sender.

// An Unmanaged_Singleton is used to avoid static object destruction
// order related problems since the underlying singleton object
// contains references to static TypeCodes.
typedef ACE_Unmanaged_Singleton<Sender, ACE_Null_Mutex> SENDER;

int g_shutdown = 0;
/// Flag set when a signal is raised.

// constructor.
Signal_Handler::Signal_Handler (void)
{
}

int
Signal_Handler::handle_signal (int signum, siginfo_t *, ucontext_t*)
{
  if (signum == SIGINT)
    {
      if (TAO_debug_level > 0)
        ACE_DEBUG ((LM_DEBUG,
                    "In the signal handler\n"));

      g_shutdown = 1;

    }
  return 0;
}

ACE_CString &
Sender_Callback::flowname (void)
{
  return this->flowname_;
}

void
Sender_Callback::flowname (const ACE_CString &flowname)
{
  this->flowname_ = flowname;
}


int
Sender_Callback::handle_destroy (void)
{
  SENDER::instance ()->connection_manager ().protocol_objects ().unbind (this->flowname_.c_str ());

  SENDER::instance ()->connection_manager ().streamctrls ().unbind (this->flowname_.c_str ());

  SENDER::instance ()->connection_manager ().receivers ().unbind (this->flowname_.c_str ());

  //  SENDER::instance ()->remove_stream ();

  return 0;
}

int
Sender_StreamEndPoint::get_callback (const char * flowname,
                                     TAO_AV_Callback *&callback)
{
  //SENDER::instance ()->add_stream ();

  /// Create and return the client application callback and return to the AVStreams
  /// for further upcalls.
  callback = &this->callback_;

  ACE_CString flow_name (flowname);
  this->callback_.flowname (flow_name);

  return 0;
}

int
Sender_StreamEndPoint::set_protocol_object (const char *flowname,
                                            TAO_AV_Protocol_Object *object)
{
  Connection_Manager &connection_manager =
    SENDER::instance ()->connection_manager ();

  /// Add to the map of protocol objects.
  connection_manager.protocol_objects ().bind (flowname,
                                               object);

  /// Store the related streamctrl.
  connection_manager.add_streamctrl (flowname,
                                     this);

  return 0;
}

CORBA::Boolean
Sender_StreamEndPoint::handle_preconnect (AVStreams::flowSpec &flowspec)
{
  /// If another receiver of the same flowname is in the map, destroy
  /// the old stream.
  for (CORBA::ULong i = 0;
       i < flowspec.length ();
       i++)
    {
      TAO_Forward_FlowSpec_Entry entry;
      entry.parse (flowspec[i]);

      ACE_CString flowname (entry.flowname ());

      Connection_Manager &connection_manager =
        SENDER::instance ()->connection_manager ();

      int result =
        connection_manager.protocol_objects ().find (flowname);

      /// If the flowname is found.
      if (result == 0)
        {
          ACE_DEBUG ((LM_DEBUG, "\nSender switching distributers\n\n"));

          /// Destroy old stream with the same flowname.
          connection_manager.destroy (flowname);
        }
    }
  return 1;
}

Sender::Sender (void)
  : sender_mmdevice_ (0),
    frame_count_ (0),
    filename_ ("input"),
    input_file_ (0),
    frame_rate_ (5),
    mb_ (BUFSIZ),
    sender_name_ ("sender")
{
}

Sender::~Sender (void)
{
  if (TAO_debug_level > 0)
    ACE_DEBUG ((LM_DEBUG,
                "Sender destructor\n"));
}

void
Sender::shut_down (void)
{
  try
    {
      AVStreams::MMDevice_var mmdevice =
        this->sender_mmdevice_->_this ();

      SENDER::instance ()->connection_manager ().unbind_sender (this->sender_name_,
                                                                mmdevice.in ());

    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception ("Sender::shut_down Failed\n");
    }
}

int
Sender::parse_args (int argc,
                    ACE_TCHAR *argv[])
{
  /// Parse command line arguments
  ACE_Get_Opt opts (argc, argv, ACE_TEXT("s:f:r:d"));

  int c;
  while ((c= opts ()) != -1)
    {
      switch (c)
        {
        case 'f':
          this->filename_ = ACE_TEXT_ALWAYS_CHAR (opts.opt_arg ());
          break;
        case 'r':
          this->frame_rate_ = ACE_OS::atoi (opts.opt_arg ());
          break;
        case 's':
          this->sender_name_ = ACE_TEXT_ALWAYS_CHAR (opts.opt_arg ());
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
  /// Initialize the endpoint strategy with the orb and poa.
  int result =
    this->endpoint_strategy_.init (TAO_AV_CORE::instance ()->orb (),
                                   TAO_AV_CORE::instance ()->poa ());
  if (result != 0)
    return result;

  /// Initialize the connection manager.
  result =
    this->connection_manager_.init (TAO_AV_CORE::instance ()->orb ());
  if (result != 0)
    return result;

  /// Parse the command line arguments
  result =
    this->parse_args (argc,
                      argv);
  if (result != 0)
    return result;

  /*
  ACE_Reactor *reactor =
    TAO_AV_CORE::instance ()->reactor ();


  if (reactor->register_handler (SIGINT,
                                 &this->signal_handler_) == -1)
    ACE_ERROR_RETURN ((LM_ERROR,
                       "Error in handler register\n"),
                      -1);
  /// Register the signal handler for clean termination of the process.
  */

  /// Open file to read.
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

  /// Register the sender mmdevice object with the ORB
  ACE_NEW_RETURN (this->sender_mmdevice_,
                  TAO_MMDevice (&this->endpoint_strategy_),
                  -1);

  /// Servant Reference Counting to manage lifetime
  PortableServer::ServantBase_var safe_mmdevice =
    this->sender_mmdevice_;

  AVStreams::MMDevice_var mmdevice =
    this->sender_mmdevice_->_this ();

  /// Register the object reference with the Naming Service and bind to
  /// the receivers
  this->connection_manager_.bind_to_receivers (this->sender_name_,
                                               mmdevice.in ());

  /// Connect to the receivers
  this->connection_manager_.connect_to_receivers ();

  return 0;
}

/// Method to send data at the specified rate
int
Sender::pace_data (void)
{
  /// The time that should lapse between two consecutive frames sent.
  ACE_Time_Value inter_frame_time;

  /// The time between two consecutive frames.
  inter_frame_time.set (1.0 / this->frame_rate_);

  if (TAO_debug_level > 0)
    ACE_DEBUG ((LM_DEBUG,
                "Frame Rate = %d / second\n"
                "Inter Frame Time = %d (msec)\n",
                this->frame_rate_,
                inter_frame_time.msec ()));

  try
    {
      /// The time taken for sending a frame and preparing for the next frame
      ACE_High_Res_Timer elapsed_timer;

      /// Continue to send data till the file is read to the end.
      while (1)
        {

          if (g_shutdown == 1)
            {
              ACE_DEBUG ((LM_DEBUG,
                          "Shut Down called\n"));

              this->shut_down ();

              break;
            }

          /// Read from the file into a message block.
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
              /// At end of file break the loop and end the sender.
              if (TAO_debug_level > 0)
                ACE_DEBUG ((LM_DEBUG,"Handle_Start:End of file\n"));

              this->shut_down ();

              break;

            }

          this->mb_.wr_ptr (n);

          if (this->frame_count_ > 1)
            {
              ///
              /// Second frame and beyond
              ///

              /// Stop the timer that was started just before the previous frame was sent.
              elapsed_timer.stop ();

              /// Get the time elapsed after sending the previous frame.
              ACE_Time_Value elapsed_time;
              elapsed_timer.elapsed_time (elapsed_time);

              if (TAO_debug_level > 0)
                ACE_DEBUG ((LM_DEBUG,
                            "Elapsed Time = %d\n",
                            elapsed_time.msec ()));

              /// Check to see if the inter frame time has elapsed.
              if (elapsed_time < inter_frame_time)
                {
                  /// Inter frame time has not elapsed.

                  /// Calculate the time to wait before the next frame needs to be sent.
                  ACE_Time_Value wait_time (inter_frame_time - elapsed_time);

                  if (TAO_debug_level > 0)
                    ACE_DEBUG ((LM_DEBUG,
                                "Wait Time = %d\n",
                                wait_time.msec ()));

                  /// Run the orb for the wait time so the sender can
                  /// continue other orb requests.
                  TAO_AV_CORE::instance ()->orb ()->run (wait_time);

                }
            }

          /// Start timer before sending the frame.
          elapsed_timer.start ();

          Connection_Manager::Protocol_Objects &protocol_objects =
            this->connection_manager_.protocol_objects ();

          /// Send frame to all receivers.
          for (Connection_Manager::Protocol_Objects::iterator iterator = protocol_objects.begin ();
               iterator != protocol_objects.end ();
               ++iterator)
            {
              int result =
                (*iterator).int_id_->send_frame (&this->mb_);

              if (result < 0)
                ACE_ERROR_RETURN ((LM_ERROR,
                                   "send failed:%p",
                                   "Sender::pace_data send\n"),
                                  -1);
            }

          ACE_DEBUG ((LM_DEBUG,
                      "Sender::pace_data frame %d was sent succesfully\n",
                      ++this->frame_count_));

          /// Reset the message block.
          this->mb_.reset ();

        } /// end while

    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception ("Sender::pace_data Failed\n");
      return -1;
    }
  return 0;
}

Connection_Manager &
Sender::connection_manager (void)
{
  return this->connection_manager_;
}

//  void
//  Sender::add_stream (void)
//  {
//    this->stream_count_++;
//  }

//  void
//  Sender::remove_stream (void)
//  {
//    this->stream_count_--;
//  }

//  int
//  Sender::stream_alive (void)
//  {
//    return this->stream_count_;
//  }

int
ACE_TMAIN (int argc,
      ACE_TCHAR *argv[])
{
  try
    {
      CORBA::ORB_var orb = CORBA::ORB_init (argc, argv);

      CORBA::Object_var obj
        = orb->resolve_initial_references ("RootPOA");

      ///Get the POA_var object from Object_var
      PortableServer::POA_var root_poa
        = PortableServer::POA::_narrow (obj.in ());

      PortableServer::POAManager_var mgr
        = root_poa->the_POAManager ();

      mgr->activate ();

      /// Initialize the AV Stream components.
      TAO_AV_CORE::instance ()->init (orb.in (),
                                      root_poa.in ());

      /// Initialize the Client.
      int result = 0;
      result = SENDER::instance ()->init (argc,
                                          argv);

      if (result < 0)
        ACE_ERROR_RETURN ((LM_ERROR,
                           "client::init failed\n"), -1);

      SENDER::instance ()->pace_data ();

      orb->destroy ();
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
