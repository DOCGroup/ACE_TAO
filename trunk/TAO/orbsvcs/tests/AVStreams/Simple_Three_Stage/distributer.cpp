// $Id$

#include "distributer.h"
#include "ace/Get_Opt.h"
#include "orbsvcs/AV/Protocol_Factory.h"

int first = 1;

int
Distributer_StreamEndPoint::set_protocol_object (const char *flow_name,
						 TAO_AV_Protocol_Object *object)
{
  // Set the protocol object corresponding to the transport protocol selected.
  DISTRIBUTER::instance ()->set_protocol_object (flow_name, object);

  // Store the flowname of the stream that this callback belongs to.
  ACE_CString fname = flow_name;
  this->callback_.flowname (fname);

  // Increment the stream count.
  DISTRIBUTER::instance ()->stream_created ();
  return 0;
}


int
Distributer_StreamEndPoint::get_callback (const char *,
                                     TAO_AV_Callback *&callback)
{
  // Create and return the application callback and return to the AVStreams
  // for further upcalls.
  callback = &this->callback_;
  return 0;
}

void
Distributer_Callback::flowname (ACE_CString flowname)
{
  this->flowname_ = flowname;
}


ACE_CString
Distributer_Callback::flowname (void)
{
  return this->flowname_;
}

int
Distributer_Callback::receive_frame (ACE_Message_Block *frame,
				     TAO_AV_frame_info *,
				     const ACE_Addr &)
{
  // Upcall from the AVStreams when there is data to be received from the
  // sender.
  ACE_DEBUG ((LM_DEBUG,
              "Distributer_Callback::receive_frame\n"));

  ACE_CString flowname = "Data_Receiver";
  
  // Get the protocol object corresponding to the receiver stream
  // send the data received from sender to the receiver.
  DISTRIBUTER::instance ()->get_protocol_object (flowname.c_str())->send_frame (frame);
  
  return 0;
}

int
Distributer_Callback::handle_destroy (void)
{
  // Called when the sender requests the stream to be shutdown.
  ACE_DECLARE_NEW_CORBA_ENV;

  ACE_DEBUG ((LM_DEBUG,
              "Distributer_Callback::end_stream\n"));

  if (ACE_OS::strcmp (this->flowname_.c_str (), "Data_Sender") == 0)
    {
      // Destroy the sender receiver stream as the sender has requested a stream destroy
      AVStreams::flowSpec stop_spec;
      DISTRIBUTER::instance ()->get_receiver_streamctrl ()->destroy (stop_spec,
                                                                     ACE_TRY_ENV);
    }
  
  // Decrement the stream count.
  DISTRIBUTER::instance ()->stream_destroyed ();
  return 0;
}

int
Distributer::set_protocol_object (const char* flowname, TAO_AV_Protocol_Object* object)
{
  // Set the corresponding protocol objects for the different streams created.
  if (ACE_OS::strcmp ("Data_Sender", flowname) == 0)
    this->protocol_object_ [0] = object;
  else if (ACE_OS::strcmp ("Data_Receiver", flowname) == 0)
    this->protocol_object_ [1] = object;

  return 0;
}

TAO_AV_Protocol_Object*
Distributer::get_protocol_object (const char* flowname)
{
  if (ACE_OS::strcmp ("Data_Sender", flowname) == 0)
    return this->protocol_object_ [0];
  else if (ACE_OS::strcmp ("Data_Receiver", flowname) == 0)
    return this->protocol_object_ [1];

  return *(this->protocol_object_);
}


Distributer::Distributer (void)
  : distributer_mmdevice_ (&a_endpoint_strategy_),
    count_ (0),
    protocol_ ("UDP"),
    stream_count_ (0),
    done_ (0)
{
  
  // Get the local host name
  char buf [BUFSIZ];
  ACE_OS::hostname (buf, 
                    BUFSIZ);

  // Set the address to the local host and port.
  this->address_ = buf;
  this->address_ += ":8000";
  
  protocol_object_ [0] = 0;
  protocol_object_ [1] = 0;
  this->mb.size (BUFSIZ);
}

Distributer::~Distributer (void)
{
}


// Method to bind the sender reference to the Naming Service.
int
Distributer::bind_to_mmdevice (AVStreams::MMDevice_ptr &mmdevice,
			       ACE_CString mmdevice_name,
			       CORBA::Environment &ACE_TRY_ENV)
{
  // Initialize the naming services
  if (my_naming_client_.init (TAO_AV_CORE::instance ()->orb ()) != 0)
    ACE_ERROR_RETURN ((LM_ERROR,
		       " (%P|%t) Unable to initialize "
		       "the TAO_Naming_Client. \n"),
		      -1);
  
  CosNaming::Name mmdevice_naming_name (1);
  mmdevice_naming_name.length (1);
  mmdevice_naming_name [0].id = CORBA::string_dup (mmdevice_name.c_str ());
  
  // Resolve the mmdevice object reference from the Naming Service
  CORBA::Object_var mmdevice_obj =
    my_naming_client_->resolve (mmdevice_naming_name,
				ACE_TRY_ENV);
  ACE_CHECK_RETURN (-1);
  
  mmdevice =
    AVStreams::MMDevice::_narrow (mmdevice_obj.in (),
				  ACE_TRY_ENV);
  ACE_CHECK_RETURN (-1);
  
  if (CORBA::is_nil (mmdevice))
    ACE_ERROR_RETURN ((LM_ERROR,
		       "Could not resolve MMdevice in Naming service <%s>\n"),
                      -1);
  return 0;
}


int
Distributer::init (int,
              char **,
              CORBA::Environment &ACE_TRY_ENV)
{
  // Initialize the endpoint strategy with the orb and poa.
  int result =
    this->a_endpoint_strategy_.init (TAO_AV_CORE::instance ()->orb (),
                                     TAO_AV_CORE::instance ()->poa ());
  if (result != 0)
    return result;

  // Bind to the receiver mmdevice
  ACE_CString mmdevice_name ("Receiver");
  result = bind_to_mmdevice (this->receiver_mmdevice_.inout (),
			     mmdevice_name,
			     ACE_TRY_ENV);
  ACE_CHECK_RETURN (-1);
  
  if (result != 0)
    return result;

  // Bind to the sender mmdevice
  mmdevice_name = "Sender";
  result = bind_to_mmdevice (this->sender_mmdevice_.inout (),
			     mmdevice_name,
			     ACE_TRY_ENV);
  ACE_CHECK_RETURN (-1);

  if (result != 0)
    return result;
  
  // Create the Flow protocol name
  ACE_CString flow_protocol_str;
  if (this->use_sfp_)
    flow_protocol_str = "sfp:1.0";
  else
    flow_protocol_str = "";
  
  // Initialize the  QoS
  AVStreams::streamQoS_var the_qos (new AVStreams::streamQoS);
  
  // Set the address of the of the distributer receiver endpoint.
  ACE_INET_Addr receiver_addr (this->address_.c_str ());

  this->receiver_flowname_ = "Data_Receiver";
  
  // Create the forward flow specification to describe the flow.
  TAO_Forward_FlowSpec_Entry receiver_entry (this->receiver_flowname_.c_str (),
					     "IN",
					     "USER_DEFINED",
					     flow_protocol_str.c_str (),
					     this->protocol_.c_str (),
					     &receiver_addr);

  // Set the flow specification for the stream between receiver and distributer 
  AVStreams::flowSpec flow_spec (1);
  flow_spec.length (1);
  flow_spec [0] = CORBA::string_dup (receiver_entry.entry_to_string ());
  
  AVStreams::MMDevice_var distributer_mmdevice = this->distributer_mmdevice_._this (ACE_TRY_ENV);
  ACE_CHECK_RETURN (-1);

  // Bind/Connect  the distributer and receiver MMDevices.
  result =
    this->receiver_streamctrl_.bind_devs (distributer_mmdevice.in (),
                                          this->receiver_mmdevice_.in (),
                                          the_qos.inout (),
                                          flow_spec,
                                          ACE_TRY_ENV);
  ACE_CHECK_RETURN (-1);

  // Flowname for the stream bewteen the sender and distributer
  this->sender_flowname_ = "Data_Sender";

  // Get the local host name
  char buf [BUFSIZ];
  ACE_OS::hostname (buf, 
                    BUFSIZ);

  // Set the address to the local host and port
  this->address_ = buf;
  this->address_ += ":8001";
    
  // Set the address of the sender.
  ACE_INET_Addr sender_addr (this->address_.c_str ());
  
  // Create the forward flow specification to describe the flow.
  TAO_Forward_FlowSpec_Entry sender_entry (this->sender_flowname_.c_str (),
					   "OUT",
					   "USER_DEFINED",
					   flow_protocol_str.c_str (),
					   this->protocol_.c_str (),
					   &sender_addr);

  // Set the flow specification for the stream between sender and distributer
  flow_spec [0] = CORBA::string_dup (sender_entry.entry_to_string ());
  
  // Bind/Connect  the sender and sitributer MMDevices.
  CORBA::Boolean res =
    this->sender_streamctrl_.bind_devs (distributer_mmdevice.in (),  
                                        this->sender_mmdevice_.in (),
                                        the_qos.inout (),
                                        flow_spec,
                                        ACE_TRY_ENV);
  ACE_CHECK_RETURN (-1);
  
  if (res == 0)
    ACE_ERROR_RETURN ((LM_ERROR,"Streamctrl::bind_devs failed\n"),-1);
    
  AVStreams::flowSpec start_spec;
  this->sender_streamctrl_.start (start_spec,ACE_TRY_ENV);
  ACE_CHECK_RETURN (-1);
  
  return 0;
}

TAO_StreamCtrl*
Distributer::get_receiver_streamctrl (void)
{
  return &this->receiver_streamctrl_;
}

TAO_StreamCtrl*
Distributer::get_sender_streamctrl (void)
{
  return &this->sender_streamctrl_;
}


void 
Distributer::stream_created (void)
{
  this->stream_count_++;
}

void 
Distributer::stream_destroyed (void)
{
  this->stream_count_--;
  
  if (this->stream_count_ == 0)
    this->done_ = 1;
}

int
Distributer::done (void)
{
  return this->done_;
}


int
main (int argc,
      char **argv)
{
  ACE_DECLARE_NEW_CORBA_ENV;
  ACE_TRY
    {
      // Initialize the ORB first.
      CORBA::ORB_var orb = CORBA::ORB_init (argc,
                                            argv,
                                            0,
                                            ACE_TRY_ENV);
      ACE_TRY_CHECK;


      CORBA::Object_var obj
        = orb->resolve_initial_references ("RootPOA",
                                           ACE_TRY_ENV);
      ACE_TRY_CHECK;

      // Get the POA_var object from Object_var.
      PortableServer::POA_var root_poa =
        PortableServer::POA::_narrow (obj.in (),
                                      ACE_TRY_ENV);
      ACE_TRY_CHECK;

      PortableServer::POAManager_var mgr
        = root_poa->the_POAManager (ACE_TRY_ENV);
      ACE_TRY_CHECK;

      mgr->activate (ACE_TRY_ENV);
      ACE_TRY_CHECK;

      // Initialize the AVStreams components.
      TAO_AV_CORE::instance ()->init (orb.in (),
                                      root_poa.in (),
                                      ACE_TRY_ENV);
      ACE_TRY_CHECK;

      // Initialize the Distributer
      int result = DISTRIBUTER::instance ()->init (argc,
					       argv,
					       ACE_TRY_ENV);
      ACE_TRY_CHECK;
      
      if (result != 0)
        return result;
  
      // run the orb till the streams are not destroyed.
      while (!DISTRIBUTER::instance ()->done () && orb->work_pending ())
        orb->perform_work (ACE_TRY_ENV);
      
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
template class TAO_AV_Endpoint_Reactive_Strategy_A
<Distributer_StreamEndPoint,TAO_VDev,AV_Null_MediaCtrl>;
template class TAO_AV_Endpoint_Reactive_Strategy
<Distributer_StreamEndPoint,TAO_VDev,AV_Null_MediaCtrl>;
#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)
#pragma instantiate ACE_Singleton <Client,ACE_Null_Mutex>
#pragma instantiate TAO_AV_Endpoint_Reactive_Strategy_A
<Distributer_StreamEndPoint,TAO_VDev,AV_Null_MediaCtrl>
#pragma instantiate TAO_AV_Endpoint_Reactive_Strategy
<Distributer_StreamEndPoint,TAO_VDev,AV_Null_MediaCtrl>
#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */
