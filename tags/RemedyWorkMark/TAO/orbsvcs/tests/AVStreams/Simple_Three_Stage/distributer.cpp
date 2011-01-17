// $Id$

#include "distributer.h"
#include "ace/Get_Opt.h"
#include "orbsvcs/AV/Protocol_Factory.h"

// An Unmanaged_Singleton is used to avoid static object destruction
// order related problems since the underlying singleton object
// contains references to static TypeCodes.

typedef ACE_Unmanaged_Singleton<Distributer, ACE_Null_Mutex> DISTRIBUTER;

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
Distributer_Sender_StreamEndPoint::set_protocol_object (const char *,
                                                        TAO_AV_Protocol_Object *object)
{
  // Set the protocol object corresponding to the transport protocol
  // selected.
  DISTRIBUTER::instance ()->sender_protocol_object (object);

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

  // Get the protocol object corresponding to the receiver stream and
  // send the data received from sender to the receiver.
  int result =
    DISTRIBUTER::instance ()->sender_protocol_object ()->send_frame (frame);

  if (result < 0)
    ACE_ERROR_RETURN ((LM_ERROR,
                       "Distributer_Callback::receive_frame send failed\n"),
                      -1);

  return 0;
}

int
Distributer_Receiver_Callback::handle_destroy (void)
{
  // Called when the sender requests the stream to be shutdown.
  try
    {
      ACE_DEBUG ((LM_DEBUG,
                  "Distributer_Callback::end_stream\n"));

      // Destroy the receiver stream
      AVStreams::flowSpec stop_spec;
      DISTRIBUTER::instance ()->receiver_streamctrl ()->destroy (stop_spec);

      // We can close down now.
      DISTRIBUTER::instance ()->done (1);
    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception (
        "Distributer_Callback::handle_destroy Failed\n");
      return -1;
    }

  return 0;
}

int
Distributer::sender_protocol_object (TAO_AV_Protocol_Object *object)
{
  // Set the corresponding protocol objects for the different streams created.
  this->sender_protocol_object_ = object;

  return 0;
}

TAO_AV_Protocol_Object *
Distributer::sender_protocol_object (void)
{
  return this->sender_protocol_object_;
}

Distributer::Distributer (void)
  : distributer_receiver_mmdevice_ (0),
    distributer_sender_mmdevice_ (0),
    sender_protocol_object_ (0),
    receiver_streamctrl_ (0),
    protocol_ ("UDP"),
    done_ (0)
{
}

Distributer::~Distributer (void)
{
}

void
Distributer::bind_to_mmdevice (AVStreams::MMDevice_ptr &mmdevice,
                               const ACE_CString &mmdevice_name)
{
  CosNaming::Name name (1);
  name.length (1);
  name [0].id =
    CORBA::string_dup (mmdevice_name.c_str ());

  // Resolve the mmdevice object reference from the Naming Service
  CORBA::Object_var mmdevice_obj =
    this->naming_client_->resolve (name);

  mmdevice =
    AVStreams::MMDevice::_narrow (mmdevice_obj.in ());
}

int
Distributer::init (int /*argc*/,
                   ACE_TCHAR *[] /*argv*/
                   )
{
  // Initialize the naming services
  int result =
    this->naming_client_.init (TAO_AV_CORE::instance ()->orb ());
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

  // Bind to the receiver mmdevice
  ACE_CString mmdevice_name ("Receiver");
  this->bind_to_mmdevice (this->receiver_mmdevice_.out (),
                          mmdevice_name);

  // Bind to the sender mmdevice
  mmdevice_name = "Sender";
  this->bind_to_mmdevice (this->sender_mmdevice_.out (),
                          mmdevice_name);

  // Initialize the  QoS
  AVStreams::streamQoS_var the_qos (new AVStreams::streamQoS);

  // Create the forward flow specification to describe the flow.
  TAO_Forward_FlowSpec_Entry receiver_entry ("Data_Receiver",
                                             "IN",
                                             "USER_DEFINED",
                                             "",
                                             this->protocol_.c_str (),
                                             0);

  // Set the flow specification for the stream between receiver and distributer
  AVStreams::flowSpec flow_spec (1);
  flow_spec.length (1);
  flow_spec [0] =
    CORBA::string_dup (receiver_entry.entry_to_string ());

  ACE_NEW_RETURN (this->distributer_sender_mmdevice_,
                  TAO_MMDevice (&this->sender_endpoint_strategy_),
                  -1);

  // Servant Reference Counting to manage lifetime
  PortableServer::ServantBase_var safe_sender_mmdevice =
    this->distributer_sender_mmdevice_;

  AVStreams::MMDevice_var distributer_sender_mmdevice =
    this->distributer_sender_mmdevice_->_this ();

  ACE_NEW_RETURN (this->distributer_receiver_mmdevice_,
                  TAO_MMDevice (&this->receiver_endpoint_strategy_),
                  -1);

  // Servant Reference Counting to manage lifetime
  PortableServer::ServantBase_var safe_receiver_mmdevice =
    this->distributer_receiver_mmdevice_;

  AVStreams::MMDevice_var distributer_receiver_mmdevice =
    this->distributer_receiver_mmdevice_->_this ();

  ACE_NEW_RETURN (this->receiver_streamctrl_,
                  TAO_StreamCtrl,
                  -1);

  // Servant Reference Counting to manage lifetime
  PortableServer::ServantBase_var safe_receiver_streamctrl =
    this->receiver_streamctrl_;

  // Bind/Connect the distributer and receiver MMDevices.
  result =
    this->receiver_streamctrl_->bind_devs (distributer_sender_mmdevice.in (),
                                           this->receiver_mmdevice_.in (),
                                           the_qos.inout (),
                                           flow_spec);

  // Create the forward flow specification to describe the flow.
  TAO_Forward_FlowSpec_Entry sender_entry ("Data_Sender",
                                           "OUT",
                                           "USER_DEFINED",
                                           "", // Flowname
                                           this->protocol_.c_str (),
                                           0);

  TAO_StreamCtrl* sender_streamctrl = 0;
  // Video stream controller for the stream between sender and distributer

  ACE_NEW_RETURN (sender_streamctrl,
                  TAO_StreamCtrl,
                  -1);

  // Servant Reference Counting to manage lifetime
  PortableServer::ServantBase_var safe_sender_streamctrl =
    sender_streamctrl;

  // Set the flow specification for the stream between sender and distributer
  flow_spec [0] = CORBA::string_dup (sender_entry.entry_to_string ());

  // Bind/Connect the sender and distributer MMDevices.
  CORBA::Boolean res =
    sender_streamctrl->bind_devs (sender_mmdevice_.in (),
                                  distributer_receiver_mmdevice.in (),
                                  the_qos.inout (),
                                  flow_spec);

  if (res == 0)
    ACE_ERROR_RETURN ((LM_ERROR,"Streamctrl::bind_devs failed\n"),-1);

  return 0;
}

TAO_StreamCtrl *
Distributer::receiver_streamctrl (void)
{
  return this->receiver_streamctrl_;
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

      // Initialize the Distributer
      int result =
        DISTRIBUTER::instance ()->init (argc,
                                        argv);

      if (result != 0)
        return result;

      while (!DISTRIBUTER::instance ()->done ())
        {
          orb->perform_work ();
        }

      // Hack for now....
      ACE_OS::sleep (1);
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
