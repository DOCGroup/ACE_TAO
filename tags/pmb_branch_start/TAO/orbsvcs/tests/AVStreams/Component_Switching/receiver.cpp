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
Receiver_StreamEndPoint::handle_connection_requested (AVStreams::flowSpec &flowspec
                                                      ACE_ENV_ARG_DECL_NOT_USED)
{
  if (TAO_debug_level > 0)
    ACE_DEBUG ((LM_DEBUG,
                "In Handle Conection Requested \n"));

  for (CORBA::ULong i = 0;
       i < flowspec.length ();
       i++)
    {
      TAO_Forward_FlowSpec_Entry entry;
      entry.parse (flowspec[i].in ());

      ACE_DEBUG ((LM_DEBUG,
                  "Handle Conection Requested flowname %s \n",
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
                char **
                ACE_ENV_ARG_DECL)
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
    this->mmdevice_->_this (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK_RETURN (-1);

  /// Bind to sender.
  this->connection_manager_.bind_to_sender (this->sender_name_,
                                            this->receiver_name_,
                                            mmdevice.in ()
                                            ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (-1);

  /// Connect to the sender.
  this->connection_manager_.connect_to_sender (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK_RETURN (-1);

  return 0;
}

int
Receiver::parse_args (int argc,
                      char **argv)
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
          this->output_file_name_ = opts.opt_arg ();
          break;
        case 's':
          this->sender_name_ = opts.opt_arg ();
          break;
        case 'r':
          this->receiver_name_ = opts.opt_arg ();
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
Receiver::shut_down (ACE_ENV_SINGLE_ARG_DECL)
{
  ACE_TRY
    {
      AVStreams::MMDevice_var mmdevice_obj =
        this->mmdevice_->_this (ACE_ENV_SINGLE_ARG_PARAMETER);

      ACE_TRY_CHECK;

      this->connection_manager_.unbind_receiver (this->sender_name_,
                                                 this->receiver_name_,
                                                 mmdevice_obj.in ());


    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION,"Receiver::unbind");
    }
  ACE_ENDTRY;

}

int
main (int argc,
      char **argv)
{
  ACE_DECLARE_NEW_CORBA_ENV;
  ACE_TRY
    {
      /// Initialize the ORB first.
      CORBA::ORB_var orb =
        CORBA::ORB_init (argc,
                         argv,
                         0
                         ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      CORBA::Object_var obj
        = orb->resolve_initial_references ("RootPOA"
                                           ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      /// Get the POA_var object from Object_var.
      PortableServer::POA_var root_poa =
        PortableServer::POA::_narrow (obj.in ()
                                      ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      PortableServer::POAManager_var mgr
        = root_poa->the_POAManager (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;

      mgr->activate (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;

      /// Initialize the AVStreams components.
      TAO_AV_CORE::instance ()->init (orb.in (),
                                      root_poa.in ()
                                      ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

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
                           "Cannot open output file %s\n",
                           receiver.output_file_name ().c_str ()),
                          -1);

      else
        ACE_DEBUG ((LM_DEBUG,
                    "File Opened Successfully\n"));

      result =
        receiver.init (argc,
                       argv
                       ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      if (result != 0)
        return result;

      while (!done)
        {
          orb->perform_work (ACE_ENV_SINGLE_ARG_PARAMETER);
          ACE_TRY_CHECK;
        }

      receiver.shut_down (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;

      ACE_OS::fclose (output_file);
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION,"receiver::init");
      return -1;
    }
  ACE_ENDTRY;
  ACE_CHECK_RETURN (-1);

  return 0;
}

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)
template class TAO_AV_Endpoint_Reactive_Strategy_B<Receiver_StreamEndPoint,TAO_VDev,AV_Null_MediaCtrl>;
template class TAO_AV_Endpoint_Reactive_Strategy<Receiver_StreamEndPoint,TAO_VDev,AV_Null_MediaCtrl>;
#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)
#pragma instantiate TAO_AV_Endpoint_Reactive_Strategy_B<Receiver_StreamEndPoint,TAO_VDev,AV_Null_MediaCtrl>
#pragma instantiate TAO_AV_Endpoint_Reactive_Strategy<Receiver_StreamEndPoint,TAO_VDev,AV_Null_MediaCtrl>
#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */
