// $Id$

#include "ftp.h"

FTP_Client_Callback::FTP_Client_Callback (FTP_Client_Flow_Handler *handler)
  :handler_ (handler)
{
}

int
FTP_Client_Callback::handle_start (void)
{
  ACE_DEBUG ((LM_DEBUG,"FTP_Client_Callback::handle_start"));
  return this->handler_->start ();
}

int
FTP_Client_Callback::handle_stop (void)
{
  ACE_DEBUG ((LM_DEBUG,"FTP_Client_Callback::handle_stop"));
  return this->handler_->stop ();
}


FTP_Client_Flow_Handler::FTP_Client_Flow_Handler (TAO_ORB_Manager *orb_manager,
                                                  ACE_Time_Value &timeout)
  :TAO_FlowProducer ("Data",CLIENT::instance ()->protocols (),CLIENT::instance ()->format ()),
   orb_manager_ (orb_manager),
   count_ (0),
   timeout_ (timeout)
{
}

int
FTP_Client_Flow_Handler::get_callback (const char *flowname,
                                       TAO_AV_Callback *&callback)
{
  ACE_DEBUG ((LM_DEBUG,"FTP_Client_Flow_Handler::get_callback\n"));
  ACE_NEW_RETURN (callback,
                  FTP_Client_Callback (this),
                  -1);
  return 0;
}

int
FTP_Client_Flow_Handler::start (void)
{
  ACE_DEBUG ((LM_DEBUG,"FTP_Client_Flow_Handler::start"));
  ACE_Time_Value delta = ACE_Time_Value::zero;
  this->timer_id_ =  
    TAO_AV_CORE::instance ()->reactor ()->schedule_timer (this,
                                                          0,
                                                          delta,
                                                          this->timeout_);
  return 0;
}

int
FTP_Client_Flow_Handler::stop (void)
{
  ACE_DEBUG ((LM_DEBUG,"FTP_Client_Flow_Handler::stop"));
  int result = TAO_AV_CORE::instance ()->reactor ()->cancel_timer (this->timer_id_);
  if (result < 0)
    ACE_ERROR_RETURN ((LM_ERROR,"FTP_Client_Flow_Handler::stop cancel timer failed\n"),-1);
  return 0;
}

int
FTP_Client_Flow_Handler::handle_timeout (const ACE_Time_Value &tv,
                                         const void *arg)
{
  ACE_DEBUG ((LM_DEBUG,"FTP_Client_StreamEndPoint::handle_timeout"));
  ACE_Message_Block mb (BUFSIZ);
  char *buf = mb.rd_ptr ();
  cerr << "message block size" << mb.size () << endl;
  int n = ACE_OS::fread(buf,mb.size (),1,CLIENT::instance ()->file ());
  if (n < 0)
    {
      TAO_AV_CORE::instance ()->reactor ()->cancel_timer (this->timer_id_);
      ACE_ERROR_RETURN ((LM_ERROR,"FTP_Client_Flow_Handler::fread end of file\n"),-1);
    }
  if (n == 0)
    {
      if (::feof (CLIENT::instance ()->file ()))
        {
          // wait for sometime for the data to be flushed to the other side.
          this->count_++;
          if (this->count_ == 10)
            {
              ACE_DECLARE_NEW_CORBA_ENV;
              ACE_DEBUG ((LM_DEBUG,"handle_timeout:End of file\n"));
              AVStreams::flowSpec stop_spec (1);
//               stop_spec.length (1);
//               stop_spec [0] = CORBA::string_dup (CLIENT::instance ()->flowname ());
              CLIENT::instance ()->streamctrl ()->stop (stop_spec,ACE_TRY_ENV);
              ACE_CHECK_RETURN (-1);
              return 0;
            }
          else
            return 0;
        }
      else
        ACE_ERROR_RETURN ((LM_ERROR,"FTP_Client_Flow_Handler::fread error\n"),-1);
    }
  cerr << "read bytes = " << n << endl;
  mb.wr_ptr (n*BUFSIZ);
  int result = this->protocol_object_->send_frame (&mb);
  if (result < 0)
    ACE_ERROR_RETURN ((LM_ERROR,"send failed:%p","FTP_Client_Flow_Handler::send \n"),-1);
  ACE_DEBUG ((LM_DEBUG,"handle_timeout::buffer sent succesfully\n"));
}

//------------------------------------------------------------
// FTP_Client_FDev
//------------------------------------------------------------

FTP_Client_FDev::FTP_Client_FDev (TAO_ORB_Manager *orb_manager)
  :TAO_FDev (CORBA::string_dup ("Data")),
   orb_manager_ (orb_manager)
{
}

AVStreams::FlowProducer_ptr
FTP_Client_FDev::make_producer (AVStreams::FlowConnection_ptr the_requester,
                                AVStreams::QoS & the_qos,
                                CORBA::Boolean_out met_qos,
                                char *& named_fdev,
                                CORBA::Environment &ACE_TRY_ENV)
{
  ACE_DEBUG ((LM_DEBUG,"FTP_Client_FDev::make_producer\n"));
  FTP_Client_Flow_Handler *handler;
  ACE_Time_Value timeout (2);
  ACE_NEW_RETURN (handler,
                  FTP_Client_Flow_Handler (this->orb_manager_,
                                           timeout),
                  0);
  AVStreams::FlowProducer_ptr producer = handler->_this (ACE_TRY_ENV);
  ACE_CHECK_RETURN (0);
  return producer;
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

char*
Client::flowname (void)
{
  return this->flowname_;
}

AVStreams::protocolSpec
Client::protocols (void)
{
  AVStreams::protocolSpec protocols (1);
  protocols.length (1);
  char buf [BUFSIZ];
  ACE_OS::sprintf (buf,"%s=%s",this->protocol_,this->address_);
  protocols [0] = CORBA::string_dup (buf);
  return protocols;
}

const char *
Client::format (void)
{
  return "UNS:ftp";
}

const char *
Client::address (void)

{
  return this->address_;
}

TAO_StreamCtrl*
Client::streamctrl (void)
{
  return &this->streamctrl_;
}

Client::Client (void)
  :orb_manager_ (TAO_AV_CORE::instance ()->orb_manager ()),
   endpoint_strategy_ (orb_manager_),
   client_mmdevice_ (&endpoint_strategy_),
   fdev_ (0),
   fp_ (0),
   protocol_ (ACE_OS::strdup ("UDP"))
{
}


int
Client::bind_to_server (const char *name)
{
  ACE_DECLARE_NEW_CORBA_ENV;

  ACE_TRY
    {
      // Initialize the naming services
      if (my_naming_client_.init (this->orb_manager_->orb ()) != 0)
        ACE_ERROR_RETURN ((LM_ERROR,
                           " (%P|%t) Unable to initialize "
                           "the TAO_Naming_Client. \n"),
                          -1);
      
      CosNaming::Name server_mmdevice_name (1);
      server_mmdevice_name.length (1);
      server_mmdevice_name [0].id = name;
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

      this->orb_manager_->activate_poa_manager (ACE_TRY_ENV);
      ACE_TRY_CHECK;
      ACE_NEW_RETURN (this->fdev_,
                      FTP_Client_FDev (this->orb_manager_),
                      -1);

      AVStreams::MMDevice_var mmdevice = this->client_mmdevice_._this (ACE_TRY_ENV);
      ACE_TRY_CHECK;
      AVStreams::FDev_var fdev = this->fdev_->_this (ACE_TRY_ENV);
      ACE_TRY_CHECK;
      mmdevice->add_fdev (fdev.in (),
                          ACE_TRY_ENV);

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

      ACE_INET_Addr addr (this->address_);
      ACE_NEW_RETURN (this->flowname_,
                      char [BUFSIZ],
                      0);
      ACE_OS::sprintf (this->flowname_,
                       "Data",
                       this->protocol_);
      TAO_Forward_FlowSpec_Entry entry (this->flowname_,
                                        "IN",
                                        "USER_DEFINED",
                                        flow_protocol_str,
                                        this->protocol_,
                                        &addr);
      flow_spec [0] = CORBA::string_dup (entry.entry_to_string ());
      flow_spec.length (1);
      CORBA::Boolean result = 
        this->streamctrl_.bind_devs (this->client_mmdevice_._this (ACE_TRY_ENV),
                                     AVStreams::MMDevice::_nil (),
                                     the_qos.inout (),
                                     flow_spec,
                                     ACE_TRY_ENV);
      ACE_TRY_CHECK;
      if (this->bind_to_server ("Server_MMDevice1") == -1)
        ACE_ERROR_RETURN ((LM_ERROR, 
                           "(%P|%t) Error binding to the naming service\n"),
                          -1);
      result = this->streamctrl_.bind_devs (AVStreams::MMDevice::_nil (),
                                            this->server_mmdevice_.in (),
                                            the_qos.inout (),
                                            flow_spec,
                                            ACE_TRY_ENV);
      ACE_TRY_CHECK;
      if (this->bind_to_server ("Server_MMDevice2") == -1)
        ACE_ERROR_RETURN ((LM_ERROR, 
                           "(%P|%t) Error binding to the naming service\n"),
                          -1);
      result = this->streamctrl_.bind_devs (AVStreams::MMDevice::_nil (),
                                            this->server_mmdevice_.in (),
                                            the_qos.inout (),
                                            flow_spec,
                                            ACE_TRY_ENV);
      ACE_TRY_CHECK;

      if (result == 0)
        ACE_ERROR_RETURN ((LM_ERROR,"streamctrl::bind_devs failed\n"),-1);
      AVStreams::flowSpec start_spec (1);
      start_spec.length (1);
      start_spec [0] = CORBA::string_dup (this->flowname_);
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
