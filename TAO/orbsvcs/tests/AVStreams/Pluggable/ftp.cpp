// $Id$

#include "ftp.h"

FTP_SFP_Object::FTP_SFP_Object (FTP_Client_SFP_Flow_Handler *handler)
  :handler_ (handler)
{
  handler_->set_sfp_object (this);
}

int
FTP_SFP_Object::start (void)
{
  this->handler_->start ();
  return 0;
}

FTP_Client_StreamEndPoint::FTP_Client_StreamEndPoint (TAO_ORB_Manager *orb_manager)
  :orb_manager_ (orb_manager)
{

}

int
FTP_Client_StreamEndPoint::make_udp_flow_handler (TAO_AV_UDP_Flow_Handler *&handler)
{
  ACE_DEBUG ((LM_DEBUG,"FTP_Client_StreamEndPoint::make_udp_flow_handler"));
  ACE_NEW_RETURN (handler,
                  FTP_Client_UDP_Flow_Handler (this->orb_manager_),
                  -1);
}

int
FTP_Client_StreamEndPoint::make_tcp_flow_handler (TAO_AV_TCP_Flow_Handler *&handler)
{
  ACE_DEBUG ((LM_DEBUG,"FTP_Client_StreamEndPoint::make_tcp_flow_handler"));
  ACE_NEW_RETURN (handler,
                  FTP_Client_TCP_Flow_Handler (this->orb_manager_),
                  -1);
}

int
FTP_Client_StreamEndPoint::make_dgram_mcast_flow_handler (TAO_AV_UDP_MCast_Flow_Handler *&handler)
{
  ACE_DEBUG ((LM_DEBUG,"FTP_Client_StreamEndPoint::make_udp_flow_handler"));
  ACE_NEW_RETURN (handler,
                  FTP_Client_UDP_MCast_Flow_Handler (this->orb_manager_),
                  -1);
}

int 
FTP_Client_StreamEndPoint::get_sfp_object (const char *flowname,
                                           TAO_SFP_Object *&sfp_object)
{
  ACE_DEBUG ((LM_DEBUG,"FTP_Client_StreamEndPoint::set_sfp_object\n"));
  FTP_Client_SFP_Flow_Handler *handler;
  ACE_NEW_RETURN (handler,
                  FTP_Client_SFP_Flow_Handler (this->orb_manager_),
                  -1);
  ACE_NEW_RETURN (sfp_object,
                  FTP_SFP_Object (handler),
                  -1);
  return 0;
}

FTP_Client_Flow_Handler::FTP_Client_Flow_Handler (TAO_ORB_Manager *orb_manager)
  :orb_manager_ (orb_manager),
   count_ (0)
{
}

int
FTP_Client_UDP_Flow_Handler::start (void)
{
  ACE_DEBUG ((LM_DEBUG,"FTP_Client_StreamEndPoint::start"));
  ACE_Time_Value interval (2);
  ACE_Time_Value delta = ACE_Time_Value::zero;
  this->timer_id_ =  
    TAO_AV_CORE::instance ()->reactor ()->schedule_timer (this,
                                                          0,
                                                          delta,
                                                          interval);
  return 0;
}

int
FTP_Client_TCP_Flow_Handler::start (void)
{
  ACE_DEBUG ((LM_DEBUG,"FTP_Client_StreamEndPoint::start"));
  ACE_Time_Value interval (2);
  ACE_Time_Value delta = ACE_Time_Value::zero;
  this->timer_id_ =  
    TAO_AV_CORE::instance ()->reactor ()->schedule_timer (this,
                                                          0,
                                                          delta,
                                                          interval);
  return 0;
}

int
FTP_Client_UDP_Flow_Handler::handle_timeout (const ACE_Time_Value &tv,
                                         const void *arg)
{
  return FTP_Client_Flow_Handler::handle_timeout (tv,arg);
}

int
FTP_Client_TCP_Flow_Handler::handle_timeout (const ACE_Time_Value &tv,
                                         const void *arg)
{
  return FTP_Client_Flow_Handler::handle_timeout (tv,arg);
}

FTP_Client_UDP_MCast_Flow_Handler::FTP_Client_UDP_MCast_Flow_Handler (TAO_ORB_Manager *orb_manager)
  :FTP_Client_Flow_Handler (orb_manager)
{
}

int
FTP_Client_UDP_MCast_Flow_Handler::start (void)
{
  ACE_DEBUG ((LM_DEBUG,"FTP_Client_StreamEndPoint::start"));
  ACE_Time_Value interval (2);
  ACE_Time_Value delta = ACE_Time_Value::zero;
  this->timer_id_ =  
    TAO_AV_CORE::instance ()->reactor ()->schedule_timer (this,
                                                          0,
                                                          delta,
                                                          interval);
  return 0;
}

int
FTP_Client_UDP_MCast_Flow_Handler::handle_timeout (const ACE_Time_Value &tv,
                                         const void *arg)
{
  return FTP_Client_Flow_Handler::handle_timeout (tv,arg);
}

int
FTP_Client_Flow_Handler::handle_timeout (const ACE_Time_Value &tv,
                                         const void *arg)
{
  ACE_DEBUG ((LM_DEBUG,"FTP_Client_StreamEndPoint::handle_timeout"));
  char buf [BUFSIZ];
  int result = ACE_OS::fread(buf,BUFSIZ,1,CLIENT::instance ()->file ());
  if (result < 0)
    {
      TAO_AV_CORE::instance ()->reactor ()->cancel_timer (this->timer_id_);
      ACE_ERROR_RETURN ((LM_ERROR,"FTP_Client_Flow_Handler::fread end of file\n"),-1);
    }
  if (result == 0)
    {
      if (::feof (CLIENT::instance ()->file ()))
        {
          // wait for sometime for the data to be flushed to the other side.
          this->count_++;
          if (this->count_ == 10)
            {
              ACE_DEBUG ((LM_DEBUG,"handle_timeout:End of file\n"));
              AVStreams::flowSpec stop_spec (1);
              stop_spec.length (1);
              ACE_DECLARE_NEW_CORBA_ENV;
              stop_spec [0] = CORBA::string_dup ("Data");
              CLIENT::instance ()->streamctrl ()->stop (stop_spec,ACE_TRY_ENV);
              ACE_CHECK_RETURN (-1);
            }
          else
            return 0;
        }
      else
        ACE_ERROR_RETURN ((LM_ERROR,"FTP_Client_Flow_Handler::fread error\n"),-1);
    }
  u_int len = BUFSIZ;
  result = this->transport_->send (buf,
                                   len);
  if (result < 0)
    ACE_ERROR_RETURN ((LM_ERROR,"send failed:%p","FTP_Client_Flow_Handler::send \n"),-1);
  if (result != len)
    ACE_DEBUG ((LM_DEBUG,"transport->send failed:%p",""));
  ACE_DEBUG ((LM_DEBUG,"handle_timeout::buffer sent succesfully\n"));
}

FTP_Client_UDP_Flow_Handler::FTP_Client_UDP_Flow_Handler (TAO_ORB_Manager *orb_manager)
  :FTP_Client_Flow_Handler (orb_manager)
{
}

FTP_Client_TCP_Flow_Handler::FTP_Client_TCP_Flow_Handler (TAO_ORB_Manager *orb_manager)
  :FTP_Client_Flow_Handler (orb_manager)
{
}

TAO_AV_Transport*
FTP_Client_SFP_Flow_Handler::transport (void)
{
  return 0;
}

void
FTP_Client_SFP_Flow_Handler::set_sfp_object (TAO_SFP_Object *object)
{
  this->sfp_object_ = object;
}

int
FTP_Client_SFP_Flow_Handler::start (void)
{
  ACE_Time_Value interval (2);
  ACE_Time_Value delta = ACE_Time_Value::zero;
  this->timer_id_ =  
    TAO_AV_CORE::instance ()->reactor ()->schedule_timer (this,
                                                          0,
                                                          delta,
                                                          interval);
  if (this->timer_id_ < 0)
    return -1;
  return 0;
}


FTP_Client_SFP_Flow_Handler::FTP_Client_SFP_Flow_Handler (TAO_ORB_Manager *orb_manager)
  :FTP_Client_Flow_Handler (orb_manager)
{
}

int
FTP_Client_SFP_Flow_Handler::handle_timeout (const ACE_Time_Value &tv,
                                             const void *arg)
{
  ACE_DEBUG ((LM_DEBUG,"FTP_Client_SFP_Flow_Handler::handle_timeout"));
  ACE_Message_Block mb (BUFSIZ);
  char *buf = mb.rd_ptr ();
  int result = ACE_OS::fread(buf,BUFSIZ,1,CLIENT::instance ()->file ());
  if (result < 0)
    {
      ACE_ERROR_RETURN ((LM_ERROR,"FTP_Client_SFP_Flow_Handler::fread end of file\n"),-1);
      TAO_AV_CORE::instance ()->reactor ()->cancel_timer (this->timer_id_);
      return -1;
    }
  if (result == 0)
    {
      if (::feof (CLIENT::instance ()->file ()))
        {
          // wait for sometime for the data to be flushed to the other side.
          ACE_DEBUG ((LM_DEBUG,"FTP_Client_SFP_Flow_Handler::handle_timeout:End of file\n"));
          result = this->sfp_object_->end_stream ();
          TAO_AV_CORE::instance ()->reactor ()->cancel_timer (this->timer_id_);
          if (result < 0)
            ACE_ERROR_RETURN ((LM_ERROR,"FTP_Client_SFP_Flow_Handler::end stream failed\n"),-1);
          return -1;
        }
      else
        ACE_ERROR_RETURN ((LM_ERROR,"FTP_Client_SFP_Flow_Handler::fread error\n"),-1);
    }
  cerr << result << endl;
  mb.wr_ptr (BUFSIZ*result);
  result = this->sfp_object_->send_frame (&mb);
  if (result < 0)
    ACE_ERROR_RETURN ((LM_ERROR,"send failed:%p","FTP_Client_Flow_Handler::send \n"),-1);
  ACE_DEBUG ((LM_DEBUG,"FTP_Client_SFP_Flow_Handler::handle_timeout::buffer sent succesfully\n"));
  return 0;
}

Endpoint_Reactive_Strategy::Endpoint_Reactive_Strategy (TAO_ORB_Manager *orb_manager,
                                                        Client *client)
  :ENDPOINT_STRATEGY (orb_manager),
   client_ (client),
   orb_manager_ (orb_manager)
{
}

int
Endpoint_Reactive_Strategy::make_stream_endpoint (FTP_Client_StreamEndPoint *&endpoint)
{
  ACE_DEBUG ((LM_DEBUG,"Endpoint_Reactive_Strategy::make_stream_endpoint"));
  ACE_NEW_RETURN (endpoint,
                  FTP_Client_StreamEndPoint (this->orb_manager_),
                  -1);
  return 0;
}

Client::parse_args (int argc,
                    char **argv)
{
  ACE_Get_Opt opts (argc,argv,"f:a:p:s");
  
  this->use_sfp_ = 0;
  char c;
  while ((c= opts ()) != -1)
    {
      switch (c)
        {
        case 'f':
          this->filename_ = ACE_OS::strdup (opts.optarg);
          break;
        case 'a':
          this->address_ = ACE_OS::strdup (opts.optarg);
          break;
        case 'p':
          this->protocol_ = ACE_OS::strdup (opts.optarg);
          break;
        case 's':
          this->use_sfp_ = 1;
          break;
        default:
          ACE_DEBUG ((LM_DEBUG,"Unknown option\n"));
          return -1;
          break;
        }
    }
  return 0;
}

FILE *
Client::file (void)
{
  return this->fp_;
}

TAO_StreamCtrl*
Client::streamctrl (void)
{
  return &this->streamctrl_;
}

Client::Client (void)
  :orb_manager_ (TAO_AV_CORE::instance ()->orb_manager ()),
   endpoint_strategy_ (orb_manager_,this),
   client_mmdevice_ (&endpoint_strategy_),
   fp_ (0),
   protocol_ (ACE_OS::strdup ("UDP"))
{
}


int
Client::bind_to_server (void)
{
  ACE_DECLARE_NEW_CORBA_ENV;

  ACE_TRY
    {
      /*
	CORBA::Object_var naming_obj =
	this->orb_manager_->orb ()->resolve_initial_references ("NameService");
	if (CORBA::is_nil (naming_obj.in ()))
	ACE_ERROR_RETURN ((LM_ERROR,
	" (%P|%t) Unable to resolve the Name Service.\n"),
	-1);
	CosNaming::NamingContext_var naming_context =
	CosNaming::NamingContext::_narrow (naming_obj.in (),
	TAO_TRY_ENV);
	TAO_CHECK_ENV;
      */

      // Initialize the naming services
      if (my_naming_client_.init (this->orb_manager_->orb ()) != 0)
	ACE_ERROR_RETURN ((LM_ERROR,
			   " (%P|%t) Unable to initialize "
			   "the TAO_Naming_Client. \n"),
			  -1);
      
      CosNaming::Name server_mmdevice_name (1);
      server_mmdevice_name.length (1);
      server_mmdevice_name [0].id = CORBA::string_dup ("Server_MMDevice");
      CORBA::Object_var server_mmdevice_obj =
        my_naming_client_->resolve (server_mmdevice_name,
                                  ACE_TRY_ENV);
      ACE_TRY_CHECK;

      this->server_mmdevice_ =
        AVStreams::MMDevice::_narrow (server_mmdevice_obj.in (),
                                      ACE_TRY_ENV);
      ACE_TRY_CHECK;

      if (CORBA::is_nil (this->server_mmdevice_.in ()))
        ACE_ERROR_RETURN ((LM_ERROR,
                           " could not resolve Server_Mmdevice in Naming service <%s>\n"),
                          -1);
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION,"Command_Handler::resolve_reference");
      return -1;
    }
  ACE_ENDTRY;
  ACE_CHECK_RETURN (-1);
  return 0;
}

int
Client::init (int argc,char **argv)
{
  this->argc_ = argc;
  this->argv_ = argv;

  // Increase the debug_level so that we can see the output
  //  TAO_debug_level++;
  CORBA::String_var ior;
  ACE_DECLARE_NEW_CORBA_ENV;
  ACE_TRY
    {
      TAO_AV_CORE::instance ()->init (argc,
                                      argv,
                                      ACE_TRY_ENV);
      ACE_TRY_CHECK;
      this->orb_manager_ = TAO_AV_CORE::instance ()->orb_manager ();
      this->orb_manager_->init_child_poa (this->argc_,
                                         this->argv_,
                                         "child_poa",
                                         ACE_TRY_ENV);
      ACE_TRY_CHECK;
      this->parse_args (this->argc_, this->argv_);
      // activate the client video mmdevice under the child poa.
      ior = this->orb_manager_->activate (&this->client_mmdevice_,
                                         ACE_TRY_ENV);
      ACE_TRY_CHECK;

      // Initialize the naming services
      CORBA::ORB_var orb = orb_manager_->orb ();
      if (this->my_naming_client_.init (orb.in ()) != 0)
        ACE_ERROR_RETURN ((LM_ERROR,
                           " (%P|%t) Unable to initialize "
                           "the TAO_Naming_Client. \n"),
                          -1);

      this->fp_ = ACE_OS::fopen (this->filename_,"r");
      if (this->fp_ != 0)
        {
          ACE_DEBUG ((LM_DEBUG,"file opened successfully\n"));
        }
      if (this->bind_to_server () == -1)
        ACE_ERROR_RETURN ((LM_ERROR, 
                           "(%P|%t) Error binding to the naming service\n"),
                          -1);
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION,"Client::init");
      return -1;
    }
  ACE_ENDTRY;
  ACE_CHECK_RETURN (-1);
  return 0;
}

int
Client::run (void)
{
  ACE_DECLARE_NEW_CORBA_ENV;
  ACE_TRY
    {
      char flow_protocol_str [BUFSIZ];
      if (this->use_sfp_)
        ACE_OS::strcpy (flow_protocol_str,"sfp:1.0");
      else
        ACE_OS::strcpy (flow_protocol_str,"");
      AVStreams::streamQoS_var the_qos (new AVStreams::streamQoS);
      AVStreams::flowSpec flow_spec (1);
      // Bind the client and server mmdevices.

      this->orb_manager_->activate_poa_manager (ACE_TRY_ENV);
      ACE_TRY_CHECK;
      ACE_INET_Addr addr (this->address_);
      TAO_Forward_FlowSpec_Entry entry ("Data",
                                        "IN",
                                        "USER_DEFINED",
                                        flow_protocol_str,
                                        this->protocol_,
                                        &addr);
      flow_spec [0] = entry.entry_to_string ();
      flow_spec.length (1);
      CORBA::Boolean result = 
        this->streamctrl_.bind_devs (this->client_mmdevice_._this (ACE_TRY_ENV),
                                     this->server_mmdevice_.in (),
                                     the_qos.inout (),
                                     flow_spec,
                                     ACE_TRY_ENV);
      ACE_TRY_CHECK;
      if (result == 0)
        ACE_ERROR_RETURN ((LM_ERROR,"streamctrl::bind_devs failed\n"),-1);
      AVStreams::flowSpec start_spec (1);
      start_spec.length (1);
      start_spec [0] = "Data";
      this->streamctrl_.start (start_spec,ACE_TRY_ENV);
      ACE_TRY_CHECK;
      // Schedule a timer for the for the flow handler.
      TAO_AV_CORE::instance ()->run ();
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION,"Client::run");
      return -1;
    }
  ACE_ENDTRY;
  ACE_CHECK_RETURN (-1);
  return 0;
}

int
main (int argc,
      char **argv)
{
  int result = 0;
  result = CLIENT::instance ()->init (argc,argv);
  if (result < 0)
    ACE_ERROR_RETURN ((LM_ERROR,"client::init failed\n"),1);
  result = CLIENT::instance ()->run ();
  if (result < 0)
    ACE_ERROR_RETURN ((LM_ERROR,"client::run failed\n"),1);

}

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)
template class ACE_Singleton <Client,ACE_Null_Mutex>;
template class TAO_AV_Endpoint_Reactive_Strategy_A<FTP_Client_StreamEndPoint,TAO_VDev,AV_Null_MediaCtrl>;
template class TAO_AV_Endpoint_Reactive_Strategy<FTP_Client_StreamEndPoint,TAO_VDev,AV_Null_MediaCtrl>;
#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)
#pragma instantiate ACE_Singleton <Client,ACE_Null_Mutex>
#pragma instantiate TAO_AV_Endpoint_Reactive_Strategy_A<FTP_Client_StreamEndPoint,TAO_VDev,AV_Null_MediaCtrl> 
#pragma instantiate TAO_AV_Endpoint_Reactive_Strategy<FTP_Client_StreamEndPoint,TAO_VDev,AV_Null_MediaCtrl> 
#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */
