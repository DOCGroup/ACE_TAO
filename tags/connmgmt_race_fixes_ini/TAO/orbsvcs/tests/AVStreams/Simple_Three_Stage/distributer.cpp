// $Id$

#include "distributer.h"
#include "ace/Get_Opt.h"
#include "orbsvcs/AV/Protocol_Factory.h"

int first = 1;

int
Distributer_StreamEndPoint::set_protocol_object (const char *flow_name,
						 TAO_AV_Protocol_Object *object)
{
  if (ACE_OS::strcmp (flow_name, "Data_Sender") == 0)
    {
      // Set the protocol object corresponding to the transport protocol selected.
      DISTRIBUTER::instance ()->set_sender_protocol_object (object);
    }
  
  // Store the flowname of the stream that this callback belongs to.
  this->callback_.flowname (flow_name);

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
Distributer_Callback::flowname (const ACE_CString &flowname)
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

  // Get the protocol object corresponding to the receiver stream
  // send the data received from sender to the receiver.

  int result = DISTRIBUTER::instance ()->get_sender_protocol_object ()->send_frame (frame);
  
  if (result < 0)
    ACE_ERROR_RETURN ((LM_ERROR,
                       "Distributer_Callback::receive_frame send failed\n"),
                      -1);
  
  return 0;
}

int
Distributer_Callback::handle_destroy (void)
{
  // Called when the sender requests the stream to be shutdown.
  ACE_TRY_NEW_ENV
    {
      
      ACE_DEBUG ((LM_DEBUG,
                  "Distributer_Callback::end_stream\n"));
      
      if (ACE_OS::strcmp (this->flowname_.c_str (), "Data_Sender") == 0)
        {
          // Destroy the sender receiver stream as the sender has requested a stream destroy
          AVStreams::flowSpec stop_spec;
          DISTRIBUTER::instance ()->get_receiver_streamctrl ()->destroy (stop_spec,
                                                                         ACE_TRY_ENV);
          ACE_TRY_CHECK;
        }
      
      // Decrement the stream count.
      DISTRIBUTER::instance ()->stream_destroyed ();
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION,
			   "Distributer_Callback::handle_destroy Failed\n");
      return -1;
    }
  ACE_ENDTRY;

  return 0;
}

int
Distributer::set_sender_protocol_object (TAO_AV_Protocol_Object* object)
{
  // Set the corresponding protocol objects for the different streams created.
  this->sender_protocol_object_ = object;

  return 0;
}

TAO_AV_Protocol_Object*
Distributer::get_sender_protocol_object (void)
{
  return this->sender_protocol_object_;
}


Distributer::Distributer (void)
  :distributer_mmdevice_ (0),
   sender_protocol_object_ (0),
   count_ (0),
   protocol_ ("UDP"),
   receiver_streamctrl_ (0),
   stream_count_ (0),
   done_ (0),
   host_sender_port_ ("8000"),
   host_receiver_port_ ("8010")
{

  // Get the local host name
  char buf [BUFSIZ];
  ACE_OS::hostname (buf, 
                    BUFSIZ);

  // Set the address to the local host and port.
  this->sender_address_ = buf;
  this->sender_address_ += ":";
  this->sender_address_ += host_sender_port_.c_str ();

  // Set the address to the local host and port.
  this->receiver_address_ = buf;
  this->receiver_address_ += ":";
  this->receiver_address_ += host_receiver_port_.c_str ();
  
}

Distributer::~Distributer (void)
{
}

int
Distributer::parse_args (int argc,
                         char **argv)
{
  // Parse command line arguments
  ACE_Get_Opt opts (argc,argv,"r:s:");

  int c;
  while ((c= opts ()) != -1)
    {
      switch (c)
        {
        case 'r':
          this->host_receiver_port_ = opts.optarg;
          break;
	case 's':
	  this->host_sender_port_ = opts.optarg;
	  break;
        default:
          ACE_DEBUG ((LM_DEBUG,"Unknown Option\n"));
          return -1;
        }
    }
  return 0;
}


// Method to bind the sender reference to the Naming Service.
int
Distributer::bind_to_mmdevice (AVStreams::MMDevice_ptr &mmdevice,
			       const ACE_CString &mmdevice_name,
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
Distributer::init (int argc,
                   char **argv,
                   CORBA::Environment &ACE_TRY_ENV)
{

  // Initialize the endpoint strategy with the orb and poa.
  int result =
    this->a_endpoint_strategy_.init (TAO_AV_CORE::instance ()->orb (),
                                     TAO_AV_CORE::instance ()->poa ());
  if (result != 0)
    return result;

  result = this->parse_args (argc, 
                             argv);

  if (result < 0)
    ACE_ERROR_RETURN ((LM_ERROR,
		       " (%P|%t) Error in Parse Args \n"),
		      -1);
  

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
  
  // Initialize the  QoS
  AVStreams::streamQoS_var the_qos (new AVStreams::streamQoS);
  
  // Set the address of the of the distributer receiver endpoint.
  ACE_INET_Addr receiver_addr (this->receiver_address_.c_str ());

  // Create the forward flow specification to describe the flow.
  TAO_Forward_FlowSpec_Entry receiver_entry ("Data_Receiver",
					     "IN",
					     "USER_DEFINED",
					     "", // Flowname
					     this->protocol_.c_str (),
					     &receiver_addr);

  // Set the flow specification for the stream between receiver and distributer 
  AVStreams::flowSpec flow_spec (1);
  flow_spec.length (1);
  flow_spec [0] = CORBA::string_dup (receiver_entry.entry_to_string ());
  
  ACE_NEW_RETURN (this->distributer_mmdevice_,
                  TAO_MMDevice (&this->a_endpoint_strategy_),
                  -1);
  
  // Servant Reference Counting to manage lifetime
  PortableServer::ServantBase_var safe_mmdevice =
    this->distributer_mmdevice_;
    
  AVStreams::MMDevice_var distributer_mmdevice = 
    this->distributer_mmdevice_->_this (ACE_TRY_ENV);
  ACE_CHECK_RETURN (-1);

  ACE_NEW_RETURN (this->receiver_streamctrl_,
                  TAO_StreamCtrl,
                  -1);
  
  // Servant Reference Counting to manage lifetime
  PortableServer::ServantBase_var safe_receiver_streamctrl =
    this->receiver_streamctrl_;
  
  // Bind/Connect  the distributer and receiver MMDevices.
  result =
    this->receiver_streamctrl_->bind_devs (distributer_mmdevice.in (),
                                          this->receiver_mmdevice_.in (),
                                          the_qos.inout (),
                                          flow_spec,
                                          ACE_TRY_ENV);
  ACE_CHECK_RETURN (-1);

  // Get the local host name
  char buf [BUFSIZ];
  ACE_OS::hostname (buf, 
                    BUFSIZ);

  // Set the address of the sender.
  ACE_INET_Addr sender_addr (this->sender_address_.c_str ());
  
  // Create the forward flow specification to describe the flow.
  TAO_Forward_FlowSpec_Entry sender_entry ("Data_Sender",
					   "OUT",
					   "USER_DEFINED",
					   "", // Flowname
					   this->protocol_.c_str (),
					   &sender_addr);

  TAO_StreamCtrl* sender_streamctrl_;
  // Video stream controller for the stream between sender and distributer
  
  ACE_NEW_RETURN (sender_streamctrl_,
                  TAO_StreamCtrl,
                  -1);

  // Servant Reference Counting to manage lifetime
  PortableServer::ServantBase_var safe_sender_streamctrl =
    sender_streamctrl_;

  // Set the flow specification for the stream between sender and distributer
  flow_spec [0] = CORBA::string_dup (sender_entry.entry_to_string ());

  // Bind/Connect  the sender and sitributer MMDevices.
  CORBA::Boolean res =
    sender_streamctrl_->bind_devs (distributer_mmdevice.in (),  
                                   sender_mmdevice_.in (),
                                   the_qos.inout (),
                                   flow_spec,
                                   ACE_TRY_ENV);
  ACE_CHECK_RETURN (-1);
  
  if (res == 0)
    ACE_ERROR_RETURN ((LM_ERROR,"Streamctrl::bind_devs failed\n"),-1);
    
  AVStreams::flowSpec start_spec;
  sender_streamctrl_->start (start_spec,
                             ACE_TRY_ENV);
  ACE_CHECK_RETURN (-1);
  
  return 0;
}

TAO_StreamCtrl*
Distributer::get_receiver_streamctrl (void)
{
  return this->receiver_streamctrl_;
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
      while (!DISTRIBUTER::instance ()->done ())
        {
          orb->perform_work (ACE_TRY_ENV);
          ACE_TRY_CHECK;
        }
      
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
template class TAO_AV_Endpoint_Reactive_Strategy_A<Distributer_StreamEndPoint,TAO_VDev,AV_Null_MediaCtrl>;
template class TAO_AV_Endpoint_Reactive_Strategy<Distributer_StreamEndPoint,TAO_VDev,AV_Null_MediaCtrl>;
#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)
#pragma instantiate ACE_Singleton <Client,ACE_Null_Mutex>
#pragma instantiate TAO_AV_Endpoint_Reactive_Strategy_A<Distributer_StreamEndPoint,TAO_VDev,AV_Null_MediaCtrl>
#pragma instantiate TAO_AV_Endpoint_Reactive_Strategy<Distributer_StreamEndPoint,TAO_VDev,AV_Null_MediaCtrl>
#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */
