#include <cstdio>
#include <memory>

#include "receiver.h"
#include "ace/Get_Opt.h"
#include "tao/debug.h"

#include "tao/Strategies/advanced_resource.h"

/// Non-owning handle; ACE_TMAIN owns the file for all callbacks.
static FILE *output_file {};

/// Flag set when the reactor handles a signal.
static bool done {};

int
Signal_Handler::handle_signal (int signum, siginfo_t *, ucontext_t*)
{
  if (signum == SIGINT)
    {
      if (TAO_debug_level > 0)
        ACE_DEBUG ((LM_DEBUG,
                    "In the signal handler\n"));

      done = true;
    }
  return 0;
}

/// Borrowed pointer to Receiver's connection manager for callback access.
Connection_Manager *connection_manager {};

int
Receiver_StreamEndPoint::get_callback (char const *flow_name,
                                       TAO_AV_Callback *&callback)
{
  // Return the receiver application callback to the AVStreams for
  // future upcalls.
  callback = &this->callback_;

  ACE_CString const fname = flow_name;
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
       ++i)
    {
      TAO_Forward_FlowSpec_Entry entry;
      entry.parse (flowspec[i]);

      ACE_DEBUG ((LM_DEBUG,
                  "Handle Connection Requested flowname %C\n",
                  entry.flowname ()));

      ACE_CString const flowname (entry.flowname ());

      // Store the related streamctrl.
      connection_manager->add_streamctrl (flowname.c_str (),
                                         this);
    }
  return 1;
}

ACE_CString &
Receiver_Callback::flowname ()
{
  return this->flowname_;
}

void
Receiver_Callback::flowname (ACE_CString const &flowname)
{
  this->flowname_ = flowname;
}

int
Receiver_Callback::handle_destroy ()
{
  // Called when the sender requests the stream to be shutdown.
  ACE_DEBUG ((LM_DEBUG,
              "Receiver_Callback::end_stream\n"));

  // Remove the related stream control reference.
  connection_manager->streamctrls ().unbind (this->flowname_.c_str ());

  return 0;
}
int
Receiver_Callback::receive_frame (ACE_Message_Block *frame,
                                  TAO_AV_frame_info *,
                                  ACE_Addr const &)
{
  //
  // Upcall from the AVStreams when there is data to be received from
  // the sender.
  //
  ACE_DEBUG ((LM_DEBUG,
              "Receiver_Callback::receive_frame for frame %B\n",
              ++this->frame_count_));

  while (frame != nullptr)
    {
      // Write the received data to the file.
      size_t const bytes_written =
        std::fwrite (frame->rd_ptr (),
                     1,
                     frame->length (),
                     output_file);

      if (bytes_written != frame->length ())
        ACE_ERROR_RETURN ((LM_ERROR,
                           "Receiver_Callback::fwrite failed\n"),
                          -1);

      frame = frame->cont ();
    }

  return 0;
}

int
Receiver::init (int,
                ACE_TCHAR *[])
{
  // Initialize the endpoint strategy with the orb and poa.
  int result =
    this->reactive_strategy_.init (TAO_AV_CORE::instance ()->orb (),
                                   TAO_AV_CORE::instance ()->poa ());
  if (result != 0)
    return result;

  // Initialize the connection manager.
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
  // Register the signal handler for clean termination of the process.

  // Register the receiver mmdevice object with the ORB
  ACE_NEW_RETURN (this->mmdevice_,
                  TAO_MMDevice (&this->reactive_strategy_),
                  -1);

  // Servant Reference Counting to manage lifetime
  PortableServer::ServantBase_var safe_mmdevice =
    this->mmdevice_;

  AVStreams::MMDevice_var mmdevice =
    this->mmdevice_->_this ();

  // Bind to sender.
  ACE_CString const sender_name (this->options_.sender_name.c_str ());
  ACE_CString const receiver_name (this->options_.receiver_name.c_str ());
  this->connection_manager_.bind_to_sender (sender_name,
                                            receiver_name,
                                            mmdevice.in ());

  // Connect to the sender.
  this->connection_manager_.connect_to_sender ();

  return 0;
}

int
Receiver::parse_args (int argc,
                      ACE_TCHAR *argv[])
{
  // Parse the command line arguments
  ACE_Get_Opt opts (argc,
                    argv,
                    ACE_TEXT("f:s:r:"));

  int c {};
  while ((c = opts ()) != -1)
    {
      switch (c)
        {
        case 'f':
          this->options_.output_file_name = ACE_TEXT_ALWAYS_CHAR (opts.opt_arg ());
          break;
        case 's':
          this->options_.sender_name = ACE_TEXT_ALWAYS_CHAR (opts.opt_arg ());
          break;
        case 'r':
          this->options_.receiver_name = ACE_TEXT_ALWAYS_CHAR (opts.opt_arg ());
          break;
        default:
          ACE_ERROR_RETURN ((LM_ERROR,
                             "Usage: receiver -f filename"),
                            -1);
        }
    }

  return 0;
}

std::string const &
Receiver::output_file_name () const
{
  return this->options_.output_file_name;
}

void
Receiver::shut_down ()
{
  try
    {
      AVStreams::MMDevice_var mmdevice_obj =
        this->mmdevice_->_this ();

      ACE_CString const sender_name (this->options_.sender_name.c_str ());
      ACE_CString const receiver_name (this->options_.receiver_name.c_str ());
      this->connection_manager_.unbind_receiver (sender_name,
                                                 receiver_name,
                                                 mmdevice_obj.in ());
    }
  catch (CORBA::Exception const &ex)
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
      // Initialize the ORB first.
      CORBA::ORB_var orb =
        CORBA::ORB_init (argc, argv);

      CORBA::Object_var obj
        = orb->resolve_initial_references ("RootPOA");

      // Get the POA_var object from Object_var.
      PortableServer::POA_var root_poa =
        PortableServer::POA::_narrow (obj.in ());

      PortableServer::POAManager_var mgr
        = root_poa->the_POAManager ();

      mgr->activate ();

      // Initialize the AVStreams components.
      TAO_AV_CORE::instance ()->init (orb.in (),
                                      root_poa.in ());

      Receiver receiver;
      int result = receiver.parse_args (argc, argv);
      if (result == -1)
        return -1;

      // Make sure we have a valid <output_file>
      std::unique_ptr<FILE, int (*) (FILE *)> output_file_owner (nullptr,
                                                                 std::fclose);
      output_file_owner.reset (std::fopen (receiver.output_file_name ().c_str (), "wb"));
      output_file = output_file_owner.get ();
      if (!output_file_owner)
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

      output_file = nullptr;
    }
  catch (CORBA::Exception const &ex)
    {
      ex._tao_print_exception ("receiver::init");
      return -1;
    }

  return 0;
}
