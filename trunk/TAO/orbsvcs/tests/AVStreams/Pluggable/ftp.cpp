// $Id$

#include "ftp.h"
#include "ace/Get_Opt.h"
#include "ace/High_Res_Timer.h"
#include "ace/Stats.h"


ACE_hrtime_t recv_throughput_base = 0;
ACE_Throughput_Stats recv_latency;

ACE_hrtime_t send_throughput_base = 0;
ACE_Throughput_Stats send_latency;

int count =0;
int message_size = 64;

FTP_Client_Callback::FTP_Client_Callback (void)
  :count_ (0)
{
}

int
FTP_Client_Callback::handle_destroy (void)
{
  TAO_AV_CORE::instance ()->stop_run ();
  return 0;
}

void
FTP_Client_Callback::get_timeout (ACE_Time_Value *&tv,
                                  void *&)
{
  ACE_Time_Value *timeout;
  ACE_NEW (timeout,
           ACE_Time_Value(2));
  tv = timeout;
}

//@@coryan: Interpretation for the return value like ACE_Event_Handler's handle_timeout method.
int
FTP_Client_Callback::handle_timeout (void *)
{
  //@@coryan: Use a preallocated buffer for benchmarks.
  //Memory map the file.
  count++;
  ACE_Message_Block mb;
  mb.size (message_size);
  mb.wr_ptr (message_size);
  if (count == 10)
    {
      AVStreams::flowSpec stop_spec (1);
      ACE_DECLARE_NEW_CORBA_ENV;
      CLIENT::instance ()->streamctrl ()->stop (stop_spec,ACE_TRY_ENV);
      ACE_CHECK_RETURN (-1);
      CLIENT::instance ()->streamctrl ()->destroy (stop_spec,ACE_TRY_ENV);
      TAO_AV_CORE::instance ()->stop_run ();
      return 0;
    }
  ACE_hrtime_t stamp = ACE_OS::gethrtime ();
  //  ACE_Message_Block mb (BUFSIZ);
  ACE_OS::memcpy (mb.rd_ptr (), &stamp, sizeof(stamp));
  //  ACE_OS::memcpy (this->frame_.rd_ptr (), &stamp, sizeof(stamp));
  ACE_DEBUG ((LM_DEBUG,"FTP_Client_Callback::get_frame"));
  //char *buf = mb.rd_ptr ();
  //cerr << "message block size" << mb.size () << endl;
  //  int n = ACE_OS::fread(buf,1,mb.size (),CLIENT::instance ()->file ());
  //int n = ACE_OS::fread(buf,1,mb.size (),stamp);
  /*
  if (n < 0)
    {
      ACE_ERROR_RETURN ((LM_ERROR,"FTP_Client_Flow_Handler::fread end of file\n"),-1);
    }
  if (n == 0)
    {
      if (::feof (CLIENT::instance ()->file ()))
        {
          // wait for sometime for the data to be flushed to the other side.
          this->count_++;
          if (this->count_ == 2)
            {
              //@@coryan: Remove these code from this method. 
              //Should be called when the user wants to stop the stream.
              ACE_DEBUG ((LM_DEBUG,"handle_timeout:End of file\n"));
              AVStreams::flowSpec stop_spec (1);
              ACE_DECLARE_NEW_CORBA_ENV;
              CLIENT::instance ()->streamctrl ()->stop (stop_spec,ACE_TRY_ENV);
              ACE_CHECK_RETURN (-1);
              CLIENT::instance ()->streamctrl ()->destroy (stop_spec,ACE_TRY_ENV);
              TAO_AV_CORE::instance ()->stop_run ();
              
            }
          else
            return 0;
        }
      else
        ACE_ERROR_RETURN ((LM_ERROR,"FTP_Client_Flow_Handler::fread error\n"),-1);
    }
  cerr << "read bytes = " << n << endl;
  */
  //mb.wr_ptr (n);
  int result = this->protocol_object_->send_frame (&mb);
  if (result < 0)
    ACE_ERROR_RETURN ((LM_ERROR,"send failed:%p","FTP_Client_Flow_Handler::send \n"),-1);
  ACE_DEBUG ((LM_DEBUG,"handle_timeout::buffer sent succesfully\n"));
  return 0;
}

FTP_Client_StreamEndPoint::FTP_Client_StreamEndPoint (TAO_ORB_Manager *orb_manager)
  :orb_manager_ (orb_manager)
{

}

int
FTP_Client_StreamEndPoint::get_callback (const char *,
                                         TAO_AV_Callback *&callback)
{
  ACE_NEW_RETURN (this->callback_,
                  FTP_Client_Callback,
                  -1);
  callback = this->callback_;
  return 0;
}

int
FTP_Client_StreamEndPoint::set_protocol_object (const char *,
                                                TAO_AV_Protocol_Object *object)
{
  this->callback_->set_protocol_object (object);
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

int
Client::parse_args (int argc,
                    char **argv)
{
  ACE_Get_Opt opts (argc,argv,"f:a:p:sdt");
  
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
        case 'd':
          TAO_debug_level++;
          break;
        case 't':
          this->test_ = 1;
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
      // @@ Pass the ORB as an argument to the init.
      TAO_AV_CORE::instance ()->init (argc,
                                      argv,
                                      ACE_TRY_ENV);
      ACE_TRY_CHECK;
      this->orb_manager_ = TAO_AV_CORE::instance ()->orb_manager ();
      this->orb_manager_->init_child_poa (argc,
                                          argv,
                                          "child_poa",
                                          ACE_TRY_ENV);
      ACE_TRY_CHECK;
      this->parse_args (argc, argv);

      this->orb_manager_->activate_poa_manager (ACE_TRY_ENV);
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

      ACE_TRY_CHECK;
      ACE_INET_Addr addr (this->address_);
      ACE_NEW_RETURN (this->flowname_,
                      char [BUFSIZ],
                      0);
      ACE_OS::sprintf (this->flowname_,
                       "Data_%s",
                       this->protocol_);
      TAO_Forward_FlowSpec_Entry entry (this->flowname_,
                                        "IN",
                                        "USER_DEFINED",
                                        flow_protocol_str,
                                        this->protocol_,
                                        &addr);

      AVStreams::flowSpec flow_spec (1);
      flow_spec [0] = CORBA::string_dup (entry.entry_to_string ());
      flow_spec.length (1);
      //ACE_High_Res_Timer timer;
      //ACE_Time_Value elapsed;
      // timer.start ();
      CORBA::Boolean result = 
        this->streamctrl_.bind_devs (this->client_mmdevice_._this (ACE_TRY_ENV),
                                     this->server_mmdevice_.in (),
                                     the_qos.inout (),
                                     flow_spec,
                                     ACE_TRY_ENV);
      //timer.stop ();
      // timer.elapsed_time (elapsed);
      //elapsed.dump ();

      // If we're supposed to do only bind_devs time calculation return.
      if (this->test_)
        {
          AVStreams::flowSpec flow_spec;
          this->streamctrl_.destroy (flow_spec,ACE_TRY_ENV);
          ACE_TRY_CHECK;
          return 0;
        }
      ACE_TRY_CHECK;
      if (result == 0)
        ACE_ERROR_RETURN ((LM_ERROR,"streamctrl::bind_devs failed\n"),-1);

      AVStreams::flowSpec start_spec (1);
      //start_spec.length (1);
      //start_spec [0] = CORBA::string_dup (this->flowname_);
      this->streamctrl_.start (start_spec,ACE_TRY_ENV);
      ACE_TRY_CHECK;

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
  ACE_DEBUG ((LM_DEBUG, "Calibrating scale factory . . . "));
  ACE_UINT32 gsf = ACE_High_Res_Timer::global_scale_factor ();
  ACE_DEBUG ((LM_DEBUG, "done\n"));
  
  recv_latency.dump_results ("Receive", gsf);
  
  send_latency.dump_results ("Send", gsf);
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
