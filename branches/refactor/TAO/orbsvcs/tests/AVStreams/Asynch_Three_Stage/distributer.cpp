// $Id$

#include "distributer.h"
#include "ace/Get_Opt.h"
#include "orbsvcs/AV/Protocol_Factory.h"

typedef ACE_Singleton<Distributer, ACE_Null_Mutex> DISTRIBUTER;

int
Distributer_Sender_StreamEndPoint::get_callback (const char *,
                                                 TAO_AV_Callback *&callback)
{
  // Create and return the sender application callback to AVStreams
  // for further upcalls.
  callback = &this->callback_;
  return 0;
}

int
Distributer_Sender_StreamEndPoint::set_protocol_object (const char *flowname,
                                                        TAO_AV_Protocol_Object *object)
{
  Connection_Manager &connection_manager =
    DISTRIBUTER::instance ()->connection_manager ();

  // Add to the map of protocol objects.
  connection_manager.protocol_objects ().bind (flowname,
                                               object);

  // Store the related streamctrl.
  connection_manager.add_streamctrl (flowname,
                                     this);

  return 0;
}

int
Distributer_Receiver_StreamEndPoint::get_callback (const char *,
                                                   TAO_AV_Callback *&callback)
{
  // Create and return the receiver application callback to AVStreams
  // for further upcalls.
  callback = &this->callback_;
  return 0;
}

Distributer_Receiver_Callback::Distributer_Receiver_Callback (void)
  : frame_count_ (1)
{
}

int
Distributer_Receiver_Callback::receive_frame (ACE_Message_Block *frame,
                                              TAO_AV_frame_info *,
                                              const ACE_Addr &)
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
Distributer_Receiver_Callback::handle_destroy (void)
{
  ACE_DEBUG ((LM_DEBUG,
              "Distributer_Callback::end_stream\n"));

  // We can close down now.
  DISTRIBUTER::instance ()->done (1);

  return 0;
}

Distributer::Distributer (void)
  : sender_name_ ("sender")
  , distributer_name_ ("distributer")
  , done_ (0)
{
}

Distributer::~Distributer (void)
{
}

Connection_Manager &
Distributer::connection_manager (void)
{
  return this->connection_manager_;
}

int
Distributer::parse_args (int argc,
                         char **argv)
{
  // Parse command line arguments
  ACE_Get_Opt opts (argc, argv, "s:r:");

  int c;
  while ((c= opts ()) != -1)
    {
      switch (c)
        {
        case 's':
          this->sender_name_ = opts.opt_arg ();
          break;
        case 'r':
          this->distributer_name_ = opts.opt_arg ();
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
                   char ** argv
                   ACE_ENV_ARG_DECL)
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
    this->parse_args (argc,
                      argv);
  if (result != 0)
    return result;

  ACE_NEW_RETURN (this->distributer_sender_mmdevice_,
                  TAO_MMDevice (&this->sender_endpoint_strategy_),
                  -1);

  // Servant Reference Counting to manage lifetime
  PortableServer::ServantBase_var safe_sender_mmdevice =
    this->distributer_sender_mmdevice_;

  AVStreams::MMDevice_var distributer_sender_mmdevice =
    this->distributer_sender_mmdevice_->_this (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK_RETURN (-1);

  ACE_NEW_RETURN (this->distributer_receiver_mmdevice_,
                  TAO_MMDevice (&this->receiver_endpoint_strategy_),
                  -1);


  AVStreams::MMDevice_var distributer_receiver_mmdevice =
    this->distributer_receiver_mmdevice_->_this (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK_RETURN (-1);

  // Bind to receivers.
  this->connection_manager_.bind_to_receivers (this->distributer_name_,
                                               distributer_sender_mmdevice.in ()
                                               ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (-1);

  // Connect to receivers
  this->connection_manager_.connect_to_receivers (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK_RETURN (-1);

  // Bind to sender.
  this->connection_manager_.bind_to_sender (this->sender_name_,
                                            this->distributer_name_,
                                            distributer_receiver_mmdevice.in ()
                                            ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (-1);

  // Connect to sender.
  this->connection_manager_.connect_to_sender (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK_RETURN (-1);

  return 0;
}

int
Distributer::done (void) const
{
  return this->done_;
}

void
Distributer::done (int done)
{
  this->done_ = done;
}

int
main (int argc,
      char **argv)
{
  ACE_DECLARE_NEW_CORBA_ENV;
  ACE_TRY
    {
      // Initialize the ORB first.
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

      // Get the POA_var object from Object_var.
      PortableServer::POA_var root_poa =
        PortableServer::POA::_narrow (obj.in ()
                                      ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      PortableServer::POAManager_var mgr
        = root_poa->the_POAManager (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;

      mgr->activate (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;

      // Initialize the AVStreams components.
      TAO_AV_CORE::instance ()->init (orb.in (),
                                      root_poa.in ()
                                      ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      // Initialize the Distributer
      int result =
        DISTRIBUTER::instance ()->init (argc,
                                        argv
                                        ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      if (result != 0)
        return result;

      while (!DISTRIBUTER::instance ()->done ())
        {
          orb->perform_work (ACE_ENV_SINGLE_ARG_PARAMETER);
          ACE_TRY_CHECK;
        }

      // Hack for now....
      ACE_OS::sleep (1);
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION,"main");
      return -1;
    }
  ACE_ENDTRY;
  ACE_CHECK_RETURN (-1);

  return 0;
}

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)
template class ACE_Singleton <Distributer,ACE_Null_Mutex>;
template class TAO_AV_Endpoint_Reactive_Strategy_A<Distributer_Sender_StreamEndPoint,   TAO_VDev, AV_Null_MediaCtrl>;
template class TAO_AV_Endpoint_Reactive_Strategy  <Distributer_Sender_StreamEndPoint,   TAO_VDev, AV_Null_MediaCtrl>;
template class TAO_AV_Endpoint_Reactive_Strategy_B<Distributer_Receiver_StreamEndPoint, TAO_VDev, AV_Null_MediaCtrl>;
template class TAO_AV_Endpoint_Reactive_Strategy  <Distributer_Receiver_StreamEndPoint, TAO_VDev, AV_Null_MediaCtrl>;
#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)
#pragma instantiate ACE_Singleton <Distributer,ACE_Null_Mutex>
#pragma instantiate TAO_AV_Endpoint_Reactive_Strategy_A<Distributer_Sender_StreamEndPoint,   TAO_VDev, AV_Null_MediaCtrl>
#pragma instantiate TAO_AV_Endpoint_Reactive_Strategy  <Distributer_Sender_StreamEndPoint,   TAO_VDev, AV_Null_MediaCtrl>
#pragma instantiate TAO_AV_Endpoint_Reactive_Strategy_B<Distributer_Receiver_StreamEndPoint, TAO_VDev, AV_Null_MediaCtrl>
#pragma instantiate TAO_AV_Endpoint_Reactive_Strategy  <Distributer_Receiver_StreamEndPoint, TAO_VDev, AV_Null_MediaCtrl>
#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */
