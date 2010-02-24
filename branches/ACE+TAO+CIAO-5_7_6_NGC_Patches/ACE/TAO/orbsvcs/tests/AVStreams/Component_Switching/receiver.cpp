// $Id$

#include "receiver.h"
#include "ace/Get_Opt.h"
#include "tao/debug.h"

#include "tao/Strategies/advanced_resource.h"

static FILE *output_file = 0;
/// File handle of the file into which received data is written.

static int done = 0;
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

      done = 1;
    }
  return 0;
}


Connection_Manager *connection_manager;

int
Receiver_StreamEndPoint::get_callback (const char *flow_name,
                                       TAO_AV_Callback *&callback)
{
  /// Return the receiver application callback to the AVStreams for
  /// future upcalls.
  callback = &this->callback_;

  ACE_CString fname = flow_name;
  this->callback_.flowname (fname);
  return 0;
}

CORBA::Boolean
Receiver_StreamEndPoint::handle_connection_requested (AVStreams::flowSpec &flowspec)
{
  if (TAO_debug_level > 0)
    ACE_DEBUG ((LM_DEBUG,
                "In Handle Connection Requested\n"));

  for (CORBA::ULong i = 0;
       i < flowspec.length ();
       i++)
    {
      TAO_Forward_FlowSpec_Entry entry;
      entry.parse (flowspec[i]);

      ACE_DEBUG ((LM_DEBUG,
                  "Handle Connection Requested flowname %C\n",
                  entry.flowname ()));

      ACE_CString flowname (entry.flowname ());

      /// Store the related streamctrl.
      connection_manager->add_streamctrl (flowname.c_str (),
                                         this);

    }
  return 1;

}

Receiver_Callback::Receiver_Callback (void)
  : frame_count_ (1)
{
}

ACE_CString &
Receiver_Callback::flowname (void)
{
  return this->flowname_;
}

void
Receiver_Callback::flowname (const ACE_CString &flowname)
{
  this->flowname_ = flowname;
}

int
Receiver_Callback::handle_destroy (void)
{
  /// Called when the sender requests the stream to be shutdown.
  ACE_DEBUG ((LM_DEBUG,
              "Receiver_Callback::end_stream\n"));

  /// Remove the related stream control reference.
  connection_manager->streamctrls ().unbind (this->flowname_.c_str ());

  return 0;
}
int
Receiver_Callback::receive_frame (ACE_Message_Block *frame,
                                  TAO_AV_frame_info *,
                                  const ACE_Addr &)
{
  ///
  /// Upcall from the AVStreams when there is data to be received from
  /// the sender.
  ///
  ACE_DEBUG ((LM_DEBUG,
              "Receiver_Callback::receive_frame for frame %d\n",
              this->frame_count_++));

  while (frame != 0)
    {
      /// Write the received data to the file.
      int result =
        ACE_OS::fwrite (frame->rd_ptr (),
                        frame->length (),
                        1,
                        output_file);

      if (result == (signed) frame->length ())
        ACE_ERROR_RETURN ((LM_ERROR,
                           "Receiver_Callback::fwrite failed\n"),
                          -1);

      frame = frame->cont ();
    }

  return 0;
}

Receiver::Receiver (void)
  : mmdevice_ (0),
    output_file_name_ ("output"),
    sender_name_ ("distributer"),
    receiver_name_ ("receiver")
{
}

Receiver::~Receiver (void)
{

}

ACE_CString
Receiver::sender_name (void)
{
  return this->sender_name_;
}

ACE_CString
Receiver::receiver_name (void)
{
  return this->receiver_name_;
}

int
Receiver::init (int,
                ACE_TCHAR *[])
{
  /// Initialize the endpoint strategy with the orb and poa.
  int result =
    this->reactive_strategy_.init (TAO_AV_CORE::instance ()->orb (),
                                   TAO_AV_CORE::instance ()->poa ());
  if (result != 0)
    return result;

  /// Initialize the connection manager.
  result =
    this->connection_manager_.init (TAO_AV_CORE::instance ()->orb ());
  if (result != 0)
    return result;

  connection_manager = &this->connection_manager_;

  ACE_Reactor *reactor =
    TAO_AV_CORE::instance ()->reactor ();

  if (reactor->register_handler (SIGINT,
                                 &this->signal_handler_) == -1)
    ACE_ERROR_RETURN ((LM_ERROR,
                       "Error in handler register\n"),
                      -1);
  /// Register the signal handler for clean termination of the process.

  /// Register the receiver mmdevice object with the ORB
  ACE_NEW_RETURN (this->mmdevice_,
                  TAO_MMDevice (&this->reactive_strategy_),
                  -1);

  /// Servant Reference Counting to manage lifetime
  PortableServer::ServantBase_var safe_mmdevice =
    this->mmdevice_;

  AVStreams::MMDevice_var mmdevice =
    this->mmdevice_->_this ();

  /// Bind to sender.
  this->connection_manager_.bind_to_sender (this->sender_name_,
                                            this->receiver_name_,
                                            mmdevice.in ());

  /// Connect to the sender.
  this->connection_manager_.connect_to_sender ();

  return 0;
}

int
Receiver::parse_args (int argc,
                      ACE_TCHAR *argv[])
{
  /// Parse the command line arguments
  ACE_Get_Opt opts (argc,
                    argv,
                    "f:s:r:");

  int c;
  while ((c = opts ()) != -1)
    {
      switch (c)
        {
        case 'f':
          this->output_file_name_ = ACE_TEXT_ALWAYS_CHAR (opts.opt_arg ());
          break;
        case 's':
          this->sender_name_ = ACE_TEXT_ALWAYS_CHAR (opts.opt_arg ());
          break;
        case 'r':
          this->receiver_name_ = ACE_TEXT_ALWAYS_CHAR (opts.opt_arg ());
          break;
        default:
          ACE_ERROR_RETURN ((LM_ERROR,
                             "Usage: receiver -f filename"),
                            -1);
        }
    }

  return 0;
}

ACE_CString
Receiver::output_file_name (void)
{
  return this->output_file_name_;
}

void
Receiver::shut_down (void)
{
  try
    {
      AVStreams::MMDevice_var mmdevice_obj =
        this->mmdevice_->_this ();


      this->connection_manager_.unbind_receiver (this->sender_name_,
                                                 this->receiver_name_,
                                                 mmdevice_obj.in ());


    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception ("Receiver::shut_down");
    }

}

int
ACE_TMAIN (int argc,
      ACE_TCHAR *argv[])
{
  try
    {
      /// Initialize the ORB first.
      CORBA::ORB_var orb =
        CORBA::ORB_init (argc, argv);

      CORBA::Object_var obj
        = orb->resolve_initial_references ("RootPOA");

      /// Get the POA_var object from Object_var.
      PortableServer::POA_var root_poa =
        PortableServer::POA::_narrow (obj.in ());

      PortableServer::POAManager_var mgr
        = root_poa->the_POAManager ();

      mgr->activate ();

      /// Initialize the AVStreams components.
      TAO_AV_CORE::instance ()->init (orb.in (),
                                      root_poa.in ());

      Receiver receiver;
      int result =
        receiver.parse_args (argc,
                             argv);
      if (result == -1)
        return -1;

      /// Make sure we have a valid <output_file>
      output_file =
        ACE_OS::fopen (receiver.output_file_name ().c_str (),
                       "w");
      if (output_file == 0)
        ACE_ERROR_RETURN ((LM_DEBUG,
                           "Cannot open output file %C\n",
                           receiver.output_file_name ().c_str ()),
                          -1);

      else
        ACE_DEBUG ((LM_DEBUG,
                    "File Opened Successfully\n"));

      result =
        receiver.init (argc,
                       argv);

      if (result != 0)
        return result;

      while (!done)
        {
          orb->perform_work ();
        }

      receiver.shut_down ();

      ACE_OS::fclose (output_file);
    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception ("receiver::init");
      return -1;
    }

  return 0;
}
