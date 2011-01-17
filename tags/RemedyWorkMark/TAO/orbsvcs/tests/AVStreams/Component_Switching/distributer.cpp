// $Id$

#include "distributer.h"
#include "tao/debug.h"
#include "ace/Get_Opt.h"
#include "orbsvcs/AV/Protocol_Factory.h"
#include "orbsvcs/AV/FlowSpec_Entry.h"

#include "tao/Strategies/advanced_resource.h"

typedef ACE_Unmanaged_Singleton<Distributer, ACE_Null_Mutex> DISTRIBUTER;

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

      DISTRIBUTER::instance ()->done (1);

    }
  return 0;
}

int
Distributer_Sender_StreamEndPoint::get_callback (const char *flow_name,
                                                 TAO_AV_Callback *&callback)
{
  /// Create and return the sender application callback to AVStreams
  /// for further upcalls.
  callback = &this->callback_;

  ACE_CString fname = flow_name;

  this->callback_.flowname (fname);

  return 0;
}

int
Distributer_Sender_StreamEndPoint::set_protocol_object (const char *flowname,
                                                        TAO_AV_Protocol_Object *object)
{
  Connection_Manager &connection_manager =
    DISTRIBUTER::instance ()->connection_manager ();

  /// Add to the map of protocol objects.
  connection_manager.protocol_objects ().bind (flowname, object);

  /// Store the related streamctrl.
  connection_manager.add_streamctrl (flowname, this);

  return 0;
}

int
Distributer_Receiver_StreamEndPoint::get_callback (const char *flow_name,
                                                   TAO_AV_Callback *&callback)
{
  /// Create and return the receiver application callback to AVStreams
  /// for further upcalls.
  callback = &this->callback_;

  ACE_CString flowname (flow_name);
  this->callback_.flowname (flowname);

  return 0;
}

int
Distributer_Receiver_StreamEndPoint::set_protocol_object (const char *,
                                                          TAO_AV_Protocol_Object *)
{
  /// Increment the stream count.
  DISTRIBUTER::instance ()->stream_created ();

  return 0;
}

CORBA::Boolean
Distributer_Receiver_StreamEndPoint::handle_connection_requested (AVStreams::flowSpec &flowspec)
{
  //if (TAO_debug_level > 0)
    ACE_DEBUG ((LM_DEBUG,
                "Distributer_Receiver_StreamEndPoint::handle_connection_requested\n"));

  Connection_Manager &connection_manager =
    DISTRIBUTER::instance ()->connection_manager ();

  /// Check to see if the flow already exists. If it does then close the
  /// old connection and setup a new one with the new sender.

  for (CORBA::ULong i = 0;
       i < flowspec.length ();
       i++)
    {
      TAO_Forward_FlowSpec_Entry entry;
      entry.parse (flowspec[i]);

      //if (TAO_debug_level > 0)
        ACE_DEBUG ((LM_DEBUG,
                    "Handle Connection Requested flowname %C\n",
                    entry.flowname ()));

      ACE_CString flowname (entry.flowname ());

      int const result = connection_manager.streamctrls ().find (flowname);

      /// If the flowname is found.
      if (result == 0)
        {
          ACE_DEBUG ((LM_DEBUG, "\nDistributer switching senders handle connection requested\n\n"));

          ///Destroy old stream with the same flowname.
          connection_manager.destroy (flowname);

        }

      /// Store the related streamctrl.
      connection_manager.add_streamctrl (flowname.c_str (), this);

    }
  return true;

}


Distributer_Receiver_Callback::Distributer_Receiver_Callback (void)
  : frame_count_ (1)
{
}

ACE_CString &
Distributer_Receiver_Callback::flowname (void)
{
  return this->flowname_;
}

void
Distributer_Receiver_Callback::flowname (const ACE_CString &flowname)
{
  this->flowname_ = flowname;
}


int
Distributer_Receiver_Callback::receive_frame (ACE_Message_Block *frame,
                                              TAO_AV_frame_info *,
                                              const ACE_Addr &)
{
  /// Upcall from the AVStreams when there is data to be received from
  /// the sender.
  ACE_DEBUG ((LM_DEBUG,
              "Distributer_Callback::receive_frame for frame %d\n",
              this->frame_count_++));

  Connection_Manager::Protocol_Objects &protocol_objects =
    DISTRIBUTER::instance ()->connection_manager ().protocol_objects ();

  /// Send frame to all receivers.
  for (Connection_Manager::Protocol_Objects::iterator iterator = protocol_objects.begin ();
       iterator != protocol_objects.end ();
       ++iterator)
    {
      int const result = (*iterator).int_id_->send_frame (frame);

      if (result < 0)
        ACE_ERROR_RETURN ((LM_ERROR,
                           "send failed:%p",
                           "Sender::pace_data send\n"),
                          -1);
    }

  return 0;
}

int
Distributer_Receiver_Callback::handle_destroy (void)
{
  /// Called when the sender requests the stream to be shutdown.
  ACE_DEBUG ((LM_DEBUG,
              "Distributer_Receiver_Callback::end_stream\n"));

  DISTRIBUTER::instance ()->connection_manager ().streamctrls ().unbind (this->flowname_.c_str ());

  /// Decrement the stream count.
  DISTRIBUTER::instance ()->stream_destroyed ();

  return 0;
}

ACE_CString &
Distributer_Sender_Callback::flowname (void)
{
  return this->flowname_;
}

void
Distributer_Sender_Callback::flowname (const ACE_CString &flowname)
{
  this->flowname_ = flowname;
}

int
Distributer_Sender_Callback::handle_destroy (void)
{
  /// Called when the sender requests the stream to be shutdown.

  ACE_DEBUG ((LM_DEBUG,
              "Distributer_Sender_Callback::end_stream\n"));

  DISTRIBUTER::instance ()->connection_manager ().protocol_objects ().unbind (this->flowname_.c_str ());

  DISTRIBUTER::instance ()->connection_manager ().streamctrls ().unbind (this->flowname_.c_str ());

  DISTRIBUTER::instance ()->connection_manager ().receivers ().unbind (this->flowname_.c_str ());

  return 0;
}

Distributer::Distributer (void)
  : distributer_receiver_mmdevice_ (0),
    sender_name_ ("sender"),
    distributer_name_ ("distributer"),
    done_ (false),
    stream_count_ (0)
{
}

Distributer::~Distributer (void)
{
}

void
Distributer::stream_created (void)
{
  ++this->stream_count_;
}

void
Distributer::stream_destroyed (void)
{
  --this->stream_count_;

  if (this->stream_count_ == 0)
    this->done_ = true;
}


Connection_Manager &
Distributer::connection_manager (void)
{
  return this->connection_manager_;
}

int
Distributer::parse_args (int argc,
                         ACE_TCHAR *argv[])
{
  /// Parse command line arguments
  ACE_Get_Opt opts (argc, argv, ACE_TEXT("s:r:"));

  int c;
  while ((c= opts ()) != -1)
    {
      switch (c)
        {
        case 's':
          this->sender_name_ = ACE_TEXT_ALWAYS_CHAR (opts.opt_arg ());
          break;
        case 'r':
          this->distributer_name_ = ACE_TEXT_ALWAYS_CHAR (opts.opt_arg ());
          break;
        default:
          ACE_DEBUG ((LM_DEBUG,"Unknown Option\n"));
          return -1;
        }
    }
  return 0;
}


int
Distributer::init (int argc,
                   ACE_TCHAR *argv[])
{
  /// Initialize the connection class.
  int result =
    this->connection_manager_.init (TAO_AV_CORE::instance ()->orb ());
  if (result != 0)
    return result;

  /// Initialize the endpoint strategy with the orb and poa.
  result =
    this->sender_endpoint_strategy_.init (TAO_AV_CORE::instance ()->orb (),
                                          TAO_AV_CORE::instance ()->poa ());
  if (result != 0)
    return result;

  result =
    this->receiver_endpoint_strategy_.init (TAO_AV_CORE::instance ()->orb (),
                                            TAO_AV_CORE::instance ()->poa ());
  if (result != 0)
    return result;

  /// Parse the command line arguments
  result =
    this->parse_args (argc,
                      argv);
  if (result != 0)
    return result;

  ACE_Reactor *reactor =
    TAO_AV_CORE::instance ()->reactor ();

  if (reactor->register_handler (SIGINT,
                                 &this->signal_handler_) == -1)
    ACE_ERROR_RETURN ((LM_ERROR,
                       "Error in handler register\n"),
                      -1);
  /// Register the signal handler for clean termination of the process.

  ACE_NEW_RETURN (this->distributer_sender_mmdevice_,
                  TAO_MMDevice (&this->sender_endpoint_strategy_),
                  -1);

  /// Servant Reference Counting to manage lifetime
  PortableServer::ServantBase_var safe_sender_mmdevice =
    this->distributer_sender_mmdevice_;

  AVStreams::MMDevice_var distributer_sender_mmdevice =
    this->distributer_sender_mmdevice_->_this ();

  ACE_NEW_RETURN (this->distributer_receiver_mmdevice_,
                  TAO_MMDevice (&this->receiver_endpoint_strategy_),
                  -1);

  /// Servant Reference Counting to manage lifetime
  PortableServer::ServantBase_var safe_receiver_mmdevice =
    this->distributer_receiver_mmdevice_;

  AVStreams::MMDevice_var distributer_receiver_mmdevice =
    this->distributer_receiver_mmdevice_->_this ();


  /// Bind to sender.
  this->connection_manager_.bind_to_sender (this->sender_name_,
                                            this->distributer_name_,
                                            distributer_receiver_mmdevice.in ());

  /// Connect to sender.
  this->connection_manager_.connect_to_sender ();

  /// Bind to receivers.
  this->connection_manager_.bind_to_receivers (this->distributer_name_,
                                               distributer_sender_mmdevice.in ());

  /// Connect to receivers
  this->connection_manager_.connect_to_receivers ();

  return 0;
}

bool
Distributer::done (void) const
{
  return this->done_;
}

void
Distributer::shut_down (void)
{
  try
    {
      AVStreams::MMDevice_var receiver_mmdevice =
        this->distributer_receiver_mmdevice_->_this ();

      DISTRIBUTER::instance ()->connection_manager ().unbind_receiver (this->sender_name_,
                                                                       this->distributer_name_,
                                                                       receiver_mmdevice.in ());
      AVStreams::MMDevice_var sender_mmdevice =
        this->distributer_sender_mmdevice_->_this ();

      DISTRIBUTER::instance ()->connection_manager ().unbind_sender (this->distributer_name_,
                                                                     sender_mmdevice.in ());

    //  DISTRIBUTER::instance ()->connection_manager ().destroy ();

    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception ("Distributer::shut_down");
    }
}

void
Distributer::done (bool done)
{
  this->done_ = done;
}

int
ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
  try
    {
      /// Initialize the ORB first.
      CORBA::ORB_var orb = CORBA::ORB_init (argc, argv);

      CORBA::Object_var obj = orb->resolve_initial_references ("RootPOA");

      /// Get the POA_var object from Object_var.
      PortableServer::POA_var root_poa =
        PortableServer::POA::_narrow (obj.in ());

      PortableServer::POAManager_var mgr = root_poa->the_POAManager ();

      mgr->activate ();

      /// Initialize the AVStreams components.
      TAO_AV_CORE::instance ()->init (orb.in (), root_poa.in ());

      /// Initialize the Distributer
      int result = DISTRIBUTER::instance ()->init (argc, argv);

      if (result != 0)
        return result;

      while (!DISTRIBUTER::instance ()->done ())
        {
          CORBA::Boolean wp = orb->work_pending ();

          if (wp)
          {
            orb->perform_work ();

          }
        }

      DISTRIBUTER::instance ()->shut_down ();

//      orb->shutdown(1);

    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception ("main");
      return -1;
    }

  DISTRIBUTER::close ();  // Explicitly finalize the Unmanaged_Singleton.

  return 0;
}

#if defined (ACE_HAS_EXPLICIT_STATIC_TEMPLATE_MEMBER_INSTANTIATION)
template ACE_Unmanaged_Singleton<Distributer, ACE_Null_Mutex> *ACE_Unmanaged_Singleton<Distributer, ACE_Null_Mutex>::singleton_;
#endif /* ACE_HAS_EXPLICIT_STATIC_TEMPLATE_MEMBER_INSTANTIATION */
