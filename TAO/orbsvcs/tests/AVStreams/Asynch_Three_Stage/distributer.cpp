// $Id$

#include "distributer.h"
#include "ace/Get_Opt.h"
#include "orbsvcs/AV/Protocol_Factory.h"

int first = 1;

int
Distributer_Receiver_StreamEndPoint::set_protocol_object (const char *flow_name,
                                                          TAO_AV_Protocol_Object */*object*/)
{
  // Store the flowname of the stream that this callback belongs to.
  ACE_CString fname = flow_name;
  this->callback_.flowname (fname);
  
  // Increment the stream count.
  DISTRIBUTER::instance ()->stream_created ();
  return 0;
}

int
Distributer_Receiver_StreamEndPoint::get_callback (const char *,
                                                   TAO_AV_Callback *&callback)
{
  // Create and return the application callback and return to the AVStreams
  // for further upcalls.
  callback = &this->callback_;
  return 0;
}

int
Distributer_Sender_StreamEndPoint::set_protocol_object (const char *flow_name,
                                                        TAO_AV_Protocol_Object *object)
{
  // Set the protocol object corresponding to the transport protocol selected.
  DISTRIBUTER::instance ()->set_protocol_object (flow_name, object);
  
  // Store the flowname of the stream that this callback belongs to.
  ACE_CString fname = flow_name;
  this->callback_.flowname (fname);

  // Store the flowname and the corresponding stream endpoint for the receiver
  DISTRIBUTER::instance ()->set_endpoint (flow_name,
                                          this);
  
  // Increment the stream count.
  DISTRIBUTER::instance ()->stream_created ();
  return 0;
}


void 
Distributer::set_endpoint (const char* flowname, TAO_StreamEndPoint* endpoint)
{
  // Set the flowname and teh corresponding endpoint.
  StreamEndpoint_Item* item;
  
  ACE_NEW (item,
           StreamEndpoint_Item);
    
  item->flowname = flowname;
  
  item->endpoint = endpoint;
  
  this->endpoint_set_.insert (item);

}

int
Distributer_Sender_StreamEndPoint::get_callback (const char *,
                                                   TAO_AV_Callback *&callback)
{
  // Create and return the application callback and return to the AVStreams
  // for further upcalls.
  callback = &this->callback_;
  return 0;
}

void
Distributer_Receiver_Callback::flowname (ACE_CString flowname)
{
  this->flowname_ = flowname;
}


ACE_CString
Distributer_Receiver_Callback::flowname (void)
{
  return this->flowname_;
}

int
Distributer_Receiver_Callback::receive_frame (ACE_Message_Block *frame,
                                              TAO_AV_frame_info *,
                                              const ACE_Addr &)
{
  // Upcall from the AVStreams when there is data to be received from the
  // sender.
  ACE_DEBUG ((LM_DEBUG,
              "Distributer_Callback::receive_frame\n"));


  int result = 0;

  Protocol_Object_Set_Itor end = DISTRIBUTER::instance ()->protocol_object_set ()->end ();
  for (Protocol_Object_Set_Itor begin = DISTRIBUTER::instance ()->protocol_object_set ()->begin ();
       begin != end; ++begin)
    {
      // Get the protocol object corresponding to the receiver stream
      // send the data received from sender to the receivers.
      result = (*begin)->protocol_object->send_frame (frame);
    }
  
  if (result < 0)
    ACE_ERROR_RETURN ((LM_ERROR,
                       "send failed:%p","Distributer::Callback\n"),
                      -1);
  
  return 0;
}

int
Distributer_Receiver_Callback::handle_destroy (void)
{
  // Called when the sender requests the stream to be shutdown.
  ACE_DECLARE_NEW_CORBA_ENV;
  ACE_DEBUG ((LM_DEBUG,
              "Distributer_Receiver_Callback::end_stream\n"));
  
  // Disconnect all receivers the sender is sending to
  DISTRIBUTER::instance ()->helper ()->disconnect (DISTRIBUTER::instance ()->get_endpoint_set (),
                                                   DISTRIBUTER::instance ()->get_streamctrl_map (),
                                                   ACE_TRY_ENV);
  // Decrement the stream count.
  DISTRIBUTER::instance ()->stream_destroyed ();
  return 0;
}


void
Distributer_Sender_Callback::flowname (ACE_CString flowname)
{
  this->flowname_ = flowname;
}


ACE_CString
Distributer_Sender_Callback::flowname (void)
{
  return this->flowname_;
}

int
Distributer_Sender_Callback::handle_destroy (void)
{
  // Called when the sender requests the stream to be shutdown.
  //   ACE_DECLARE_NEW_CORBA_ENV;
  ACE_DEBUG ((LM_DEBUG,
              "Distributer_Sender_Callback::end_stream\n"));

  // Decrement the stream count.
  DISTRIBUTER::instance ()->stream_destroyed ();
  
  return 0;
}

Protocol_Object_Set*
Distributer::protocol_object_set (void)
{
  return &this->protocol_object_set_;
}

void
Distributer::protocol_object_set (Protocol_Object_Set* protocol_object_set)
{
  this->protocol_object_set_ = *protocol_object_set;
}

int
Distributer::set_protocol_object (const char* flowname, TAO_AV_Protocol_Object* object)
{
  // Set the corresponding protocol objects for the different streams created.
  // and store them in a protocol object list
  Protocol_Object_Item* item;
  
  ACE_NEW_RETURN (item,
                  Protocol_Object_Item,
                  -1);

  item->flowname = flowname;

  item->protocol_object = object;

  this->protocol_object_set_.insert (item);

  return 0;
}


Distributer::Distributer (void)
  : count_ (0),
    protocol_ ("UDP"),
    stream_count_ (0),
    done_ (0),
    default_port (9000)
{
  
  this->mb.size (BUFSIZ);
}

Distributer::~Distributer (void)
{
}

Connection_Setup_Helper*
Distributer::helper (void)
{
  return &this->helper_;
}

int
Distributer::parse_args (int argc,
                         char **argv)
{
  // Parse command line arguments
  ACE_Get_Opt opts (argc,argv,"s:r:");

  int c;
  while ((c= opts ()) != -1)
    {
      switch (c)
        {
        case 's':
          this->sender_device_name_ = opts.optarg;
          break;
	case 'r':
	  this->distributer_device_name_ = opts.optarg;
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
                   char ** argv,
                   CORBA::Environment &ACE_TRY_ENV)
{
  // Initialize the endpoint strategy with the orb and poa.
  int result =
    this->a_endpoint_strategy_.init (TAO_AV_CORE::instance ()->orb (),
                                     TAO_AV_CORE::instance ()->poa ());
  if (result != 0)
    return result;

  result =
    this->b_endpoint_strategy_.init (TAO_AV_CORE::instance ()->orb (),
                                     TAO_AV_CORE::instance ()->poa ());
  if (result != 0)
    return result;

  // Parse the command line arguments
  result = this->parse_args (argc, 
                             argv);
  
  if (result < 0)
    ACE_ERROR_RETURN ((LM_ERROR,
		       " (%P|%t) Error in Parse Args \n"),
		      -1);

  // Initialize the helper class.
  this->helper_.init (TAO_AV_CORE::instance ()->orb (), 
                      TAO_AV_CORE::instance ()->poa ());
  

  ACE_NEW_RETURN (this->distributer_sender_mmdevice_,
                  TAO_MMDevice (&this->a_endpoint_strategy_),
                  -1);

  // Servant Reference Counting to manage lifetime
  PortableServer::ServantBase_var safe_sender_mmdevice =
    this->distributer_sender_mmdevice_;

  // Register the sender mmdevice object with the ORB 
  this->distributer_sender_mmdevice_obj_ =
    this->distributer_sender_mmdevice_->_this (ACE_TRY_ENV);
  ACE_CHECK_RETURN(-1);
  

  ACE_NEW_RETURN (this->distributer_receiver_mmdevice_,
                  TAO_MMDevice (&this->b_endpoint_strategy_),
                  -1);

  // Servant Reference Counting to manage lifetime
  PortableServer::ServantBase_var safe_receiver_mmdevice =
    this->distributer_receiver_mmdevice_;

  // Register the receiver mmdevice object with the ORB 
  this->distributer_receiver_mmdevice_obj_ =
    this->distributer_receiver_mmdevice_->_this (ACE_TRY_ENV);
  ACE_CHECK_RETURN(-1);
  
  // Bind to receivers
  result = this->helper_.bind_to_receivers (&this->distributer_device_name_,
                                            this->distributer_sender_mmdevice_obj_.in (),
                                            this->recv_obj_ref_set_,
                                            ACE_TRY_ENV);
  ACE_CHECK_RETURN (-1);

  
  if (result != 0)
    return result;

  ACE_CString flow_protocol_str ("");

  ACE_CString address;
  // Get the local host name
  char buf [BUFSIZ];
  ACE_OS::hostname (buf, 
                    BUFSIZ);
  
  // Set the address to the local host and port.
  address = buf;
  address += ":9000";
  

  // Connect to receivers
  result = this->helper_.connect_to_receivers (&this->distributer_device_name_,
                                               &flow_protocol_str,
                                               &this->protocol_,
                                               &address,
                                               this->distributer_sender_mmdevice_obj_.in (),
                                               &this->recv_obj_ref_set_,
                                               this->streamctrl_map_,
                                               ACE_TRY_ENV);
  ACE_CHECK_RETURN (-1);
  
  
  if (result != 0)
    return result;

  // Bind to sender
  result = this->helper_.bind_to_sender (&this->sender_device_name_,
                                         &this->distributer_device_name_,
                                         this->sender_mmdevice_.inout (),
                                         this->distributer_receiver_mmdevice_obj_.in (),
                                         ACE_TRY_ENV);
  ACE_CHECK_RETURN (-1);
  
  
  if (result != 0)
    return result;

  // Set the address to the local host and port.
  address = "";
  
  // Connect to sender
  result = this->helper_.connect_to_sender (&this->sender_device_name_,
                                            &this->distributer_device_name_,
                                            &flow_protocol_str,
                                            &this->protocol_,
                                            &address,
                                            this->sender_mmdevice_.inout (),
                                            this->distributer_receiver_mmdevice_obj_.in (),
                                            this->streamctrl_map_,
                                            ACE_TRY_ENV);
  ACE_CHECK_RETURN (-1);
  

  if (result != 0)
    return result;

  
  return 0;
}


StreamCtrl_Map*
Distributer::get_streamctrl_map (void)
{
  return &this->streamctrl_map_;
}

StreamEndpoint_Set* 
Distributer::get_endpoint_set (void)
{
  return &this->endpoint_set_;
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
      while (!DISTRIBUTER::instance ()->done () || orb->work_pending ())
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
template class TAO_AV_Endpoint_Reactive_Strategy_B <Distributer_Receiver_StreamEndPoint,TAO_VDev,AV_Null_MediaCtrl>;
template class TAO_AV_Endpoint_Reactive_Strategy <Distributer_Receiver_StreamEndPoint,TAO_VDev,AV_Null_MediaCtrl>;
template class TAO_AV_Endpoint_Reactive_Strategy_A <Distributer_Sender_StreamEndPoint,TAO_VDev,AV_Null_MediaCtrl>;
template class TAO_AV_Endpoint_Reactive_Strategy <Distributer_Sender_StreamEndPoint,TAO_VDev,AV_Null_MediaCtrl>;

#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)
#pragma instantiate ACE_Singleton <Distributer,ACE_Null_Mutex>
#pragma instantiate TAO_AV_Endpoint_Reactive_Strategy_B <Distributer_Receiver_StreamEndPoint,TAO_VDev,AV_Null_MediaCtrl>
#pragma instantiate TAO_AV_Endpoint_Reactive_Strategy <Distributer_Receiver_StreamEndPoint,TAO_VDev,AV_Null_MediaCtrl>
#pragma instantiate TAO_AV_Endpoint_Reactive_Strategy_A <Distributer_Sender_StreamEndPoint,TAO_VDev,AV_Null_MediaCtrl>
#pragma instantiate TAO_AV_Endpoint_Reactive_Strategy <Distributer_Sender_StreamEndPoint,TAO_VDev,AV_Null_MediaCtrl>

#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */
