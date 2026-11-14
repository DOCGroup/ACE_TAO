#include "distributer.h"
#include "ace/Get_Opt.h"
#include "orbsvcs/AV/Protocol_Factory.h"

using DISTRIBUTER = ACE_Unmanaged_Singleton<Distributer, ACE_Null_Mutex>;

int
Distributer_Sender_StreamEndPoint::get_callback (char const *,
                                                 TAO_AV_Callback *&callback)
{
  // Create and return the sender application callback to AVStreams
  // for further upcalls.
  callback = &this->callback_;
  return 0;
}

int
Distributer_Sender_StreamEndPoint::set_protocol_object (char const *flowname,
                                                        TAO_AV_Protocol_Object *object)
{
  Connection_Manager &connection_manager =
    DISTRIBUTER::instance ()->connection_manager ();

  // Add to the map of protocol objects.
  connection_manager.protocol_objects ().bind (flowname,
                                               object);

  // Store the related streamctrl.
  connection_manager.add_streamctrl (flowname, this);

  return 0;
}

int
Distributer_Receiver_StreamEndPoint::get_callback (char const *,
                                                   TAO_AV_Callback *&callback)
{
  // Create and return the receiver application callback to AVStreams
  // for further upcalls.
  callback = &this->callback_;
  return 0;
}

int
Distributer_Receiver_Callback::receive_frame (ACE_Message_Block *frame,
                                              TAO_AV_frame_info *,
                                              ACE_Addr const &)
{
  // Upcall from the AVStreams when there is data to be received from
  // the sender.
  ACE_DEBUG ((LM_DEBUG,
              "Distributer_Callback::receive_frame for frame %d\n",
              this->frame_count_++));

  Connection_Manager::Protocol_Objects &protocol_objects =
    DISTRIBUTER::instance ()->connection_manager ().protocol_objects ();

  // Send frame to all receivers.
  for (Connection_Manager::Protocol_Objects::iterator iterator = protocol_objects.begin ();
       iterator != protocol_objects.end ();
       ++iterator)
    {
      int result =
        (*iterator).int_id_->send_frame (frame);

      if (result < 0)
        ACE_ERROR_RETURN ((LM_ERROR,
                           "send failed:%p",
                           "Sender::pace_data send\n"),
                          -1);
    }

  return 0;
}

int
Distributer_Receiver_Callback::handle_destroy ()
{
  ACE_DEBUG ((LM_DEBUG,
              "Distributer_Callback::end_stream\n"));

  // We can close down now.
  DISTRIBUTER::instance ()->done (1);

  return 0;
}

Connection_Manager &
Distributer::connection_manager ()
{
  return this->connection_manager_;
}

int
Distributer::parse_args (int argc, ACE_TCHAR *argv[])
{
  // Parse command line arguments
  ACE_Get_Opt opts (argc, argv, ACE_TEXT("s:r:a:"));

  int c {};
  while ((c = opts ()) != -1)
    {
      switch (c)
        {
        case 'a':
          this->options_.addr_file = opts.opt_arg ();
          break;
        case 's':
          this->options_.sender_name = ACE_TEXT_ALWAYS_CHAR (opts.opt_arg ());
          break;
        case 'r':
          this->options_.distributer_name = ACE_TEXT_ALWAYS_CHAR (opts.opt_arg ());
          break;
        default:
          ACE_DEBUG ((LM_DEBUG,"Unknown Option\n"));
          return -1;
        }
    }
  return 0;
}

int
Distributer::init (int argc, ACE_TCHAR *argv[])
{
  // Initialize the connection class.
  int result =
    this->connection_manager_.init (TAO_AV_CORE::instance ()->orb ());
  if (result != 0)
    return result;

  // Initialize the endpoint strategy with the orb and poa.
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

  // Parse the command line arguments
  result =
    this->parse_args (argc, argv);
  if (result != 0)
    return result;

  this->connection_manager_.load_ep_addr (
    ACE_TEXT_ALWAYS_CHAR (this->options_.addr_file.c_str ()));

  TAO_MMDevice *sender_mmdevice_servant {};
  ACE_NEW_RETURN (sender_mmdevice_servant,
                  TAO_MMDevice (&this->sender_endpoint_strategy_),
                  -1);

  // Servant Reference Counting to manage lifetime
  PortableServer::ServantBase_var safe_sender_mmdevice =
    sender_mmdevice_servant;

  AVStreams::MMDevice_var distributer_sender_mmdevice =
    sender_mmdevice_servant->_this ();

  TAO_MMDevice *receiver_mmdevice_servant {};
  ACE_NEW_RETURN (receiver_mmdevice_servant,
                  TAO_MMDevice (&this->receiver_endpoint_strategy_),
                  -1);

  PortableServer::ServantBase_var safe_receiver_mmdevice =
    receiver_mmdevice_servant;

  AVStreams::MMDevice_var distributer_receiver_mmdevice =
    receiver_mmdevice_servant->_this ();

  // Bind to receivers.
  this->connection_manager_.bind_to_receivers (this->options_.distributer_name,
                                               distributer_sender_mmdevice.in ());

  // Connect to receivers
  this->connection_manager_.connect_to_receivers (distributer_sender_mmdevice.in ());

  // Bind to sender.
  this->connection_manager_.bind_to_sender (this->options_.sender_name,
                                            this->options_.distributer_name,
                                            distributer_receiver_mmdevice.in ());

  // Connect to sender.
  this->connection_manager_.connect_to_sender ();

  return 0;
}

bool
Distributer::done () const
{
  return this->done_;
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

      // Initialize the Distributer
      int result =
        DISTRIBUTER::instance ()->init (argc, argv);

      if (result != 0)
        return result;

      while (!DISTRIBUTER::instance ()->done ())
        {
          orb->perform_work ();
        }

      // Hack for now....
      ACE_OS::sleep (1);
    }
  catch (CORBA::Exception const &ex)
    {
      ex._tao_print_exception ("main");
      return -1;
    }

  DISTRIBUTER::close ();  // Explicitly finalize the Unmanaged_Singleton.

  return 0;
}

#if defined (ACE_HAS_EXPLICIT_STATIC_TEMPLATE_MEMBER_INSTANTIATION)
template ACE_Unmanaged_Singleton<Distributer, ACE_Null_Mutex> *
  ACE_Unmanaged_Singleton<Distributer, ACE_Null_Mutex>::singleton_;
#endif /* ACE_HAS_EXPLICIT_STATIC_TEMPLATE_MEMBER_INSTANTIATION */
